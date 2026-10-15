#ifndef CALIPERS_HPP
#define CALIPERS_HPP

#include <algorithm>
#include <cmath>
#include <vector>
#include "geometry_base.hpp"

// ============================================================================
// 【旋转卡壳求凸多边形直径平方 Rotating Calipers (calipers.hpp)】
// ----------------------------------------------------------------------------
// 函数签名：
//   inline Real convex_polygon_diameter_squared(const std::vector<Point>& polygon)
// 参数与返回值详解：
//   - polygon：按【逆时针顺序】排列的凸多边形顶点列表（可直接传入 convex_hull 的返回值）。
//   - 返回凸多边形上最远点对（凸包直径）的欧氏距离平方（点数 < 2 返回 0）。时间复杂度严格线性 O(n)。
// ============================================================================

// inline Real convex_polygon_diameter_squared(const std::vector<Point>& polygon) — 功能：参数与返回值详解：
inline Real convex_polygon_diameter_squared(const std::vector<Point>& polygon) {
    const int n = polygon.size();
    if (n < 2) {
        return 0;
    }
    Real result = 0;
    int opposite = 1;
    for (int i = 0; i < n; ++i) {
        const int next = (i + 1) % n;
        const Vector edge = polygon[next] - polygon[i];
        while (std::fabs(cross(edge, polygon[(opposite + 1) % n] - polygon[i])) >
               std::fabs(cross(edge, polygon[opposite] - polygon[i])) + EPS) {
            opposite = (opposite + 1) % n;
        }
        result = std::max(result, distance_squared(polygon[i], polygon[opposite]));
        result = std::max(result, distance_squared(polygon[next], polygon[opposite]));
    }
    return result;
}
#endif
