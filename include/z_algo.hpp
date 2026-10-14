#ifndef Z_ALGO_HPP
#define Z_ALGO_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

// Z-algorithm: z[i] is the LCP length of s and s[i..n), with z[0] = n. O(n).

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
