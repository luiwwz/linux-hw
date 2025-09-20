#ifndef COMPLEX_H
#define COMPLEX_H

#include <iostream>
#include <cmath>

class Complex {
private:
	double real;
	double imag;
public:
 	Complex(double r = 0.0, double i = 0.0);

	Complex operator+(const Complex& other) const;
	Complex operator-(const Complex& other) const;
	Complex operator*(double k) const;

	double abs() const;

	friend std::ostream& operator<<(std::ostream& os, const Complex& c);
};

#endif
