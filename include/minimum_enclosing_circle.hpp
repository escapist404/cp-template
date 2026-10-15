#ifndef MINIMUM_ENCLOSING_CIRCLE_HPP
#define MINIMUM_ENCLOSING_CIRCLE_HPP

#include <algorithm>
#include <cmath>
#include <random>
#include <vector>

#include "geometry_base.hpp"

// 【随机增量法最小圆覆盖 (minimum_enclosing_circle.hpp)】
// - minimum_enclosing_circle(points): 在期望严格线性 O(n) 时间内计算覆盖平面上所有给定点的最小圆 Circle{center, radius}

inline Circle circumcircle(Point a, Point b, Point c) {
	const Real determinant = 2 * cross(b - a, c - a);
	if (dcmp(determinant) == 0) {
		Circle result(a, 0);
		for (Point p : {b, c})
			if (distance_squared(a, p) > result.radius * result.radius)
				result = Circle((a + p) / 2, distance(a, p) / 2);
		if (distance_squared(b, c) > result.radius * result.radius)
			result = Circle((b + c) / 2, distance(b, c) / 2);
		return result;
	}
	const Real aa = a.length_squared(), bb = b.length_squared(), cc = c.length_squared();
	const Real x = (aa * (b.y - c.y) + bb * (c.y - a.y) + cc * (a.y - b.y)) / determinant;
	const Real y = (aa * (c.x - b.x) + bb * (a.x - c.x) + cc * (b.x - a.x)) / determinant;
	Point center(x, y);
	return Circle(center, distance(center, a));
}

inline bool contains(const Circle& circle, Point point) {
	return distance_squared(circle.center, point) <= circle.radius * circle.radius + EPS;
}

// Expected O(n^3) randomized incremental algorithm.
inline Circle minimum_enclosing_circle(std::vector<Point> points) {
	if (points.empty()) return Circle();
	std::mt19937 random(712367821);
	std::shuffle(points.begin(), points.end(), random);
	Circle result(points[0], 0);
	for (int i = 1; i < (int)points.size(); ++i) {
		if (contains(result, points[i])) continue;
		result = Circle(points[i], 0);
		for (int j = 0; j < i; ++j) {
			if (contains(result, points[j])) continue;
			result = Circle((points[i] + points[j]) / 2, distance(points[i], points[j]) / 2);
			for (int k = 0; k < j; ++k)
				if (!contains(result, points[k]))
					result = circumcircle(points[i], points[j], points[k]);
		}
	}
	return result;
}

#endif
