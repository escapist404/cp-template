#ifndef COMB_HPP
#define COMB_HPP

#include <vector>

#include "modint.hpp"

// ============================================================================
// 【线性预处理阶乘逆元与 O(1) 组合数查询 Combinatorics (comb.hpp)】
// ----------------------------------------------------------------------------
// 模板类签名：template <typename Z> class Combinatorics
//   - Z：模整数类型（如 ModInt<int, 998244353>），要求上界 n < Z::mod() 且模数为素数以保证 n! 可逆。
// 构造函数与成员函数签名详解：
//   - Combinatorics(int n)
//     功能：线性递推预处理 [0, n] 范围内的阶乘表 fact_ 与阶乘逆元表 ifact_。
//     复杂度：时间复杂度 O(n + log(mod))，空间复杂度 O(n)。
//   - Z factorial(int n) const
//     功能：返回 n! (0 <= n <= N)；若 n < 0 或超出预处理上界，安全返回 Z(0)。时间复杂度 O(1)。
//   - Z inverse_factorial(int n) const
//     功能：返回 (n!)^(-1) (0 <= n <= N)；若 n < 0 或超出预处理上界，安全返回 Z(0)。时间复杂度 O(1)。
//   - Z choose(int n, int k) const
//   - Z operator()(int n, int k) const
//     功能：计算二项式系数 C(n, k) = n! / (k! * (n - k)!)；若 n 超出预处理范围或 k < 0 或 k > n，安全返回 Z(0)。O(1)。
// ============================================================================

template <typename Z>
class Combinatorics {
public:
	// Combinatorics(int n) — 功能：线性递推预处理 [0, n] 范围内的阶乘表 fact_ 与阶乘逆元表 ifact_。
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

	// Z operator()(int n, int k) const — 功能：计算二项式系数 C(n, k) = n! / (k! * (n - k)!)；若 n 超出预处理范围或 k < 0 或 k > n，安全返回 Z(0)。O(1)。
	Z operator()(int n, int k) const { return choose(n, k); }

private:
	std::vector<Z> fact_, ifact_;

	bool valid(int n) const {
		return 0 <= n && n < (int)(fact_.size());
	}
};

#endif
