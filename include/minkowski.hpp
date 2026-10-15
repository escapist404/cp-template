#ifndef MINKOWSKI_HPP
#define MINKOWSKI_HPP

#include <algorithm>
#include <vector>
#include <utility>
#include "hull.hpp"

// ============================================================================
// 【二维凸包闵可夫斯基和 Minkowski Sum of Convex Polygons (minkowski.hpp)】
// ----------------------------------------------------------------------------
// 函数签名：
//   inline std::vector<Point> minkowski_sum(std::vector<Point> a, std::vector<Point> b)
// 参数与返回值详解：
//   - 输入两个平面点集 a, b（内部自动先调用 convex_hull 提取严格逆时针凸包），
//     归并边向量后返回闵可夫斯基和 { p + q | p in a, q in b } 的逆时针严格凸包顶点列表。O((n + m) log(n + m))。
// ============================================================================

// inline std::vector<Point> minkowski_sum(std::vector<Point> a, std::vector<Point> b) — 功能：参数与返回值详解：
inline std::vector<Point> minkowski_sum(std::vector<Point> a,
                                        std::vector<Point> b) {
    a = convex_hull(std::move(a));
    b = convex_hull(std::move(b));
    if (a.empty() || b.empty()) {
        return {};
    }
    if (a.size() == 1) {
        for (Point& point : b) point = point + a[0];
        return b;
    }
    if (b.size() == 1) {
        for (Point& point : a) point = point + b[0];
        return a;
    }

    auto lowest = [](Point lhs, Point rhs) {
        if (dcmp(lhs.y, rhs.y) != 0) return lhs.y < rhs.y;
        return lhs.x < rhs.x;
    };
    const auto rotate_to_lowest = [&](std::vector<Point>& polygon) {
        const auto it = std::min_element(polygon.begin(), polygon.end(), lowest);
        std::rotate(polygon.begin(), it, polygon.end());
    };
    rotate_to_lowest(a);
    rotate_to_lowest(b);

    const int n = (int)a.size(), m = (int)b.size();
    std::vector<Point> result;
    result.reserve(n + m);
    result.push_back(a[0] + b[0]);
    int i = 0, j = 0;
    while (i < n || j < m) {
        const Vector ea = i < n ? a[(i + 1) % n] - a[i] : Vector();
        const Vector eb = j < m ? b[(j + 1) % m] - b[j] : Vector();
        if (j == m || (i < n && dcmp(cross(ea, eb)) > 0)) {
            result.push_back(result.back() + ea);
            ++i;
        } else if (i == n || dcmp(cross(ea, eb)) < 0) {
            result.push_back(result.back() + eb);
            ++j;
        } else {
            result.push_back(result.back() + ea + eb);
            ++i;
            ++j;
        }
    }
    result.pop_back();
    return convex_hull(std::move(result));
}

#endif
