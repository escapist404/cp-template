#ifndef DIST_HPP
#define DIST_HPP

#include <algorithm>
#include <limits>
#include <vector>
#include "geometry_base.hpp"

// 【二维几何距离与投影 (dist.hpp)】
// - 点到直线垂直投影 project(p, l)、对称点 reflect(p, l)
// - 点到直线距离、点到线段最短距离、两条线段之间的最短距离计算

inline Real closest_pair_distance_squared(std::vector<Point> points) {
    if (points.size() < 2) {
        return std::numeric_limits<Real>::infinity();
    }
    std::sort(points.begin(), points.end(), [](Point a, Point b) {
        if (dcmp(a.x, b.x) != 0) {
            return a.x < b.x;
        }
        return a.y < b.y;
    });
    const auto solve = [&](int l, int r, const auto& self) -> Real {
        if (r - l <= 3) {
            Real result = std::numeric_limits<Real>::infinity();
            for (int i = l; i < r; ++i) {
                for (int j = l; j < i; ++j) {
                    result = std::min(result, distance_squared(points[i], points[j]));
                }
            }
            std::sort(points.begin() + l, points.begin() + r, [](Point a, Point b) {
                return a.y < b.y;
            });
            return result;
        }
        const int mid = (l + r) / 2;
        const Real split_x = points[mid].x;
        Real result = std::min(self(l, mid, self), self(mid, r, self));
        std::vector<Point> merged;
        std::merge(points.begin() + l, points.begin() + mid,
                   points.begin() + mid, points.begin() + r,
                   std::back_inserter(merged), [](Point a, Point b) {
                       return a.y < b.y;
                   });
        std::copy(merged.begin(), merged.end(), points.begin() + l);
        std::vector<Point> strip;
        for (int i = l; i < r; ++i) {
            if ((points[i].x - split_x) * (points[i].x - split_x) >= result) {
                continue;
            }
            for (int j = (int)strip.size() - 1; j >= 0; --j) {
                if ((points[i].y - strip[j].y) * (points[i].y - strip[j].y) >= result) {
                    break;
                }
                result = std::min(result, distance_squared(points[i], strip[j]));
            }
            strip.push_back(points[i]);
        }
        return result;
    };
    return solve(0, points.size(), solve);
}
#endif
