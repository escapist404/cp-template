#ifndef CALIPERS_HPP
#define CALIPERS_HPP

#include <algorithm>
#include <cmath>
#include <vector>
#include "geometry_base.hpp"

// 【旋转卡壳：凸包直径与最小外接矩形 (calipers.hpp)】
// - 要求输入为按【逆时针顺序】排列的凸包顶点序列，时间复杂度 O(n)
// - 计算凸包最远点对（直径）以及面积最小 / 周长最小的外接矩形

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
