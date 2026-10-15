#ifndef DIVIDE_CONQUER_DP_HPP
#define DIVIDE_CONQUER_DP_HPP

#include <algorithm>
#include <cassert>
#include <vector>

// ============================================================================
// 【决策单调性分治优化 DP Divide and Conquer DP (divide_conquer_dp.hpp)】
// ----------------------------------------------------------------------------
// 函数签名：
//   template <typename T, typename Cost>
//   std::vector<T> divide_conquer_dp(int groups, int n, Cost cost, T infinity)
// 参数与返回值详解：
//   - 求解分层划分 DP：dp[g][i] = min_{g - 1 <= j < i} { dp[g - 1][j] + cost(j, i) }（1 <= g <= groups, 0 <= i <= n），
//     要求代价回调 `T cost(int j, int i)` 使得每层最优决策点关于 i 单调不降。
//   - 返回长度为 n + 1 的第 groups 层最优值数组 dp[groups][0..n]。总时间复杂度 O(groups * n log n)。
// ============================================================================

// template <typename T, typename Cost> std::vector<T> divide_conquer_dp(int groups, int n, Cost cost, T infinity) — 功能：参数与返回值详解：
template <typename T, typename Cost>
std::vector<T> divide_conquer_dp(int groups, int n, Cost cost, T infinity) {
	assert(groups >= 0 && n >= 0);
	std::vector<T> previous(n + 1, infinity), current(n + 1);
	previous[0] = T();
	if (groups > n) return std::vector<T>(n + 1, infinity);
	for (int g = 1; g <= groups; ++g) {
		std::fill(current.begin(), current.end(), infinity);
		auto solve = [&](auto&& self, int left, int right, int opt_left, int opt_right) -> void {
			if (left >= right) return;
			int mid = (left + right) / 2;
			int best = -1;
			int from = std::max(opt_left, g - 1);
			int to = std::min(opt_right, mid - 1);
			for (int j = from; j <= to; ++j) {
				if (previous[j] == infinity) continue;
				T value = previous[j] + cost(j, mid);
				if (best == -1 || value < current[mid]) {
					current[mid] = value;
					best = j;
				}
			}
			self(self, left, mid, opt_left, best == -1 ? opt_right : best);
			self(self, mid + 1, right, best == -1 ? opt_left : best, opt_right);
		};
		solve(solve, g, n + 1, g - 1, n - 1);
		previous.swap(current);
	}
	return previous;
}

#endif
