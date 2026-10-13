#ifndef STIRLING_HPP
#define STIRLING_HPP

#include <cassert>
#include <vector>

// Unsigned first kind and second kind Stirling numbers up to n.
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
