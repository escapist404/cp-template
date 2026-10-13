#ifndef GEOMETRY_BASE_HPP
#define GEOMETRY_BASE_HPP

#include <cmath>
#include "types.hpp"

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
