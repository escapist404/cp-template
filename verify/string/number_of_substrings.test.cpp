#define PROBLEM "https://judge.yosupo.jp/problem/number_of_substrings"

#include "../../include/fastio.hpp"
#include <iostream>
#include <string>

#include "../../include/suffix_automaton.hpp"

int main() {
	OptimizedIO<1 << 20> io;
	std::string s;
	io >> s;
	SuffixAutomaton<char> automaton;
	automaton.build(s);
	std::cout << automaton.distinct_substrings() << '\n';
}
