#ifndef LYNDON_HPP
#define LYNDON_HPP

#include <utility>
#include <vector>

// Return the standard factorization into non-increasing Lyndon words.
// Each pair is a half-open interval [l, r).
template <typename Sequence>
std::vector<std::pair<int, int>> lyndon_factorization(const Sequence& sequence) {
	const int n = sequence.size();
	std::vector<std::pair<int, int>> result;
	for (int i = 0; i < n;) {
		int j = i + 1, k = i;
		while (j < n && sequence[k] <= sequence[j]) {
			if (sequence[k] < sequence[j]) k = i;
			else ++k;
			++j;
		}
		while (i <= k) {
			const int length = j - k;
			result.push_back({i, i + length});
			i += length;
		}
	}
	return result;
}

#endif
