#ifndef POLY_HPP
#define POLY_HPP

#include <vector>
#include "inter.hpp"

// ============================================================================
// 【多边形有向面积、面积与点在多边形内判定 Polygon (poly.hpp)】
// ----------------------------------------------------------------------------
// 函数签名与接口详解：
//   1. inline Real polygon_signed_area(const std::vector<Point>& polygon)
//      计算简单多边形有向面积（逆时针为正，顺时针为负）。O(n)。
//   2. inline Real polygon_area(const std::vector<Point>& polygon)
//      返回简单多边形的面积绝对值。O(n)。
//   3. inline bool point_in_polygon(const std::vector<Point>& polygon, Point point)
//      射线法判断点 point 是否在简单多边形（凸或凹、顺时针或逆时针均可）的【内部或边界上】。O(n)。
// ============================================================================

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

// 多边形面积两倍：支持顺时针或逆时针顶点序。
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
