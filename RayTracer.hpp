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
	void ComputeLightning(Vector P, Vector N, const Vector &D, Material material, double& diffuse, double& specular);
	Vector ReflectRay(const Vector& L, const Vector& N);
	bool RefractRay(const Vector &I, const Vector &N, double eta1, double eta2, Vector &D);
public:
	RayTracer(int width, int height);
	void Trace();
};

RayTracer::RayTracer(int width, int height) {
	this->width = width;
	this->height = height;

	Material ivory = { Vector(0.4, 0.4, 0.3), 50, 1.0, {0.6, 0.3, 0.1, 0.0} };
	Material glass = { Vector(0.6, 0.7, 0.8), 125, 1.5, {0.0, 0.5, 0.1, 0.8} };
	Material red_rubber = { Vector(0.3, 0.1, 0.1), 10, 1.0, {0.9, 0.1, 0.0, 0.0} };
	Material mirror = { Vector(1, 1, 1), 1425, 1.0, {0.0, 10.0, 0.8, 0.0} };

	spheres.push_back(Sphere(Vector(0, -1, 3), 1, ivory));
	spheres.push_back(Sphere(Vector(2, 0, 4), 1, mirror));
	spheres.push_back(Sphere(Vector(-2, 0, 4), 1, red_rubber));
	spheres.push_back(Sphere(Vector(0, -5001, 0), 5000, ivory));

	lights.push_back(Light(0.4));
	lights.push_back(Light(POINT, 1.2, Vector(2, 1, 0)));
	lights.push_back(Light(DIRECTIONAL, 0.4, Vector(1, 4, 4)));
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
		return Vector(0.2,0.7,0.8);
	}

	Vector P = O + D * closest_t;
	Vector N = spheres[closest_index].GetNormal(P);
	Material material = spheres[closest_index].GetMaterial();
	double diffuse = 0;
	double specular = 0;

	ComputeLightning(P, N, D, material, diffuse, specular);
	Vector color = material.color * (diffuse * material.albedo[0] + specular * material.albedo[1]);

	if (depth <= 0) {
		return color;
	}

	if (material.albedo[2] > 0) {
		Vector vec = ReflectRay((D * (-1)), N);
		Vector reflected_color = TraceRay(P, vec, 0.001, INF, depth - 1);
		color = color + reflected_color * material.albedo[2];
	}

	if (material.albedo[3] > 0) {
		Vector vec;
		if (RefractRay(D, N, material.refractive, 1, vec)) {
			Vector refracted_color = TraceRay(P + N * 0.0001, vec, 0.001, INF, depth - 1);
			color = color + refracted_color * material.albedo[3];
		}
	}

	return color;
}

void RayTracer::ComputeLightning(Vector P, Vector N, const Vector &D, Material material, double& diffuse, double& specular) {
	for (int i = 0; i < lights.size(); i++) {
		if (lights[i].type == AMBIENT) {
			diffuse += lights[i].intensity;
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
				diffuse += NL * lights[i].intensity;
			}

			double LD = -(ReflectRay(L, N) * D);
			if (LD > 0) {
				specular += pow(LD, material.specular) * lights[i].intensity;
			}
		}
	}
}

Vector RayTracer::ReflectRay(const Vector& L, const Vector& N) {
	return N * ((L * N) * 2) - L;
}

bool RayTracer::RefractRay(const Vector &I, const Vector &N, double eta1, double eta2, Vector &D) {
	double cos_a = -(I * N);
	double eta, sign;
	if (cos_a < 0) {
		eta = eta1 / eta2;
		sign = 1;
	}
	else {
		eta = eta2 / eta1; 
		sign = -1;
	}
	double k = 1 - eta * eta * (1 - cos_a * cos_a);
	if (k < 0) {
		return false;
	}
	D = I*eta + N * (eta*cos_a + sign * sqrt(k));
	return true;
}
