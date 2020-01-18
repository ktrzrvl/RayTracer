#include <iostream>
#include <cmath>
#include "Sphere.hpp"
#include "Vector.hpp"
#include "Bitmap.hpp"
#include "RayTracer.hpp"

using namespace std;

int main() {
	RayTracer tracer(1000, 1000);
	tracer.Trace();
}