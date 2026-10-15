#ifndef POLY_HPP
#define POLY_HPP

#include <vector>
#include "inter.hpp"

// 【多边形面积、点定位与凸性判定 (poly.hpp)】
// - 多边形有向面积 polygon_area（逆时针为正）、凸多边形判定
// - 点在多边形内部/边界/外部判定（射线法 / 环绕数法）

inline Real polygon_signed_area(const std::vector<Point>& polygon) {
    Real area_twice = 0;
    for (int i = 0; i < (int)polygon.size(); ++i) {
        area_twice += cross(polygon[i], polygon[(i + 1) % polygon.size()]);
    }
    return area_twice / 2;
}

inline Real polygon_area(const std::vector<Point>& polygon) {
    return std::fabs(polygon_signed_area(polygon));
}

// Includes the boundary. Works for clockwise and counter-clockwise polygons.
inline bool point_in_polygon(const std::vector<Point>& polygon, Point point) {
    bool inside = false;
    for (int i = 0, j = polygon.size() - 1; i < (int)polygon.size(); j = i++) {
        Segment edge(polygon[j], polygon[i]);
        if (on_segment(point, edge)) {
            return true;
        }
        const bool crosses = (polygon[i].y > point.y) != (polygon[j].y > point.y);
        if (crosses) {
            const Real x = (polygon[j].x - polygon[i].x) *
                               (point.y - polygon[i].y) /
                               (polygon[j].y - polygon[i].y) + polygon[i].x;
            if (point.x < x) {
                inside = !inside;
            }
        }
    }
    return inside;
}
#endif
