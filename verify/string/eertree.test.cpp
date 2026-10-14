#define PROBLEM "https://judge.yosupo.jp/problem/eertree"

#include "../../include/fastio.hpp"
#include <iostream>
#include <string>

#include "../../include/palindromic_tree.hpp"

int main() {
	OptimizedIO<1 << 20> io;
	std::string s;
	io >> s;
	PalindromicTree<char> tree;
	std::vector<int> last;
	for (int i = 0; i < (int)s.size(); ++i) {
		int node = tree.add(s[i]);
		last.push_back(node - 1);
	}
	auto states = tree.states();
	std::cout << tree.distinct_palindromes() << '\n';
	for (int node = 2; node < (int)states.size(); ++node) {
		int parent = states[node].parent == 0 ? -1 : states[node].parent - 1;
		int suffix = states[node].link == 0 ? 0 : states[node].link - 1;
		std::cout << parent << ' ' << suffix << '\n';
	}
	for (int node : last) std::cout << node << ' ';
	std::cout << '\n';
}
