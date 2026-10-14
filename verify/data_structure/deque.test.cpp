#define PROBLEM "https://judge.yosupo.jp/problem/deque"

#include <deque>
#include <iostream>

#include "../../include/fastio.hpp"

int main() {
	OptimizedIO<1 << 20> io;
	int q;
	io >> q;
	std::deque<long long> sequence;
	while (q--) {
		int type;
		io >> type;
		if (type == 0) {
			long long x;
			io >> x;
			sequence.push_front(x);
		} else if (type == 1) {
			long long x;
			io >> x;
			sequence.push_back(x);
		} else if (type == 2) {
			sequence.pop_front();
		} else if (type == 3) {
			sequence.pop_back();
		} else {
			int i;
			io >> i;
			std::cout << sequence[i] << '\n';
		}
	}
}
