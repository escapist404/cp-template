# ExtendedKMP

给定一个模式串和一段文本，计算模式串与文本每个后缀的最长公共前缀长度。

## 接口

```cpp
template <typename Sequence>
std::vector<int> exkmp_z(const Sequence& sequence);
template <typename Sequence>
std::vector<int> exkmp(const Sequence& pattern, const Sequence& text);
```

`exkmp_z(s)[i]` 是 `s` 与 `s[i..)` 的 LCP；`exkmp(p,t)[i]` 是 `p` 与 `t[i..)` 的 LCP，均为 0-based。

## 复杂度

时间 $O(n)$，空间 $O(n)$，其中 `n` 是模式与文本长度之和的量级。

## 使用示例

```cpp
#include "exkmp.hpp"
#include <cassert>
#include <string>

int main() {
    auto lcp = exkmp(std::string("aba"), std::string("abacaba"));
    assert((lcp == std::vector<int>{3, 0, 1, 3, 0, 1, 1}));
}
```

## 相关文档

- [KMP](kmp.md) —— 只需要匹配位置时的前缀函数
- [ZAlgorithm](z_algo.md) —— 单个序列的 Z 函数
