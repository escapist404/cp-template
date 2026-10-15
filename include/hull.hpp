#ifndef HULL_HPP
#define HULL_HPP

#include <algorithm>
#include <vector>
#include "geometry_base.hpp"

// ============================================================================
// 【Andrew 单调链算法求二维严格凸包 Convex Hull (hull.hpp)】
// ----------------------------------------------------------------------------
// 函数签名：
//   inline std::vector<Point> convex_hull(std::vector<Point> points)
// 参数与返回值详解：
//   - points：平面点集（内部自动排序并按 dcmp 去重）。
//   - 严格凸包约定：使用 `dcmp(cross(...)) <= 0` 弹栈，【剔除边内部共线点】，仅保留严格极点。
//   - 返回值 std::vector<Point>：按【逆时针 (CCW)】顺序排列的凸包顶点（首点为最左下点，首尾不重复）。O(n log n)。
// ============================================================================

// inline std::vector<Point> convex_hull(std::vector<Point> points) — 功能：参数与返回值详解：
inline std::vector<Point> convex_hull(std::vector<Point> points) {
    std::sort(points.begin(), points.end(), [](Point a, Point b) {
        if (dcmp(a.x, b.x) != 0) {
            return a.x < b.x;
        }
        return a.y < b.y;
    });
    points.erase(std::unique(points.begin(), points.end()), points.end());
    if (points.size() <= 1) {
        return points;
    }
    std::vector<Point> hull;
    for (Point point : points) {
        while (hull.size() >= 2 &&
               dcmp(cross(hull.back() - hull[hull.size() - 2], point - hull.back())) <= 0) {
            hull.pop_back();
        }
        hull.push_back(point);
    }
    const int lower_size = hull.size();
    for (int i = (int)points.size() - 2; i >= 0; --i) {
        while ((int)hull.size() > lower_size &&
               dcmp(cross(hull.back() - hull[hull.size() - 2],
                          points[i] - hull.back())) <= 0) {
            hull.pop_back();
        }
        hull.push_back(points[i]);
    }
    hull.pop_back();
    return hull;
}
#endif
