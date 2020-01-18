#pragma once
#include <iostream>
#include <cmath>
#include "Vector.hpp"
#include "Material.hpp"

using namespace std;

class Sphere {
	Vector C;
	double R;
	Material material;

public:
	Sphere(const Vector& c, double rad, Material material);
	bool IsIntersect(const Vector &O, const Vector &D, double &t) const;
	Vector GetNormal(const Vector& P);
	Material GetMaterial() const;
};

Sphere::Sphere(const Vector& c, double rad, Material material) {
	C = c;
	R = rad;
	this->material = material;
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

Material Sphere::GetMaterial() const {
	return material;
}

Vector Sphere::GetNormal(const Vector& P) {
	return Vector((P.GetX() - C.GetX()) / R, (P.GetY() - C.GetY()) / R, (P.GetZ() - C.GetZ()) / R);
}
