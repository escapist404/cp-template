#define PROBLEM "https://judge.yosupo.jp/problem/number_of_substrings"

#include <iostream>
#include <string>

#include "../../include/suffix_automaton.hpp"

int main() {
	std::string s;
	std::cin >> s;
	SuffixAutomaton<char> automaton;
	automaton.build(s);
	std::cout << automaton.distinct_substrings() << '\n';
}
