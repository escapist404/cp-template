# CustomHash (`hash.hpp`)

`hash.hpp` 提供结合 SplitMix64 混淆函数与高精度运行期随机种子的哈希仿函数 `CustomHash`，用于替代 `std::unordered_map` 和 `std::unordered_set` 的默认 `std::hash`，防止在 Codeforces 等包含 Hack 环节的比赛中被针对固定桶模数构造的数据卡到单次 $O(n)$ 退化。

## 接口说明

```cpp
struct CustomHash {
    static std::uint64_t mix(std::uint64_t x);
    static std::uint64_t seed();

    template <class T>
    std::size_t operator()(T value) const;

    template <class A, class B>
    std::size_t operator()(const std::pair<A, B>& value) const;
};
```

- `mix(x)`：标准的 64 位 SplitMix64 整数混淆函数，消除算术级数或低位规律性导致的哈希碰撞。
- `seed()`：在首次调用时读取 `std::chrono::steady_clock::now().time_since_epoch().count()` 作为进程级静态随机种子，使得每次程序运行的哈希分布均不相同。
- `operator()(T value)`：支持任意可显式转换为 `std::uint64_t` 的标量类型（如 `int`、`long long`、`unsigned long long`、枚举或指针）。注意不能直接传入 `std::string`（字符串请使用 [`rolling_hash.hpp`](rolling_hash.md) 或标准哈希组合）。
- `operator()(const std::pair<A, B>& value)`：原生支持 `std::pair<A, B>` 作为哈希表键值（要求 `A` 和 `B` 均可转换为 `std::uint64_t`），无需额外手写 `pair` 哈希函数。

## 复杂度

- 单次哈希计算：时间复杂度 $O(1)$，额外空间复杂度 $O(1)$。

## 使用示例

```cpp
#include "hash.hpp"
#include <cassert>
#include <unordered_map>
#include <unordered_set>
#include <utility>

int main() {
    std::unordered_map<long long, int, CustomHash> count_map;
    count_map[1000000000000LL] = 42;
    assert(count_map[1000000000000LL] == 42);

    std::unordered_set<std::pair<int, int>, CustomHash> visited_edges;
    visited_edges.insert({1, 2});
    assert(visited_edges.count({1, 2}) == 1);
    return 0;
}
```

## 相关文档

- [RollingHash](rolling_hash.md) —— 字符串多项式滚动哈希
- [TreeHash](tree_hash.md) —— 树同构判定的哈希算法
