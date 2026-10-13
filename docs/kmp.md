# KMP

给定一个模式串和一段文本，找出模式串在文本中的全部起始位置。

## 接口

```cpp
std::vector<int> kmp(const std::string &s, const std::string &t);
```

- `s`：**模式串**
- `t`：**文本串**

**注意参数顺序**：模式串在前，文本串在后。

**返回值**
- 所有满足 `t[p..p+|s|-1] == s` 的 0-indexed 起始位置 `p`，按升序排列，允许重叠
- 若 `s` 为空串，返回空 `vector`（源码开头即 `if (s.empty()) return result;`）

**复杂度**
- $O(|s| + |t|)$，源码未显式注明

## 使用示例

```cpp
#include "kmp.hpp"
#include <iostream>
#include <string>

int main() {
    std::string text = "ababcababa";
    std::string pattern = "aba";

    // 注意：模式串是第一个参数
    std::vector<int> positions = kmp(pattern, text);

    for (int p : positions) {
        std::cout << p << " ";
    }
    std::cout << std::endl;
    // 输出 0 5 7

    return 0;
}
```

### 只判断是否出现

```cpp
bool contains(const std::string &text, const std::string &pattern) {
    return !kmp(pattern, text).empty();
}
```

### 统计出现次数（含重叠）

```cpp
int count_occurrences(const std::string &text, const std::string &pattern) {
    return (int)kmp(pattern, text).size();
}
```

## 前置条件与实现约定

- 本文件**只提供这一个函数**。没有单独的「求 next / fail 数组」接口，也没有 `kmp_match` 之类的别名；`fail` 数组是函数内部的局部 lambda，不对外暴露。
- 交卷时若需要 fail 数组（例如求最小循环节），需要自行复制实现，无法从本模板复用。
- 字符集无限制，按 `std::string` 的 `char` 逐字节比较。

## 相关算法

- [Z 算法](z_algo.md) —— 单次线性扫描得到所有后缀与全串的 LCP
- [Manacher](manacher.md) —— 回文子串
- [RollingHash](rolling_hash.md) —— 哈希做法
- [AhoCorasick](ac_automaton.md) —— 多模式串匹配
- [SuffixArray](suffix_array.md) —— 后缀数组，支持区间 LCP 查询
