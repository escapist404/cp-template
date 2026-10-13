#define PROBLEM "https://judge.yosupo.jp/problem/set_xor_min"

#include <iostream>
#include <set>

#include "../../include/binary_trie.hpp"

int main() {
	int q;
	std::cin >> q;
	BinaryTrie<unsigned, 30> trie;
	std::set<unsigned> values;
	while (q--) {
		int type;
		unsigned x;
		std::cin >> type >> x;
		if (type == 0) {
			if (values.insert(x).second) trie.insert(x);
		} else if (type == 1) {
			if (values.erase(x)) trie.erase(x);
		} else {
			std::cout << trie.min_xor(x) << '\n';
		}
	}
}
