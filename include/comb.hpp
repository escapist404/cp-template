#ifndef COMB_HPP
#define COMB_HPP

#include <vector>

#include "modint.hpp"

// Binomial coefficients C(n, k) and factorials modulo prime using O(N) precomputation and O(1) queries.

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
