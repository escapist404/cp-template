#ifndef WAVELET_TREE_HPP
#define WAVELET_TREE_HPP

#include <algorithm>
#include <cassert>
#include <functional>
#include <vector>

#include "bit.hpp"

// 【小波树 Wavelet Matrix (wavelet_tree.hpp)】
// - 内置自动离散化的静态区间查询结构 WaveletTree<T, Compare>（T 仅需支持严格弱序比较）
// - 建树时间 O(n log n)，空间 O(n log sigma)，单次查询 O(log sigma)，区间均为 0-based 左闭右开 [l, r)
// - kth(l, r, k)           : 返回子区间 [l, r) 内 0-based 第 k 小元素（要求 0 <= k < r - l）
// - count_less(l, r, val)  : 返回子区间 [l, r) 内严格小于 val 的元素个数
// - count(l, r, low, high) : 返回子区间 [l, r) 内落在左闭右开值域 [low, high) 的元素个数（要求 low <= high）

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

    // 查询 [l, r) 内 0-based 第 k 小元素 (0 <= k < r - l)
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

    // 统计 [l, r) 内严格小于 value 的元素个数
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

    // 统计 [l, r) 内落在 [low, high) 范围内的元素个数
    int count(int l, int r, const T& low, const T& high) const {
        assert(!compare(high, low));
        return count_less(l, r, high) - count_less(l, r, low);
    }
};
#endif
