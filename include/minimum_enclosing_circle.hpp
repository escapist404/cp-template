#ifndef MINIMUM_ENCLOSING_CIRCLE_HPP
#define MINIMUM_ENCLOSING_CIRCLE_HPP

#include <algorithm>
#include <cmath>
#include <random>
#include <vector>

#include "geometry_base.hpp"

// ============================================================================
// 【随机增量法求最小圆覆盖 Minimum Enclosing Circle (minimum_enclosing_circle.hpp)】
// ----------------------------------------------------------------------------
// 函数签名与接口详解：
//   1. inline Circle circumcircle(Point a, Point b, Point c)
//      求平面三点 a, b, c 的外接圆（共线退化时返回最远两点为直径的圆）。O(1)。
//   2. inline bool contains(const Circle& circle, Point point)
//      判断点 point 是否在圆 circle 内部或圆周上（含 EPS 容差）。O(1)。
//   3. inline Circle minimum_enclosing_circle(std::vector<Point> points)
//      随机打乱后运行 Welzl 随机增量法，返回覆盖所有点的最小圆 Circle{center, radius}。期望时间复杂度严格 O(n)。
// ============================================================================

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

// 随机增量法求最小覆盖圆：打乱点序后期望时间复杂度 O(n)，最坏情况 O(n^3)。
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
