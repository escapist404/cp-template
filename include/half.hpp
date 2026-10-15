#ifndef HALF_HPP
#define HALF_HPP

#include <algorithm>
#include <cassert>
#include <cmath>
#include <vector>
#include "inter.hpp"

// ============================================================================
// 【半平面交 S&I 算法 Half-Plane Intersection (half.hpp)】
// ----------------------------------------------------------------------------
// 函数签名：
//   inline std::vector<Point> half_plane_intersection(std::vector<Line> lines)
// 参数与返回值详解：
//   - 每条有向直线 Line(start, end) 表示其【左侧半平面】（满足 cross(end - start, p - start) >= 0 的点集）。
//   - 若半平面交可能无界，调用前需手动添加四条包围盒边界有向直线。
//   - 返回值：核区域退化或为空集时返回 {}；否则按【逆时针顺序】返回核凸多边形的顶点列表。O(n log n)。
// ============================================================================

// inline std::vector<Point> half_plane_intersection(std::vector<Line> lines) — 功能：参数与返回值详解：
inline std::vector<Point> half_plane_intersection(std::vector<Line> lines) {
    const auto angle = [](const Line& line) {
        return std::atan2(line.end.y - line.start.y, line.end.x - line.start.x);
    };
    std::sort(lines.begin(), lines.end(), [&](const Line& a, const Line& b) {
        const Real aa = angle(a);
        const Real ab = angle(b);
        if (dcmp(aa, ab) != 0) {
            return aa < ab;
        }
        return cross(a.end - a.start, b.start - a.start) > 0;
    });
    std::vector<Line> unique;
    for (const Line& line : lines) {
        if (unique.empty() || dcmp(cross(line.end - line.start,
                                         unique.back().end - unique.back().start)) != 0) {
            unique.push_back(line);
        } else if (cross(line.end - line.start, unique.back().start - line.start) > 0) {
            unique.back() = line;
        }
    }
    const auto outside = [](const Line& line, Point point) {
        return dcmp(cross(line.end - line.start, point - line.start)) < 0;
    };
    std::vector<Line> deque;
    for (const Line& line : unique) {
        while (deque.size() > 1 && outside(line,
               line_intersection(deque[deque.size() - 1], deque[deque.size() - 2]))) {
            deque.pop_back();
        }
        while (deque.size() > 1 && outside(line,
               line_intersection(deque[0], deque[1]))) {
            deque.erase(deque.begin());
        }
        if (!deque.empty() && dcmp(cross(line.end - line.start,
                                         deque.back().end - deque.back().start)) == 0) {
            if (outside(line, deque.back().start)) {
                deque.pop_back();
            } else {
                continue;
            }
        }
        deque.push_back(line);
    }
    while (deque.size() > 2 && outside(deque.front(),
           line_intersection(deque.back(), deque[deque.size() - 2]))) {
        deque.pop_back();
    }
    while (deque.size() > 2 && outside(deque.back(),
           line_intersection(deque[0], deque[1]))) {
        deque.erase(deque.begin());
    }
    if (deque.size() < 3) {
        return {};
    }
    std::vector<Point> polygon;
    for (int i = 0; i < (int)deque.size(); ++i) {
        polygon.push_back(line_intersection(deque[i], deque[(i + 1) % deque.size()]));
    }
    return polygon;
}
#endif
