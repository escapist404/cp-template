#ifndef STIRLING_HPP
#define STIRLING_HPP

#include <cassert>
#include <vector>

// ============================================================================
// 【第一类（无符号）与第二类斯特林数递推表 Stirling Numbers (stirling.hpp)】
// ----------------------------------------------------------------------------
// 模板类签名：template <typename Z> class StirlingNumbers
//   通过二维递推在 O(n^2) 时空内同时预处理 0 <= i, j <= n 的无符号第一类斯特林数与第二类斯特林数：
//   - 第一类斯特林数 [n, k]：将 n 个不同元素排成 k 个非空圆排列的方案数，递推式 f[i][j] = f[i-1][j-1] + (i-1) * f[i-1][j]；
//   - 第二类斯特林数 {n, k}：将 n 个不同元素划分为 k 个非空无序集合的方案数，递推式 s[i][j] = s[i-1][j-1] + j * s[i-1][j]。
// 构造函数与成员函数签名详解：
//   - StirlingNumbers(int n)
//     功能：预处理阶数上限为 n (n >= 0) 的第一类与第二类斯特林数表。时空复杂度 O(n^2)。
//   - Z first(int n, int k) const
//     功能：返回无符号第一类斯特林数 [n, k]（要求 0 <= n <= N；若 k < 0 或 k > n 返回 Z(0)）。O(1)。
//   - Z second(int n, int k) const
//     功能：返回第二类斯特林数 {n, k}（要求 0 <= n <= N；若 k < 0 或 k > n 返回 Z(0)）。O(1)。
// ============================================================================

template <typename Z>
class StirlingNumbers {
	std::vector<std::vector<Z>> first_, second_;

public:
	// StirlingNumbers(int n) — 功能：预处理阶数上限为 n (n >= 0) 的第一类与第二类斯特林数表。时空复杂度 O(n^2)。
	StirlingNumbers(int n) : first_(n >= 0 ? n + 1 : 0, std::vector<Z>(n >= 0 ? n + 1 : 0)),
		second_(n >= 0 ? n + 1 : 0, std::vector<Z>(n >= 0 ? n + 1 : 0)) {
		assert(n >= 0);
		first_[0][0] = second_[0][0] = Z(1);
		for (int i = 1; i <= n; ++i) {
			for (int j = 1; j <= i; ++j) {
				first_[i][j] = first_[i - 1][j - 1] + first_[i - 1][j] * Z(i - 1);
				second_[i][j] = second_[i - 1][j - 1] + second_[i - 1][j] * Z(j);
			}
		}
	}

	// Z first(int n, int k) const — 功能：返回无符号第一类斯特林数 [n, k]（要求 0 <= n <= N；若 k < 0 或 k > n 返回 Z(0)）。O(1)。
	Z first(int n, int k) const {
		assert(0 <= n && n < (int)first_.size());
		return 0 <= k && k <= n ? first_[n][k] : Z(0);
	}

	// Z second(int n, int k) const — 功能：返回第二类斯特林数 {n, k}（要求 0 <= n <= N；若 k < 0 或 k > n 返回 Z(0)）。O(1)。
	Z second(int n, int k) const {
		assert(0 <= n && n < (int)second_.size());
		return 0 <= k && k <= n ? second_[n][k] : Z(0);
	}
};

#endif
