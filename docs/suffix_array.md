# SuffixArray

给定一个序列，按字典序排列所有后缀，并支持查询任意两个后缀的最长公共前缀。

## 构造函数

```cpp
template <typename Sequence, typename Compare = std::less<typename Sequence::value_type>>
SuffixArray(const Sequence &text, Compare compare = Compare{});
```

- `text`：任意**可随机访问**的有序序列，常见为 `std::string` 或 `std::vector<int>`
- `compare`：字符大小的比较器，默认 `std::less`

**值类型要求**
- 只需支持 `Compare` 给出的**严格弱序**（源码用 `!compare(a,b) && !compare(b,a)` 判等）

**约束**
- $0 \le |\mathrm{text}|$；空序列是合法的，此时后续查询接口均不可用
- 源码未对长度开设 `assert`

**复杂度**
- $O(n \log n)$（倍增，每轮计数排序）

## 成员函数

### array

```cpp
const std::vector<int> &array() const;
```

返回**后缀数组** `sa`：按字典序排序后，第 $i$ 小的后缀的起始位置为 `sa[i]`。

**复杂度**
- $O(1)$

### ranks

```cpp
const std::vector<int> &ranks() const;
```

返回**名次数组** `rank`：`rank[i]` 为从位置 $i$ 开始的后缀的名次。恒有 `rank[sa[i]] == i`。

**复杂度**
- $O(1)$

### lcp_array

```cpp
const std::vector<int> &lcp_array() const;
```

返回 **height 数组**：长度为 $n-1$，`lcp_array()[i]` 为排名相邻的两个后缀 `sa[i]` 与 `sa[i+1]` 的最长公共前缀长度。

**复杂度**
- $O(1)$

### lcp

```cpp
int lcp(int i, int j) const;
```

返回从位置 $i$ 与位置 $j$ 开始的两个后缀的最长公共前缀长度。内部用 RMQ 查 height 上的区间最小值。

**约束**
- $0 \le i, j < n$，源码有 `assert`
- `i == j` 时返回 `n - i`（后缀与自身全等）

**复杂度**
- $O(1)$（预处理 $O(n \log n)$）

## 使用示例

```cpp
#include "suffix_array.hpp"
#include <iostream>
#include <string>

int main() {
    std::string s = "banana";
    SuffixArray sa(s);

    // 后缀数组：起始位置按字典序排列
    for (int i : sa.array()) std::cout << i << " ";
    std::cout << '\n';
    // 5 3 1 0 4 2  → a, ana, anana, banana, na, nana

    // height 数组
    for (int i : sa.lcp_array()) std::cout << i << " ";
    std::cout << '\n';
    // 1 3 0 0 2

    // 任意两个后缀的 LCP
    std::cout << sa.lcp(1, 3) << '\n';  // "ana" 与 "anana" → 3
    std::cout << sa.lcp(0, 0) << '\n';  // 自身 → 6

    return 0;
}
```

### 不同子串计数

```cpp
long long count_distinct_substrings(const std::string &s) {
    long long n = (long long)s.size();
    SuffixArray sa(s);
    long long total = n * (n + 1) / 2;
    for (int h : sa.lcp_array()) total -= h;
    return total;
}
```

### 最长重复子串

```cpp
std::string longest_repeated_substring(const std::string &s) {
    SuffixArray sa(s);
    const auto &height = sa.lcp_array();
    int best = 0, pos = 0;
    for (int i = 0; i < (int)height.size(); ++i) {
        if (height[i] > best) {
            best = height[i];
            pos = sa.array()[i];
        }
    }
    return s.substr(pos, best);
}
```

### 整数序列也能用

```cpp
std::vector<int> a = {3, 1, 4, 1, 5};
SuffixArray sa(a);   // 自动做离散化
```

## 前置条件与实现约定

- **接口是方法而非公开成员**。`sa`、`rank`、`height` 是私有的，必须通过 `array()` / `ranks()` / `lcp_array()` 访问。写成 `sa.sa[i]` 无法编译。
- **返回值是常引用**：可以 `const auto &x = sa.array();` 避免拷贝。对象析构前有效。
- 空串构造后 `array()` 为空，`lcp()` 的断言不可能成立，任何查询都是非法的。
- 不做去重 / 附加哨兵字符。若要处理「多串拼接」，需自行插入互不相同的分隔符，并注意分隔符参与排序的事实。
- 比较器中相等用 `!compare(a,b) && !compare(b,a)` 判定，因此 `Compare` 必须是真正的严格弱序（`std::less` 默认满足）。

## 相关算法

- [SuffixAutomaton](suffix_automaton.md) —— 后缀自动机，支持在线追加
- [RollingHash](rolling_hash.md) —— 哈希做法求 LCP，常数更小但可能冲突
- [KMP](kmp.md)、[Z 算法](z_algo.md) —— 单模式 / 前缀匹配
