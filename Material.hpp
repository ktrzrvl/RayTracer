#pragma once
#include <iostream>
#include <vector>
#include "Vector.hpp"

struct Material {
	Vector color;
	double specular;
	double refractive;
	vector<double> albedo;
};