#include <iostream>
#include "Sphere.hpp"
#include "Vector.hpp"
#include "Bitmap.hpp"
#include "Light.hpp"

using namespace std;

const double INF = 1e300;
const int DEPTH = 5;
class RayTracer {
	int width;
	int height;
	vector<Sphere> spheres;
	vector<Light> lights;

	Vector TraceRay(const Vector& O, const Vector& D, double tmin, double tmax, int depth);
	int ClosestIntersection(const Vector& O, const Vector& D, double tmin, double tmax, double& t); // индекс сферы с ближайшим пересечением
	bool HaveIntersection(const Vector& O, const Vector& D, double tmin, double tmax); 
	double ComputeLightning(Vector P, Vector N, const Vector &D, double specular);
	Vector RefractRay(const Vector& L, const Vector& N);
public:
	RayTracer(int width, int height);
	void Trace();
};

RayTracer::RayTracer(int width, int height) {
	this->width = width;
	this->height = height;

	Material m1 = { Vector(1, 0, 0), 50, 0.2 };
	Material m2 = { Vector(0, 0, 1), 500, 0.3 };
	Material m3 = { Vector(0, 1, 0), 10, 0.4 };
	Material m4 = { Vector(1, 1, 0), 1000, 0.5 };

	spheres.push_back(Sphere(Vector(0, -1, 3), 1, m1));
	spheres.push_back(Sphere(Vector(2, 0, 4), 1, m2));
	spheres.push_back(Sphere(Vector(-2, 0, 4), 1, m3));
	spheres.push_back(Sphere(Vector(0, -5001, 0), 5000, m4));

	lights.push_back(Light(0.2));
	lights.push_back(Light(POINT, 0.6, Vector(2, 1, 0)));
	lights.push_back(Light(DIRECTIONAL, 0.2, Vector(1, 4, 4)));
}

void RayTracer::Trace() {
	Vector O(0,0,0);
	Bitmap bitmap(width, height);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			Vector D((double) j / width - 0.5, 0.5 - (double) i / height, 1);
			D.Normalize();
			Vector color = TraceRay(O, D, 1, INF, DEPTH);
			int r = min(255.0, color.GetX() * 255);
			int g = min(255.0, color.GetY() * 255);
			int b = min(255.0, color.GetZ() * 255);

			bitmap.set_pixel(j, i, r, g, b);
		}
	}
	bitmap.save_image("out.bmp");
}

int RayTracer::ClosestIntersection(const Vector& O, const Vector& D, double tmin, double tmax, double& t) {
	int index = -1;
	for (int i = 0; i < spheres.size(); i++) {
		double t0;
		if (spheres[i].IsIntersect(O, D, t0) && t0 > tmin && t0 < tmax && (index == -1 || t0 < t)) {
			t = t0;
			index = i;
		}
	}
	return index;
}

bool RayTracer::HaveIntersection(const Vector& O, const Vector& D, double tmin, double tmax) {
	for (int i = 0; i < spheres.size(); i++) {
		double t0;
		if (spheres[i].IsIntersect(O, D, t0) && t0 > tmin && t0 < tmax) {
			return true;
		}
	}

	return false;
}


Vector RayTracer::TraceRay(const Vector& O, const Vector& D, double tmin, double tmax, int depth) {
	double closest_t;
	int closest_index = ClosestIntersection(O, D, tmin, tmax, closest_t);
	if (closest_index == -1) {
		return Vector(0,0,0);
	}

	Vector P = O + D * closest_t;
	Vector N = spheres[closest_index].GetNormal(P);
	Material material = spheres[closest_index].GetMaterial();

	double intense = ComputeLightning(P, N, D, material.specular);
	Vector color = material.color * intense;

	if (material.specular == 0 || depth <= 0) {
		return color;
	}

	Vector vec = RefractRay((D * (-1)), N);
	Vector reflected_color = TraceRay(P, vec, 0.001, INF, DEPTH - 1);
	color = color * (1 - material.reflective) + reflected_color * material.reflective;

	return color;
}

double RayTracer::ComputeLightning(Vector P, Vector N, const Vector &D, double specular) {
	double intense = 0;
	for (int i = 0; i < lights.size(); i++) {
		if (lights[i].type == AMBIENT) {
			intense += lights[i].intensity;
		}
		else {
			Vector L;
			double tmax;
			if (lights[i].type == POINT) {
				L = lights[i].vec - P;
				tmax = L.Length();
			}
			else {
				L = lights[i].vec;
				tmax = INF;
			}

			L.Normalize();

			if (HaveIntersection(P, L, 0.001, tmax)) 
				continue;

			double NL = N * L;
			if (NL > 0) {
				intense += NL * lights[i].intensity;
			}

			double LD = -(RefractRay(L, N) * D);
			if (LD > 0) {
				intense += pow(LD, specular) * lights[i].intensity;
			}
		}
	}
	return intense;
}

Vector RayTracer::RefractRay(const Vector& L, const Vector& N) {
	return N * ((L * N) * 2) - L;
}
