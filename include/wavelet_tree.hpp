#ifndef WAVELET_TREE_HPP
#define WAVELET_TREE_HPP

#include <algorithm>
#include <cassert>
#include <functional>
#include <vector>

#include "bit.hpp"

// Static sequence. T needs only a strict weak ordering, not bit operations.
// O(log sigma) kth/count_less/count queries, O(n log sigma) storage.
template <typename T, typename Compare = std::less<T>>
class WaveletTree {
    int n, levels = 0;
    Compare compare;
    std::vector<T> alphabet;
    std::vector<std::vector<int>> zeros;
    std::vector<int> middle;

public:
    WaveletTree(const std::vector<T>& values, Compare compare = Compare{})
        : n(values.size()), compare(compare), alphabet(values) {
        std::sort(alphabet.begin(), alphabet.end(), compare);
        alphabet.erase(std::unique(alphabet.begin(), alphabet.end(), [&](const T& a, const T& b) {
            return !compare(a, b) && !compare(b, a);
        }), alphabet.end());
		levels = ceil_log2(alphabet.size());
        std::vector<int> ranks;
        for (const auto& value : values) {
            ranks.push_back(std::lower_bound(alphabet.begin(), alphabet.end(), value, compare) - alphabet.begin());
        }
        for (int bit = levels - 1; bit >= 0; --bit) {
            std::vector<int> prefix(n + 1), left, right;
            for (int i = 0; i < n; ++i) {
                bool zero = !(ranks[i] >> bit & 1);
                prefix[i + 1] = prefix[i] + zero;
                if (zero) {
                    left.push_back(ranks[i]);
                } else {
                    right.push_back(ranks[i]);
                }
            }
            middle.push_back(left.size());
            zeros.push_back(std::move(prefix));
            left.insert(left.end(), right.begin(), right.end());
            ranks = std::move(left);
        }
    }

    // Zero-based k in [0,r-l).
    T kth(int l, int r, int k) const {
        assert(0 <= l && l < r && r <= n && 0 <= k && k < r - l);
        int rank = 0;
        for (int depth = 0; depth < levels; ++depth) {
            int a = zeros[depth][l], b = zeros[depth][r];
            if (k < b - a) {
                l = a;
                r = b;
            } else {
                k -= b - a;
                rank |= 1 << (levels - 1 - depth);
                l = middle[depth] + l - a;
                r = middle[depth] + r - b;
            }
        }
        return alphabet[rank];
    }

    // Count values at positions in [l, r) that are less than value.
    int count_less(int l, int r, const T& value) const {
        assert(0 <= l && l <= r && r <= n);
        int rank = std::lower_bound(alphabet.begin(), alphabet.end(), value, compare) - alphabet.begin();
        if (rank == (int)alphabet.size()) {
            return r - l;
        }
        int result = 0;
        for (int depth = 0; depth < levels; ++depth) {
            int a = zeros[depth][l], b = zeros[depth][r];
            if (rank >> (levels - 1 - depth) & 1) {
                result += b - a;
                l = middle[depth] + l - a;
                r = middle[depth] + r - b;
            } else {
                l = a;
                r = b;
            }
        }
        return result;
    }

    // Count values at positions in [l, r) that are in [low, high).
    int count(int l, int r, const T& low, const T& high) const {
        assert(!compare(high, low));
        return count_less(l, r, high) - count_less(l, r, low);
    }
};
#endif
