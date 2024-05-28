#ifndef AAB_H
#define AABB_H

#include "RayTracing.h"

class aabb {
public:
	interval x, y, z;

	aabb() {} // The default AABB is empty, since intervals are empty by default

	aabb(const interval& x, const interval& y, const interval& z) : x(x), y(y), z(z) {}

	aabb(const point3& a, const point3& b) {
		// Treat the two points a and b as extrema for the bounding box, so we don't require a particular minimum / maximum coordinate order
		x = (a[0] <= b[0]) ? interval(a[0], b[0]) : interval(b[0], a[0]);
		y = (a[1] <= b[1]) ? interval(a[1], b[1]) : interval(b[1], a[1]);
		z = (a[2] <= b[2]) ? interval(a[2], b[2]) : interval(b[2], a[2]);
	}

	const interval& axis_interval(int n) const {
		if (n == 1) return y;
		if (n == 2) return z;
		return x;
	}

	bool hit(const ray& r, interval ray_t) const {
		const point3& ray_orig = r.origin();
		const vec3& ray_dir = r.direction();

		for (int axis = 0; axis < 3; axis++) {
			const interval& ax = axis_interval(axis);
			const double adinv = 1.0 / ray_dir[axis];

			double t0 = (ax.min - ray_orig[axis]) * adinv;
			double t1 = (ax.max - ray_orig[axis]) * adinv;

			/*if (adinv < 0)
				std::swap(t0, t1);
				ray_t.min = (t0 > ray_t.min) ? t0 : ray_t.min;
				ray_t.max = (t1 < ray_t.max) ? t1 : ray_t.max;
			*/
			if (t0 < t1) {
				if (t0 > ray_t.min) ray_t.min = t0;
				if (t1 < ray_t.max) ray_t.max = t1;
			}
			else {
				if (t1 > ray_t.min) ray_t.min = t1;
				if (t0 < ray_t.max) ray_t.max = t0;
			}

			if (ray_t.max <= ray_t.min)
				return false;
		}
		return true;
		//TODO: Test if this is better
		/*
		// IMPROVED: Faster method using vector instructions
		// This method is a bit faster because of modern CPUs (vector instructions) and newer compilers (doing auto-vectorization)
		// Adapted from: https://medium.com/@bromanz/another-view-on-the-classic-ray-aabb-intersection-algorithm-for-bvh-traversal-41125138b525
		const double t0[3] = {
			(x.min - ray_orig[0]) / r.direction()[0],
			(y.min - ray_orig[1]) / r.direction()[1],
			(z.min - ray_orig[2]) / r.direction()[2]
		};

		const double t1[3] = {
			(x.max - ray_orig[0]) / r.direction()[0],
			(y.max - ray_orig[1]) / r.direction()[1],
			(z.max - ray_orig[2]) / r.direction()[2]
		};

		const double t_smaller[3] = {
			std::fmin(t0[0], t1[0]),
			std::fmin(t0[1], t1[1]),
			std::fmin(t0[2], t1[2])
		};

		const double t_larger[3] = {
			std::fmax(t0[0], t1[0]),
			std::fmax(t0[1], t1[1]),
			std::fmax(t0[2], t1[2])
		};

		ray_t.min = std::max({ ray_t.min, t_smaller[0], t_smaller[1], t_smaller[2] });
		ray_t.max = std::min({ ray_t.max, t_larger[0], t_larger[1], t_larger[2] });
		return (ray_t.min < ray_t.max);
		*/
	}
};

#endif