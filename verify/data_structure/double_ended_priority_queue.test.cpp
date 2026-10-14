#define PROBLEM "https://judge.yosupo.jp/problem/double_ended_priority_queue"

#include <iostream>
#include <set>

#include "../../include/fastio.hpp"

int main() {
	OptimizedIO<1 << 20> io;
	int n, q;
	io >> n >> q;
	std::multiset<long long> values;
	while (n--) {
		long long x;
		io >> x;
		values.insert(x);
	}
	while (q--) {
		int type;
		io >> type;
		if (type == 0) {
			long long x;
			io >> x;
			values.insert(x);
		} else if (type == 1) {
			auto it = values.begin();
			std::cout << *it << '\n';
			values.erase(it);
		} else {
			auto it = std::prev(values.end());
			std::cout << *it << '\n';
			values.erase(it);
		}
	}
}
