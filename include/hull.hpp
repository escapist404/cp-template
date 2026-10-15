#ifndef HULL_HPP
#define HULL_HPP

#include <algorithm>
#include <vector>
#include "geometry_base.hpp"

// 【二维凸包 Andrew 单调链算法 (hull.hpp)】
// - convex_hull(points, strict): O(n log n) 按逆时针顺序返回凸包顶点序列（起点为字典序最小点）
// - strict 参数控制共线点处理：strict = true 时严格剔除边内部共线点，strict = false 时保留边界上的共线点

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
