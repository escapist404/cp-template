#ifndef SUFFIX_ARRAY_HPP
#define SUFFIX_ARRAY_HPP

#include <algorithm>
#include <cassert>
#include <functional>
#include <numeric>
#include <vector>

// ============================================================================
// 【倍增法后缀数组、Kasai 高度数组与 O(1) LCP 查询 Suffix Array (suffix_array.hpp)】
// ----------------------------------------------------------------------------
// 类签名：class SuffixArray
//   支持任意可比较序列（std::string 或 std::vector<T>），内部自动完成字符集离散化、O(n log n) 倍增基数排序、
//   O(n) Kasai 算法构建相邻后缀 LCP 数组 height 及 O(n log n) ST 表预处理。
// 构造函数与成员函数签名详解（所有下标均为 0-based）：
//   - template <typename Sequence, typename Compare = std::less<typename Sequence::value_type>>
//     SuffixArray(const Sequence& text, Compare compare = Compare{})
//     功能：构造序列 text（长度为 n）的后缀数组、名次数组、高度数组及 RMQ 表。时空复杂度 O(n log n)。
//   - const std::vector<int>& array() const
//     功能：返回长度为 n 的后缀数组 sa，其中 sa[k] (0 <= k < n) 表示字典序排名第 k 小（0-based）的后缀起始下标。O(1)。
//   - const std::vector<int>& ranks() const
//     功能：返回长度为 n 的名次数组 rank，其中 rank[i] (0 <= i < n) 表示起始于下标 i 的后缀在所有后缀中的 0-based 排名（满足 sa[rank[i]] == i）。O(1)。
//   - const std::vector<int>& lcp_array() const
//     功能：返回长度为 max(0, n - 1) 的高度数组 height，其中 height[k] = LCP(suffix(sa[k]), suffix(sa[k + 1]))。O(1)。
//   - int lcp(int i, int j) const
//     功能：查询原序列中起始于下标 i 与下标 j (0 <= i, j < n) 的两个后缀的最长公共前缀长度：
//           当 i == j 时返回 n - i；当 i != j 时通过 ST 表查询 min_{min(rk[i],rk[j]) <= k < max(rk[i],rk[j])} height[k]。
//     复杂度：单次查询严格 O(1)。
// ============================================================================

class SuffixArray {
    std::vector<int> sa, rank, height, logarithm;
    std::vector<std::vector<int>> rmq;

public:
    // template <typename Sequence, typename Compare = std::less<typename Sequence::value_type>> SuffixArray(const Sequence& text, Compare compare = Compare{}) — 功能：构造序列 text（长度为 n）的后缀数组、名次数组、高度数组及 RMQ 表。时空复杂度 O(n log n)。
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
        // 规范化 rank 数组使其严格成为 sa 的逆排列（涵盖 n = 1 或字符两两不同的边界情形）。
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

    // const std::vector<int>& array() const — 功能：返回长度为 n 的后缀数组 sa，其中 sa[k] (0 <= k < n) 表示字典序排名第 k 小（0-based）的后缀起始下标。O(1)。
    const std::vector<int>& array() const {
        return sa;
    }

    // const std::vector<int>& ranks() const — 功能：返回长度为 n 的名次数组 rank，其中 rank[i] (0 <= i < n) 表示起始于下标 i 的后缀在所有后缀中的 0-based 排名（满足 sa[rank[i]] == i）。O(1)。
    const std::vector<int>& ranks() const {
        return rank;
    }

    // const std::vector<int>& lcp_array() const — 功能：返回长度为 max(0, n - 1) 的高度数组 height，其中 height[k] = LCP(suffix(sa[k]), suffix(sa[k + 1]))。O(1)。
    const std::vector<int>& lcp_array() const {
        return height;
    }

    // int lcp(int i, int j) const — 功能：查询原序列中起始于下标 i 与下标 j (0 <= i, j < n) 的两个后缀的最长公共前缀长度：
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
