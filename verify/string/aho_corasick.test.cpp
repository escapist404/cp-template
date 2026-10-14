#define PROBLEM "https://judge.yosupo.jp/problem/aho_corasick"

#include "../../include/fastio.hpp"
#include <iostream>
#include <string>

#include "../../include/ac_automaton.hpp"

int main() {
	OptimizedIO<1 << 20> io;
	int n;
	io >> n;
	AhoCorasick automaton;
	for (int i = 0; i < n; ++i) {
		std::string s;
		io >> s;
		automaton.insert(s);
	}
	automaton.build();
	auto links = automaton.state_links();
	std::cout << automaton.size() << '\n';
	for (int i = 1; i < automaton.size(); ++i)
		std::cout << links[i].first << ' ' << links[i].second << '\n';
	for (int v : automaton.pattern_vertices()) std::cout << v << ' ';
	std::cout << '\n';
}
