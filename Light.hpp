#pragma once
#include <iostream>
#include <cmath>
#include "Sphere.hpp"
#include "Vector.hpp"

using namespace std;

const int AMBIENT = 0; // источник постоянного освещения
const int DIRECTIONAL = 1; // источник направленного освещения
const int POINT = 2; // источник точечного освещения

class Light {
public:
	int type;
	double intensity;
	Vector vec;

	Light(double intensity);
	Light(int type, double intensity, Vector vec);
};

Light::Light(double intensity) {
	this->type = AMBIENT;
	this->intensity = intensity;
}

Light::Light(int type, double intensity, Vector vec) {
	this->type = type;
	this->intensity = intensity;
	this->vec = vec;
}
