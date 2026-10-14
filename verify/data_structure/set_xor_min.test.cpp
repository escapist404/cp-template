#define PROBLEM "https://judge.yosupo.jp/problem/set_xor_min"

#include "../../include/fastio.hpp"
#include <iostream>
#include <set>

#include "../../include/binary_trie.hpp"

int main() {
	OptimizedIO<1 << 20> io;
	int q;
	io >> q;
	BinaryTrie<unsigned, 30> trie;
	std::set<unsigned> values;
	while (q--) {
		int type;
		unsigned x;
		io >> type >> x;
		if (type == 0) {
			if (values.insert(x).second) trie.insert(x);
		} else if (type == 1) {
			if (values.erase(x)) trie.erase(x);
		} else {
			std::cout << trie.min_xor(x) << '\n';
		}
	}
}
