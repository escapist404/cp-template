#ifndef CONVEX_HULL_TRICK_HPP
#define CONVEX_HULL_TRICK_HPP

#include <cassert>
#include <vector>

// ============================================================================
// 【单调斜率凸包优化栈 Convex Hull Trick / CHT (convex_hull_trick.hpp)】
// ----------------------------------------------------------------------------
// 模板类签名：template <typename T = long long> class ConvexHullTrick
//   维护直线 y = slope * x + intercept 的下包络线（求最小值），内部用 long double 防相乘溢出。
// 成员函数签名详解：
//   - void clear() / bool empty() const / int size() const
//   - void add_line(T slope, T intercept)
//     【前置条件】插入直线的斜率 slope 必须【单调不降】（slope >= hull.back().slope），均摊 O(1)。
//   - T query(T x) const
//     二分查询横坐标 x 处的最小函数值 min_i (slope_i * x + intercept_i)（要求 !empty()）。O(log n)。
// ============================================================================

template <typename T = long long>
class ConvexHullTrick {
	struct Line {
		T slope, intercept;
	};

	std::vector<Line> hull;

	static long double value(const Line& line, T x) {
		return (long double)line.slope * x + line.intercept;
	}

	static bool bad(const Line& a, const Line& b, const Line& c) {
		return ((long double)a.intercept - b.intercept) * ((long double)c.slope - b.slope) <=
			((long double)b.intercept - c.intercept) * ((long double)b.slope - a.slope);
	}

public:
	void clear() { hull.clear(); }
	bool empty() const { return hull.empty(); }
	int size() const { return hull.size(); }

	void add_line(T slope, T intercept) {
		if (!hull.empty()) {
			assert(slope >= hull.back().slope);
			if (slope == hull.back().slope) {
				if (intercept >= hull.back().intercept)
					return;
				hull.pop_back();
			}
		}
		Line line{slope, intercept};
		while (hull.size() >= 2 && bad(hull[hull.size() - 2], hull.back(), line))
			hull.pop_back();
		hull.push_back(line);
	}

	T query(T x) const {
		assert(!hull.empty());
		int left = 0, right = hull.size() - 1;
		while (left < right) {
			int middle = (left + right) / 2;
			if (value(hull[middle], x) <= value(hull[middle + 1], x))
				right = middle;
			else
				left = middle + 1;
		}
		return hull[left].slope * x + hull[left].intercept;
	}
};

#endif
