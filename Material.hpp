#pragma once
#include <iostream>
#include <vector>
#include "Vector.hpp"

struct Material {
	Vector color;
	double specular;
	double refractive;
	double albedo[4];

	friend istream& operator>>(istream &is, Material& m);
};

istream& operator>>(istream &is, Material& m) {
	return is >> m.color >> m.specular >> m.refractive >> m.albedo[0] >> m.albedo[1] >> m.albedo[2] >> m.albedo[3];
}