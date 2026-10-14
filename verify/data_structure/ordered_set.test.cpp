#define PROBLEM "https://judge.yosupo.jp/problem/ordered_set"

#include "../../include/fastio.hpp"
#include <iostream>
#include <limits>

#include "../../include/scapegoat.hpp"

using Set = ScapegoatTree<long long>;

bool contains(const Set& set, long long x) {
	int rank = set.get_rank(x);
	return rank <= set.size() && set.get_value(rank) == x;
}

int count_less_equal(const Set& set, long long x) {
	int rank = set.get_rank(x);
	return rank <= set.size() && set.get_value(rank) == x ? rank : rank - 1;
}

int main() {
	OptimizedIO<1 << 20> io;
	int n, q;
	io >> n >> q;
	Set set;
	for (int i = 0; i < n; ++i) {
		long long x;
		io >> x;
		set.insert(x);
	}
	while (q--) {
		int type;
		long long x;
		io >> type >> x;
		if (type == 0) {
			if (!contains(set, x)) set.insert(x);
		} else if (type == 1) {
			if (contains(set, x)) set.remove(x);
		} else if (type == 2) {
			std::cout << (x <= set.size() ? set.get_value(x) : -1) << '\n';
		} else if (type == 3) {
			std::cout << count_less_equal(set, x) << '\n';
		} else if (type == 4) {
			long long result = set.get_prev(x + 1);
			std::cout << (result == std::numeric_limits<long long>::lowest() ? -1 : result) << '\n';
		} else {
			long long result = set.get_next(x - 1);
			std::cout << (result == std::numeric_limits<long long>::max() ? -1 : result) << '\n';
		}
	}
}
