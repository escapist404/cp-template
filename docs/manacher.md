# Manacher

给定一个字符串，计算每个中心向两侧能扩展出的最长回文范围。

## 接口

```cpp
std::vector<int> manacher(const std::string &s);
```

## 返回值（注意）

源码先在 `s` 的**每个字符之间及两端**插入 `'#'` 得到扩展串 `t`：

```
t = "#" + s[0] + "#" + s[1] + "#" + ... + "#"
```

于是 `t.size() == 2n + 1`，偶数下标是 `'#'`，奇数下标是 `s[i]`。

**返回的是在扩展串 `t` 上的回文半径数组 `r`**，长度 `2n + 1`，`r[i]` 为以 `i` 为中心的最大半径（**含中心**）。

- 返回的**不是**原串下标，也**不是**原串中的回文长度
- 源码未说明如何换算回 `s` 的语义

**边界**
- `s` 为空串时 `t == "#"`，返回长度为 1 的 `r`，且 `r[0] == 1`

**约束**
- 无 `assert`、无注释
- **`'#'` 会与输入冲突**：若原串本身含 `'#'`，结果不可靠（源码未说明此限制）
- 字符集无限制

**复杂度**
- $O(n)$，源码未注明

## 使用示例

```cpp
#include "manacher.hpp"
#include <iostream>

int main() {
    std::string s = "abba";
    auto r = manacher(s);

    for (int x : r) std::cout << x << ' ';
    std::cout << '\n';

    // 最长回文半径在扩展串中为 5（中心在中间的 '#'）
    // 对应原串回文长度 = radius - 1 = 4

    return 0;
}
```

### 换算成原串的回文长度与起点

```cpp
// 最长回文子串的长度与起始位置
std::pair<int, int> longest_palindrome(const std::string &s) {
    auto r = manacher(s);
    int best_len = 0, best_start = 0;
    for (int i = 0; i < (int)r.size(); ++i) {
        int len = r[i] - 1;              // 扩展串半径 - 1 = 原串回文长度
        if (len > best_len) {
            best_len = len;
            best_start = (i - r[i] + 1) / 2;   // 换算回原串下标
        }
    }
    return {best_len, best_start};
}
```

### 判断原串是否为回文

```cpp
bool is_palindrome(const std::string &s) {
    return longest_palindrome(s).first == (int)s.size();
}
```

## 前置条件与实现约定

- **返回的是扩展串 `t` 上的数组**，长度是 `2n+1` 而非 `n`。直接拿它当「以 `s[i]` 为中心的回文半径」用是错的。
- 原串含 `'#'` 时结果不可靠：插入的分隔符与真实字符无法区分。
- `r[i]` 含中心，所以「原串回文长度 = r[i] - 1」。
- 奇偶长度回文被统一到扩展串上：奇数下标对应奇回文，偶数下标对应偶回文。

## 相关文档

- [PalindromicTree](palindromic_tree.md) —— 回文树，统计不同回文子串
- [Z 算法](z_algo.md)、[KMP](kmp.md) —— 线性字符串算法
- [RollingHash](rolling_hash.md) —— 哈希判回文
