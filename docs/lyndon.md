# Lyndon

将一个序列唯一分解为按字典序非递增排列的 Lyndon 串。

## 接口

```cpp
template <typename Sequence>
std::vector<std::pair<int, int>> lyndon_factorization(const Sequence& sequence);
```

返回若干个半开区间 `[l,r)`。按区间取出的字符串是 Lyndon 词，并且因子按字典序非递增排列。

## 复杂度

时间 $O(n)$，空间 $O(k)$，其中 `k` 是因子数量。

## 使用示例

```cpp
#include "lyndon.hpp"
#include <cassert>
#include <string>

int main() {
    std::string s = "ababbab";
    auto factors = lyndon_factorization(s);
    std::string rebuilt;
    for (auto [l, r] : factors) rebuilt += s.substr(l, r - l);
    assert(rebuilt == s);
}
```
