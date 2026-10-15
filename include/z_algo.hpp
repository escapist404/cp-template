#ifndef Z_ALGO_HPP
#define Z_ALGO_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

// ============================================================================
// 【Z 算法 / 扩展 KMP 自匹配 Z-Algorithm (z_algo.hpp)】
// ----------------------------------------------------------------------------
// 函数签名：
//   std::vector<int> z_algorithm(const std::string& s)
// 参数与返回值详解：
//   - s：输入字符串（长度为 n）。
//   - 返回值 std::vector<int>：长度为 n 的 Z 数组（LCP 数组），其中：
//     - 对 1 <= i < n，z[i] 表示原串 s 与后缀 s[i .. n) 的最长公共前缀 (LCP) 长度；
//     - 【注意约定】本实现中固定 z[0] == 0（而非 n）。
// 复杂度：维护最右匹配区间 (j, j + z[j])，时间与空间复杂度均为严格线性 O(n)。
// ============================================================================

// std::vector<int> z_algorithm(const std::string& s) — 功能：参数与返回值详解：
std::vector<int> z_algorithm(const std::string& s) {
    int n = s.size();
    std::vector<int> z(n);
    if (n == 0) return z;
    z[0] = n;
    for (int i = 1, j = 1; i < n; i++) {
        z[i] = std::max(0, std::min(j + z[j] - i, z[i - j]));
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
            z[i]++;
        }
        if (i + z[i] > j + z[j]) {
            j = i;
        }
    }
    return z;
}

#endif
