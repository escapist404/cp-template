#define PROBLEM "https://judge.yosupo.jp/problem/longest_common_substring"

#include "../../include/fastio.hpp"
#include <iostream>
#include <string>

#include "../../include/suffix_automaton.hpp"

int main() {
	OptimizedIO<1 << 20> io;
	std::string s, t;
	io >> s >> t;
	SuffixAutomaton<char> automaton;
	automaton.build(s);
	auto states = automaton.states();
	int node = 0, length = 0, best = 0, end_s = 0, end_t = 0;
	for (int i = 0; i < (int)t.size(); ++i) {
		while (node && !states[node].next.count(t[i])) {
			node = states[node].link;
			length = states[node].length;
		}
		if (states[node].next.count(t[i])) {
			node = states[node].next[t[i]];
			++length;
		} else length = 0;
		if (length > best) {
			best = length;
			end_s = states[node].first_position + 1;
			end_t = i + 1;
		}
	}
	std::cout << end_s - best << ' ' << end_s << ' '
			<< end_t - best << ' ' << end_t << '\n';
}
