#ifndef GEOMETRY_BASE_HPP
#define GEOMETRY_BASE_HPP

#include <cmath>
#include "types.hpp"

// ============================================================================
// 【二维计算几何基础：常数、点、向量、直线、线段与圆 (geometry_base.hpp)】
// ----------------------------------------------------------------------------
// 1. 精度常数与三态比较函数签名：
//    - inline constexpr Real PI = 3.141592653589793238462643383279502884;
//    - inline constexpr Real EPS = 1E-8;
//    - inline int dcmp(Real a, Real b = 0)  // |a - b| < EPS 返回 0；a < b 返回 -1；a > b 返回 1
// 2. 二维点与向量结构体签名：struct Vector / using Point = Vector;
//    - Vector(Real x = 0, Real y = 0)
//    - Vector operator+(Vector) const / operator-(Vector) const / operator*(Real) const / operator/(Real) const / operator==(Vector) const
//    - Real length_squared() const / Real length() const：模长平方 / 欧氏模长
//    - Vector unit() const                              ：返回单位方向向量 *this / length()
//    - Vector normal() const                            ：返回逆时针旋转 90 度后的单位法向量 Vector(-y, x).unit()
//    - Vector rotated(Real angle) const                 ：返回逆时针旋转弧度 angle 后的向量
// 3. 全局向量函数签名：
//    - inline Real dot(Vector a, Vector b)              ：点积 a.x * b.x + a.y * b.y
//    - inline Real cross(Vector a, Vector b)            ：叉积 a.x * b.y - a.y * b.x
//    - inline Real distance_squared(Point a, Point b) / inline Real distance(Point a, Point b)
//    - inline Real angle(Vector a, Vector b)            ：两向量夹角弧度 in [0, PI]
// 4. 直线与线段结构体签名：struct Line / using Segment = Line;
//    - Line() / Line(Point p1, Point p2) / Line(Point point, Real angle) / Line(Real a, Real b, Real c)（一般式 ax + by + c = 0）
//    - Point projection(Point point) const              ：点在直线上的正交投影点（垂足）
//    - Point reflection(Point point) const              ：点关于直线的镜像对称点
//    - inline Real distance(Point point, const Line& line)：点到直线的垂直距离
// 5. 圆结构体签名：struct Circle { Point center; Real radius; };
//    - Circle() / Circle(Point center, Real radius) / Circle(Real x, Real y, Real radius)
// ============================================================================

inline constexpr Real PI = 3.141592653589793238462643383279502884;
inline constexpr Real EPS = 1E-8;

inline int dcmp(Real a, Real b = 0) {
    if (std::fabs(a - b) < EPS) {
        return 0;
    }
    return a < b ? -1 : 1;
}

struct Vector {
    Real x, y;

    Vector(Real x = 0, Real y = 0) : x(x), y(y) {}

    Vector operator+(Vector other) const {
        return {x + other.x, y + other.y};
    }

    Vector operator-(Vector other) const {
        return {x - other.x, y - other.y};
    }

    Vector operator*(Real scalar) const {
        return {x * scalar, y * scalar};
    }

    Vector operator/(Real scalar) const {
        return {x / scalar, y / scalar};
    }

    bool operator==(Vector other) const {
        return dcmp(x, other.x) == 0 && dcmp(y, other.y) == 0;
    }

    Real length_squared() const {
        return x * x + y * y;
    }

    Real length() const {
        return std::sqrt(length_squared());
    }

    Vector unit() const {
        return *this / length();
    }

    Vector normal() const {
        return Vector(-y, x).unit();
    }

    Vector rotated(Real angle) const {
        return {x * std::cos(angle) - y * std::sin(angle),
                x * std::sin(angle) + y * std::cos(angle)};
    }
};

inline Vector operator*(Real scalar, Vector vector) {
    return vector * scalar;
}

using Point = Vector;

inline Real dot(Vector a, Vector b) {
    return a.x * b.x + a.y * b.y;
}

inline Real cross(Vector a, Vector b) {
    return a.x * b.y - a.y * b.x;
}

inline Real distance_squared(Point a, Point b) {
    return (a - b).length_squared();
}

inline Real distance(Point a, Point b) {
    return std::sqrt(distance_squared(a, b));
}

inline Real angle(Vector a, Vector b) {
    return std::acos(dot(a, b) / a.length() / b.length());
}

struct Line {
    Point start, end;

    Line() = default;

    Line(Point p1, Point p2) : start(p1), end(p2) {}

    Line(Point point, Real angle) : start(point) {
        if (dcmp(angle, PI / 2) == 0) {
            end = start + Point(0, 1);
        } else {
            end = start + Point(1, std::tan(angle));
        }
    }

    // ax + by + c = 0.
    Line(Real a, Real b, Real c) {
        if (dcmp(a) == 0) {
            start = {0, -c / b};
            end = {1, -c / b};
        } else if (dcmp(b) == 0) {
            start = {-c / a, 0};
            end = {-c / a, 1};
        } else {
            start = {0, -c / b};
            end = {1, -(c + a) / b};
        }
    }

    Point projection(Point point) const {
        const Vector direction = end - start;
        return start + direction * (dot(direction, point - start) / direction.length_squared());
    }

    Point reflection(Point point) const {
        const Point projected = projection(point);
        return projected * 2 - point;
    }
};

using Segment = Line;

inline Real distance(Point point, const Line& line) {
    return std::fabs(cross(point - line.start, line.end - line.start)) /
           distance(line.start, line.end);
}

struct Circle {
    Point center;
    Real radius;

    Circle() = default;
    Circle(Point center, Real radius) : center(center), radius(radius) {}
    Circle(Real x, Real y, Real radius) : center(x, y), radius(radius) {}
};


#endif
