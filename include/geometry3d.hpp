#ifndef GEOMETRY3D_HPP
#define GEOMETRY3D_HPP

#include <cassert>
#include <cmath>
#include <algorithm>
#include "geometry_base.hpp"

// ============================================================================
// 【三维计算几何：空间向量、三维直线、平面与球体 (geometry3d.hpp)】
// ----------------------------------------------------------------------------
// 1. 三维点与向量结构体签名：struct Vector3 / using Point3 = Vector3;
//    - Vector3(Real x = 0, Real y = 0, Real z = 0), operator+, operator-, operator*(Real), operator/(Real), operator==
//    - Real length_squared() const / Real length() const / Vector3 unit() const
//    - inline Real dot(Vector3 a, Vector3 b) / inline Vector3 cross(Vector3 a, Vector3 b)
//    - inline Real distance_squared(Point3 a, Point3 b) / inline Real distance(Point3 a, Point3 b)
// 2. 三维直线结构体签名：struct Line3 { Point3 start, end; };
//    - Line3() / Line3(Point3 start, Point3 end)
//    - Vector3 direction() const                        ：返回方向向量 end - start
//    - Point3 projection(Point3 point) const            ：返回点 point 在三维直线上的垂足
//    - inline Real distance(Point3 point, const Line3& line)：返回空间点到三维直线的垂直距离
// 3. 三维平面结构体签名：struct Plane3 { Point3 point; Vector3 normal; };（normal 构造时自动单位化）
//    - Plane3() / Plane3(Point3 point, Vector3 normal) / Plane3(Point3 a, Point3 b, Point3 c)
//    - Real signed_distance(Point3 value) const         ：点到平面的有向距离 dot(normal, value - point)
//    - Point3 projection(Point3 value) const            ：点在平面上的正交投影点
//    - inline Real distance(Point3 point, const Plane3& plane)：点到平面的垂直距离
//    - inline bool line_plane_intersection(const Line3& line, const Plane3& plane, Point3& result)
//      求直线与平面的唯一交点并写入 result；若平行或共面则返回 false。
// 4. 球体结构体签名：struct Sphere3 { Point3 center; Real radius; };
// ============================================================================

struct Vector3 {
	Real x, y, z;

	Vector3(Real x = 0, Real y = 0, Real z = 0) : x(x), y(y), z(z) {}
	Vector3 operator+(Vector3 other) const { return {x + other.x, y + other.y, z + other.z}; }
	Vector3 operator-(Vector3 other) const { return {x - other.x, y - other.y, z - other.z}; }
	Vector3 operator*(Real scalar) const { return {x * scalar, y * scalar, z * scalar}; }
	Vector3 operator/(Real scalar) const { return {x / scalar, y / scalar, z / scalar}; }
	bool operator==(Vector3 other) const {
		return dcmp(x, other.x) == 0 && dcmp(y, other.y) == 0 && dcmp(z, other.z) == 0;
	}
	Real length_squared() const { return x * x + y * y + z * z; }
	Real length() const { return std::sqrt(length_squared()); }
	Vector3 unit() const { return *this / length(); }
};

using Point3 = Vector3;

inline Vector3 operator*(Real scalar, Vector3 vector) { return vector * scalar; }
inline Real dot(Vector3 a, Vector3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
inline Vector3 cross(Vector3 a, Vector3 b) {
	return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}
inline Real distance_squared(Point3 a, Point3 b) { return (a - b).length_squared(); }
inline Real distance(Point3 a, Point3 b) { return std::sqrt(distance_squared(a, b)); }

struct Line3 {
	Point3 start, end;
	Line3() = default;
	Line3(Point3 start, Point3 end) : start(start), end(end) {}
	Vector3 direction() const { return end - start; }
	Point3 projection(Point3 point) const {
		const Vector3 d = direction();
		return start + d * (dot(d, point - start) / d.length_squared());
	}
};

struct Plane3 {
	Point3 point;
	Vector3 normal;

	Plane3() = default;
	Plane3(Point3 point, Vector3 normal) : point(point), normal(normal.unit()) {}
	Plane3(Point3 a, Point3 b, Point3 c)
		: point(a), normal(cross(b - a, c - a).unit()) {}
	Real signed_distance(Point3 value) const { return dot(normal, value - point); }
	Point3 projection(Point3 value) const { return value - normal * signed_distance(value); }
};

struct Sphere3 {
	Point3 center;
	Real radius;
	Sphere3() = default;
	Sphere3(Point3 center, Real radius) : center(center), radius(radius) {}
};

inline Real distance(Point3 point, const Line3& line) {
	return cross(point - line.start, line.direction()).length() / line.direction().length();
}

inline Real distance(Point3 point, const Plane3& plane) {
	return std::fabs(plane.signed_distance(point));
}

inline bool line_plane_intersection(const Line3& line, const Plane3& plane, Point3& result) {
	const Real denominator = dot(plane.normal, line.direction());
	if (dcmp(denominator) == 0) return false;
	const Real t = dot(plane.normal, plane.point - line.start) / denominator;
	result = line.start + line.direction() * t;
	return true;
}

#endif
