#ifndef TWO_SAT_HPP
#define TWO_SAT_HPP

#include <algorithm>
#include <cassert>
#include <vector>

#include "scc.hpp"

// ============================================================================
// 【2-SAT 布尔可满足性求解器 (two_sat.hpp)】
// ----------------------------------------------------------------------------
// 类签名：class TwoSat
//   基于 Tarjan 强连通分量在 O(n + m) 时间内求解 n 个布尔变量的合取范式可满足性问题，变量编号 0-based [0, n)。
// 构造函数与成员函数签名详解：
//   - TwoSat(int n)
//     功能：初始化包含 n (n >= 0) 个布尔变量 [0, n) 的 2-SAT 求解器（内部构建 2n 个文字节点）。O(n)。
//   - void add_implication(int x, bool xv, int y, bool yv)
//     功能：添加蕴含约束 (x == xv) => (y == yv)，同时自动添加其逆否命题 (y == !yv) => (x == !xv)。O(1)。
//   - void add_or(int x, bool xv, int y, bool yv)
//     功能：添加析取子句 (x == xv) V (y == yv)（两个条件至少成立一个）。O(1)。
//   - void add_true(int x, bool value)
//     功能：添加单元子句，强制指定变量 x 必须取布尔值 value。O(1)。
//   - bool solve()
//     功能：运行 Tarjan SCC 判定是否存在合法布尔赋值；有解返回 true 并生成赋值表，无解返回 false。O(n + m)。
//   - const std::vector<int>& assignment() const
//     功能：【仅在 solve() 返回 true 后有效】返回长度为 n 的 0/1 赋值数组，assignment()[i] 表示变量 i 的取值。O(1)。
// ============================================================================

class TwoSat {
	int n;
	std::vector<std::vector<int>> graph;
	std::vector<int> component, answer;

public:
	// TwoSat(int n) — 功能：初始化包含 n (n >= 0) 个布尔变量 [0, n) 的 2-SAT 求解器（内部构建 2n 个文字节点）。O(n)。
	TwoSat(int n) : n(n), graph(2 * n) {}

	// 添加蕴含约束 (x == xv) => (y == yv) 及其逆否命题
	void add_implication(int x, bool xv, int y, bool yv) {
		assert(0 <= x && x < n && 0 <= y && y < n);
		const int a = 2 * x + xv, b = 2 * y + yv;
		graph[a].push_back(b);
		graph[b ^ 1].push_back(a ^ 1);
	}
	// void add_or(int x, bool xv, int y, bool yv) — 功能：添加析取子句 (x == xv) V (y == yv)（两个条件至少成立一个）。O(1)。
	void add_or(int x, bool xv, int y, bool yv) {
		add_implication(x, !xv, y, yv);
	}
	// void add_true(int x, bool value) — 功能：添加单元子句，强制指定变量 x 必须取布尔值 value。O(1)。
	void add_true(int x, bool value) {
		add_implication(x, !value, x, value);
	}

	// bool solve() — 功能：运行 Tarjan SCC 判定是否存在合法布尔赋值；有解返回 true 并生成赋值表，无解返回 false。O(n + m)。
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
