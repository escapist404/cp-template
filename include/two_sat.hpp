#ifndef TWO_SAT_HPP
#define TWO_SAT_HPP

#include <algorithm>
#include <cassert>
#include <vector>

#include "scc.hpp"

// 【2-SAT 布尔可满足性 (two_sat.hpp)】
// - 0-based 布尔变量编号 [0, n)，时间与空间复杂度 O(n + m)
// - add_implication(x, xv, y, yv): 添加蕴含约束 (x == xv) => (y == yv) 及其逆否命题
// - add_or(x, xv, y, yv)         : 添加析取子句 (x == xv) V (y == yv)
// - add_true(x, val)             : 强制固定变量 x 取值为 val
// - solve()                      : 判定是否有解；若有解返回 true，之后可通过 assignment()[i] 获取第 i 个变量的 0/1 赋值

class TwoSat {
	int n;
	std::vector<std::vector<int>> graph;
	std::vector<int> component, answer;

public:
	TwoSat(int n) : n(n), graph(2 * n) {}

	// 添加蕴含约束 (x == xv) => (y == yv) 及其逆否命题
	void add_implication(int x, bool xv, int y, bool yv) {
		assert(0 <= x && x < n && 0 <= y && y < n);
		const int a = 2 * x + xv, b = 2 * y + yv;
		graph[a].push_back(b);
		graph[b ^ 1].push_back(a ^ 1);
	}
	void add_or(int x, bool xv, int y, bool yv) {
		add_implication(x, !xv, y, yv);
	}
	void add_true(int x, bool value) {
		add_implication(x, !value, x, value);
	}

	bool solve() {
		const auto components = get_scc(graph);
		component.assign(2 * n, 0);
		for (int id = 0; id < (int)components.size(); ++id)
			for (const int v : components[id]) component[v] = id;
		answer.assign(n, false);
		for (int i = 0; i < n; ++i) {
			if (component[2 * i] == component[2 * i + 1]) {
				return false;
			}
			answer[i] = component[2 * i] > component[2 * i + 1];
		}
		return true;
	}
	// 仅在 solve() 返回 true 后有效
	const std::vector<int>& assignment() const {
		return answer;
	}
};

#endif
