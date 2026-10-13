#ifndef SUFFIX_ARRAY_HPP
#define SUFFIX_ARRAY_HPP

#include <algorithm>
#include <cassert>
#include <functional>
#include <numeric>
#include <vector>

// O(n log n) doubling with counting sort. Supports strings and ordered sequences.
class SuffixArray {
    std::vector<int> sa, rank, height, logarithm;
    std::vector<std::vector<int>> rmq;

public:
    template <typename Sequence, typename Compare = std::less<typename Sequence::value_type>>
    SuffixArray(const Sequence& text, Compare compare = Compare{}) {
        int n = text.size();
        sa.resize(n);
        rank.resize(n);
        if (!n) {
            return;
        }
        std::vector<typename Sequence::value_type> alphabet(text.begin(), text.end());
        std::sort(alphabet.begin(), alphabet.end(), compare);
        alphabet.erase(std::unique(alphabet.begin(), alphabet.end(), [&](const auto& a, const auto& b) {
            return !compare(a, b) && !compare(b, a);
        }), alphabet.end());
        for (int i = 0; i < n; ++i) {
            rank[i] = std::lower_bound(alphabet.begin(), alphabet.end(), text[i], compare) - alphabet.begin();
        }
        int classes = alphabet.size();
        std::vector<int> count(std::max(n, classes)), next(n);
        for (int x : rank) {
            ++count[x];
        }
        std::partial_sum(count.begin(), count.end(), count.begin());
        for (int i = n - 1; i >= 0; --i) {
            sa[--count[rank[i]]] = i;
        }
        for (int width = 1; classes < n; width *= 2) {
            std::vector<int> second;
            for (int i = std::max(0, n - width); i < n; ++i) {
                second.push_back(i);
            }
            for (int p : sa) {
                if (p >= width) {
                    second.push_back(p - width);
                }
            }
            count.assign(classes, 0);
            for (int p : second) {
                ++count[rank[p]];
            }
            std::partial_sum(count.begin(), count.end(), count.begin());
            for (int i = n - 1; i >= 0; --i) {
                int p = second[i];
                sa[--count[rank[p]]] = p;
            }
            int new_classes = 1;
            next[sa[0]] = 0;
            for (int i = 1; i < n; ++i) {
                int a = sa[i - 1], b = sa[i];
                int ar = a + width < n ? rank[a + width] : -1;
                int br = b + width < n ? rank[b + width] : -1;
                if (rank[a] != rank[b] || ar != br) {
                    ++new_classes;
                }
                next[b] = new_classes - 1;
            }
            rank.swap(next);
            classes = new_classes;
        }
        // Make rank the inverse of sa, including the n=1/distinct-symbol case.
        for (int i = 0; i < n; ++i) {
            rank[sa[i]] = i;
        }
        height.resize(n - 1);
        int common = 0;
        for (int i = 0; i < n; ++i) {
            if (rank[i] == n - 1) {
                common = 0;
                continue;
            }
            int j = sa[rank[i] + 1];
            while (i + common < n && j + common < n &&
                   !compare(text[i + common], text[j + common]) &&
                   !compare(text[j + common], text[i + common])) {
                ++common;
            }
            height[rank[i]] = common;
            if (common) {
                --common;
            }
        }
        logarithm.resize(n + 1);
        for (int i = 2; i <= n; ++i) {
            logarithm[i] = logarithm[i / 2] + 1;
        }
        if (n > 1) {
            rmq.push_back(height);
            for (int k = 1; (1 << k) <= n - 1; ++k) {
                std::vector<int> row(n - (1 << k));
                for (int i = 0; i < (int)row.size(); ++i) {
                    row[i] = std::min(rmq[k - 1][i], rmq[k - 1][i + (1 << (k - 1))]);
                }
                rmq.push_back(std::move(row));
            }
        }
    }

    const std::vector<int>& array() const {
        return sa;
    }

    const std::vector<int>& ranks() const {
        return rank;
    }

    const std::vector<int>& lcp_array() const {
        return height;
    }

    int lcp(int i, int j) const {
        int n = sa.size();
        assert(0 <= i && i < n && 0 <= j && j < n);
        if (i == j) {
            return n - i;
        }
        int a = rank[i], b = rank[j];
        if (a > b) {
            std::swap(a, b);
        }
        int k = logarithm[b - a];
        return std::min(rmq[k][a], rmq[k][b - (1 << k)]);
    }
};
#endif
