#pragma once
#include <iostream>
#include <cmath>

using namespace std;

class Vector {
	double x, y, z;

public:
	Vector(double x1 = 0, double y1 = 0, double z1 = 0);
	Vector operator+(const Vector &a) const;
	Vector operator-(const Vector &a) const;
	Vector operator*(double n) const;
	double operator*(const Vector &a) const;
	double Length() const;
	void Normalize();
	double GetX() const;
	double GetY() const;
	double GetZ() const;

	friend istream& operator>>(istream &is, Vector& v);
};

Vector::Vector(double x1, double y1, double z1) {
	x = x1;
	y = y1;
	z = z1;
}

Vector Vector::operator+(const Vector &a) const {
	double x1 = a.x + x;
	double y1 = a.y + y;
	double z1 = a.z + z;
	return Vector(x1, y1, z1);
}

Vector Vector::operator-(const Vector &a) const {
	double x1 = x - a.x;
	double y1 = y - a.y;
	double z1 = z - a.z;
	return Vector(x1, y1, z1);
}

Vector Vector::operator*(double n) const {
	return Vector(n*x, n*y, n*z);
}

double Vector::operator*(const Vector &a) const {
	return x*a.x + y*a.y + z*a.z;
}

double Vector::Length() const {
	return sqrt(x*x + y*y + z*z);
}

void Vector::Normalize() {
	double leng = Length();
	if (leng == 0) {
		return;
	}
	x /= leng;
	y /= leng;
	z /= leng;
}

double Vector::GetX() const {
	return x;
}

double Vector::GetY() const {
	return y;
}

double Vector::GetZ() const {
	return z;
}

istream& operator>>(istream &is, Vector& v) {
	is >> v.x >> v.y >> v.z;
	return is;
}