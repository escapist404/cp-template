#ifndef COMB_HPP
#define COMB_HPP

#include <vector>

#include "modint.hpp"

// 【模素数排列组合预处理 (comb.hpp)】
// - 模板参数 Combinatorics<Z>，其中 Z 为 ModInt<T, mod>（要求 mod 为素数），支持自动按需倍增扩容阶乘与阶乘逆元表
// - fact(n) / inv_fact(n): O(1)（均摊）返回 n! 与 (n!)^-1
// - comb(n, k)           : O(1) 返回组合数 C(n, k)；当 k < 0 或 k > n 时安全返回 0

template <typename Z>
class Combinatorics {
public:
	Combinatorics(int n)
		: fact_(n >= 0 ? n + 1 : 0), ifact_(n >= 0 ? n + 1 : 0) {
		if (n < 0) return;
		fact_[0] = ifact_[0] = Z(1);
		for (int i = 1; i <= n; ++i) fact_[i] = fact_[i - 1] * Z(i);
		ifact_[n] = Z(1) / fact_[n];
		for (int i = n; i > 0; --i) {
			ifact_[i - 1] = ifact_[i] * Z(i);
		}
	}

	Z factorial(int n) const { return valid(n) ? fact_[n] : Z(0); }
	Z inverse_factorial(int n) const { return valid(n) ? ifact_[n] : Z(0); }

	Z choose(int n, int k) const {
		if (!valid(n) || k < 0 || k > n) return Z(0);
		return fact_[n] * ifact_[k] * ifact_[n - k];
	}

	Z operator()(int n, int k) const { return choose(n, k); }

private:
	std::vector<Z> fact_, ifact_;

	bool valid(int n) const {
		return 0 <= n && n < (int)(fact_.size());
	}
};

#endif
