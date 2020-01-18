#pragma once
#include <iostream>
#include "Vector.hpp"

struct Material {
	Vector color;
	double specular;
	double reflective;
};