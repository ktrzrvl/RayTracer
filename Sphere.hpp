#pragma once
#include <iostream>
#include <cmath>
#include "Vector.hpp"

using namespace std;

class Sphere {
	Vector C;
	double R;
	Vector color;
	double specular;
	double reflective;

public:
	Sphere(const Vector& c, double rad, const Vector& color, double specular, double reflective);
	bool IsIntersect(const Vector &O, const Vector &D, double &t) const;
	Vector GetColor() const;
	Vector GetNormal(const Vector& P);
	double GetSpecular();
	double GetReflective();
};

Sphere::Sphere(const Vector& c, double rad, const Vector& color, double specular, double reflective) {
	C = c;
	R = rad;
	this->color = color;
	this->specular = specular;
	this->reflective = reflective;
}

bool Sphere::IsIntersect(const Vector &O, const Vector &D, double &t) const {
	Vector OC = O - C;
	double ocd = OC * D;
	double oc = OC * OC;
	double disc = ocd * ocd - (oc - R*R);
	if (disc < 0) {
		return false;
	}
	disc = sqrt(disc);
	t = -ocd - disc;
	if (t < 0) {
		t = -ocd + disc;
	}
	return t >= 0;
}

Vector Sphere::GetColor() const {
	return color;
}

Vector Sphere::GetNormal(const Vector& P) {
	return Vector((P.GetX() - C.GetX()) / R, (P.GetY() - C.GetY()) / R, (P.GetZ() - C.GetZ()) / R);
}

double Sphere::GetSpecular() {
	return specular;
}

double Sphere::GetReflective() {
	return reflective;
}
