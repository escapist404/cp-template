#ifndef CONVEX_HULL_TRICK_HPP
#define CONVEX_HULL_TRICK_HPP

#include <cassert>
#include <vector>

// Minimum queries with slopes inserted in nondecreasing order.
// For maximum queries, negate both slope and intercept and negate the answer.
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
