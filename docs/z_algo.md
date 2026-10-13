# Z Algorithm

给定一个字符串，计算它与每个后缀的最长公共前缀长度。

## 接口

```cpp
std::vector<int> z_algorithm(const std::string &s);
```

**返回值**
- 长度 $n = |s|$ 的数组 `z`
- `z[i]` 为 `s` 与 `s[i..]` 的最长公共前缀长度

**重要**：`z[0] = n`（源码显式赋值），**不是 0**。若按教科书习惯期望 `z[0] = 0`，需自行忽略或改写。

**边界**
- `s` 为空串时返回空 `vector`，`z[0]` 不会被写入

**约束**
- 源码无 `assert`、无注释
- 字符集无限制（`std::string` 的 `char`）

**复杂度**
- $O(n)$，源码未注明

## 使用示例

```cpp
#include "z_algo.hpp"
#include <iostream>

int main() {
    std::string s = "aabcaab";
    auto z = z_algorithm(s);

    for (int x : z) std::cout << x << ' ';
    std::cout << '\n';
    // 7 1 0 0 3 1 0
    // z[0] = 7 = n

    return 0;
}
```

### 模式串匹配

把模式串与文本串用分隔符拼接后求 Z 值，`z` 中等于模式串长度的位置即为匹配点：

```cpp
std::vector<int> z_match(const std::string &text, const std::string &pattern) {
    std::string combined = pattern + '\1' + text;   // \1 需不出现在两串中
    auto z = z_algorithm(combined);
    std::vector<int> result;
    for (int i = (int)pattern.size() + 1; i < (int)combined.size(); ++i) {
        if (z[i] >= (int)pattern.size()) {
            result.push_back(i - (int)pattern.size() - 1);
        }
    }
    return result;
}
```

### 最小循环节

```cpp
int min_period(const std::string &s) {
    auto z = z_algorithm(s);
    int n = s.size();
    for (int p = 1; p <= n; ++p) {
        if (n % p == 0 && z[p] >= n - p) return p;
    }
    return n;
}
```

## 相关文档

- [KMP](kmp.md) —— 单模式串匹配，返回全部出现位置
- [Manacher](manacher.md) —— 回文子串
- [SuffixArray](suffix_array.md) —— 支持区间 LCP 查询
