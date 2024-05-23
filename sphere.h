#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class sphere : public hittable {
public:
	sphere(const point3& center, double radius) : center(center), radius(fmax(0, radius)) {}
	
	bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override {
		vec3 oc = center - r.origin();
		// Mathematically vec3.length_squared() is the same as dot of a vec3 with itself
		double a = r.direction().length_squared();
		double h = dot(r.direction(), oc); // The -2.0 will simplify itself out
		double c = oc.length_squared() - radius * radius;
		double discriminant = h * h - a * c; // Simplified out because of -2.0
		if (discriminant < 0) {
			return false;
		}
		double sqrtd = sqrt(discriminant);

		// Find the nearest root that lies in the acceptable range
		double root = (h - sqrtd) / a;
		if (root <= ray_tmin || ray_tmax <= root) {
			root = (h + sqrtd) / a;
			if (root <= ray_tmin || ray_tmax <= root)
				return false;
		}

		rec.t = root;
		rec.p = r.at(rec.t);
		vec3 outward_normal = (rec.p - center) / radius;
		rec.set_face_normal(r, outward_normal);

		return true;
	}
private:
	point3 center;
	double radius;
};

#endif