#ifndef Z_ALGO_HPP
#define Z_ALGO_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

// 【Z 算法 / 扩展 KMP 自匹配 (z_algo.hpp)】
// - z_algorithm(s): O(n) 返回长度为 n 的数组 z，其中 z[i] 表示 s 与后缀 s[i..n) 的最长公共前缀 (LCP) 长度
// - 约定: z[0] = n（空串返回空向量）

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
