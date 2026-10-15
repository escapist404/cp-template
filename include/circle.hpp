#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include <algorithm>
#include <cmath>
#include <vector>
#include "inter.hpp"

// 【圆的几何性质、切线与圆交集 (circle.hpp)】
// - 三点外接圆 circumcircle、三角形内切圆 incircle
// - 直线与圆交点、两圆交点、过圆外一点求圆的切线、两圆公切线及两圆相交面积

inline std::vector<Point> line_circle_intersection(const Line& line, const Circle& circle) {
    const Point foot = line.projection(circle.center);
    const Real distance_to_line = distance(foot, circle.center);
    if (distance_to_line > circle.radius + EPS) {
        return {};
    }
    if (dcmp(distance_to_line, circle.radius) == 0) {
        return {foot};
    }
    const Real height = std::sqrt(std::max<Real>(0,
        circle.radius * circle.radius - distance_to_line * distance_to_line));
    const Vector unit = (line.end - line.start).unit();
    return {foot + unit * height, foot - unit * height};
}

inline std::vector<Point> circle_circle_intersection(const Circle& a, const Circle& b) {
    const Vector delta = b.center - a.center;
    const Real distance_between_centers = delta.length();
    if (dcmp(distance_between_centers) == 0 ||
        distance_between_centers > a.radius + b.radius + EPS ||
        distance_between_centers < std::fabs(a.radius - b.radius) - EPS) {
        return {};
    }
    const Real x = (a.radius * a.radius - b.radius * b.radius +
                    distance_between_centers * distance_between_centers) /
                   (2 * distance_between_centers);
    const Real height_squared = a.radius * a.radius - x * x;
    const Point foot = a.center + delta * (x / distance_between_centers);
    if (dcmp(height_squared) == 0) {
        return {foot};
    }
    const Vector normal = delta.normal();
    const Real height = std::sqrt(std::max<Real>(0, height_squared));
    return {foot + normal * height, foot - normal * height};
}


#endif
