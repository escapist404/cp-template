#ifndef KM_HPP
#define KM_HPP

#include <cassert>
#include <limits>
#include <utility>
#include <vector>

// ============================================================================
// 【Kuhn-Munkres (KM / 匈牙利算法) 二分图最大权完备匹配 (km.hpp)】
// ----------------------------------------------------------------------------
// 结果结构体签名：
//   template <typename T> struct KuhnMunkresResult {
//       T weight{};              // 最大权匹配的总权值
//       std::vector<int> left;   // 长度为 n，left[i] 为左部点 i 匹配的右部点编号 in [0, m)
//       std::vector<int> right;  // 长度为 m，right[j] 为右部点 j 匹配的左部点编号（未匹配为 -1）
//   };
// 求解函数签名：
//   template <typename T>
//   KuhnMunkresResult<T> kuhn_munkres(const std::vector<std::vector<T>>& weight)
// 参数与前置条件详解：
//   - weight：n 行 m 列的完全二分图边权矩阵，weight[i][j] 表示左部点 i (0 <= i < n) 与右部点 j (0 <= j < m) 的边权。
//   - 【前置条件】必须满足 n <= m（以保证左部每个顶点都能匹配到一个互不相同的右部顶点）；T 需为有符号算术类型。
//   - 若某些边不存在，可根据题目将对应 weight[i][j] 设为充分小的负权（但绝对值勿超过 numeric_limits<T>::max() / 8 以防溢出）。
// 复杂度：基于势能与最短路交替树增广，时间复杂度严格 O(n^2 m)，空间复杂度 O(nm)。
// ============================================================================

template <typename T>
struct KuhnMunkresResult {
	T weight{};
	std::vector<int> left, right;
};

// Kuhn-Munkres (匈牙利/KM) 二分图最大权完美匹配：T 需为有符号算术类型且权值和不溢出。
template <typename T>
KuhnMunkresResult<T> kuhn_munkres(const std::vector<std::vector<T>>& weight) {
	const int n = weight.size();
	const int m = n ? weight[0].size() : 0;
	assert(n <= m);
	for (const auto& row : weight) assert((int)row.size() == m);
	const T infinity = std::numeric_limits<T>::max() / 4;
	std::vector<T> u(n + 1), v(m + 1);
	std::vector<int> match(m + 1), previous(m + 1);
	for (int i = 1; i <= n; ++i) {
		match[0] = i;
		int y = 0;
		std::vector<T> slack(m + 1, infinity);
		std::vector<char> used(m + 1);
		do {
			used[y] = true;
			const int x = match[y];
			int next = 0;
			T delta = infinity;
			for (int j = 1; j <= m; ++j) if (!used[j]) {
				const T cost = -weight[x - 1][j - 1] - u[x] - v[j];
				if (cost < slack[j]) {
					slack[j] = cost;
					previous[j] = y;
				}
				if (slack[j] < delta) {
					delta = slack[j];
					next = j;
				}
			}
			for (int j = 0; j <= m; ++j) {
				if (used[j]) {
					u[match[j]] += delta;
					v[j] -= delta;
				} else {
					slack[j] -= delta;
				}
			}
			y = next;
		} while (match[y]);
		do {
			const int next = previous[y];
			match[y] = match[next];
			y = next;
		} while (y);
	}
	KuhnMunkresResult<T> result;
	result.left.assign(n, -1);
	result.right.assign(m, -1);
	for (int j = 1; j <= m; ++j) if (match[j]) {
		const int x = match[j] - 1;
		result.left[x] = j - 1;
		result.right[j - 1] = x;
		result.weight += weight[x][j - 1];
	}
	return result;
}

#endif
