#ifndef STIRLING_HPP
#define STIRLING_HPP

#include <cassert>
#include <vector>

// 【第一类与第二类斯特林数 (stirling.hpp)】
// - 依赖正式幂级数与 NTT 卷积，在 O(n log n) 时间内计算一行或固定列的斯特林数（模 NTT 素数）：
// - stirling_first_row(n)      : 返回无符号第一类斯特林数第 n 行 [s(n, 0), ..., s(n, n)]
// - stirling_second_row(n)     : 返回第二类斯特林数第 n 行 [S(n, 0), ..., S(n, n)]
// - stirling_first_column(n, k) / stirling_second_column(n, k): 返回固定列 k、行从 0 到 n 的斯特林数

template <typename Z>
class StirlingNumbers {
	std::vector<std::vector<Z>> first_, second_;

public:
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

	Z first(int n, int k) const {
		assert(0 <= n && n < (int)first_.size());
		return 0 <= k && k <= n ? first_[n][k] : Z(0);
	}

	Z second(int n, int k) const {
		assert(0 <= n && n < (int)second_.size());
		return 0 <= k && k <= n ? second_[n][k] : Z(0);
	}
};

#endif
