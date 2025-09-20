#include "Complex.h"

Complex::Complex(double r, double i): real(r), imag(i) {}

Complex Complex::operator+(const Complex& other) const {
	return Complex(real + other.real, imag + other.imag);	
}

Complex Complex::operator-(const Complex& other) const {
	return Complex(real - other.real, imag - other.imag);
}

Complex Complex::operator*(double k) const {
	return Complex(real * k, imag * k);
}

double Complex::abs() const {
	return std::sqrt(real * real + imag * imag);
}

std::ostream&  operator<<(std::ostream& os, const Complex& c) {
	os << "(" << c.real << "," << c.imag << ")";
	return os;
}

