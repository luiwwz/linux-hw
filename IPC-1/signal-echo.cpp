#include <iostream>
#include <csignal>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <cstring>
#include <ucontext.h>

void handler(int sig, siginfo_t* info, void* context) {
    pid_t senderPid = info->si_pid;
    uid_t senderUid = info->si_uid;

    passwd* pw = getpwuid(senderUid);
    const char* username = pw ? pw->pw_name : "unknown";

    ucontext_t* uc = (ucontext_t*)context;
    unsigned long eip = uc->uc_mcontext.gregs[REG_RIP];
    unsigned long eax = uc->uc_mcontext.gregs[REG_RAX];
    unsigned long ebx = uc->uc_mcontext.gregs[REG_RBX];

    std::cout << "Got SIGUSR1 from PID " << senderPid 
              << ", UID " << senderUid << " (" << username << ")\n";

    std::cout << "Regs: EIP=" << eip 
              << " EAX=" << eax 
              << " EBX=" << ebx << "\n";
}

int main() {
    std::cout << "Process PID = " << getpid() << std::endl;

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = handler;
    sa.sa_flags = SA_SIGINFO;

    sigaction(SIGUSR1, &sa, nullptr);

    if (sigaction(SIGUSR1, &sa, nullptr) == -1) {
        std::cerr << "sigaction failed\n";
        return 1;
    }

    while (true) {
        sleep(10);
    }

    return 0;
}
