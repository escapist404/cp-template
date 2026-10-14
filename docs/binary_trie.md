# Binary Trie (`binary_trie.hpp`)

`binary_trie.hpp` 实现固定位宽的 01 字典树（多重集合）`BinaryTrie<T, Bits>`，支持插入非负整数、删除已存在元素、统计元素出现次数，以及查询给定值与集合中某元素异或的最大值与最小值。

## 接口说明

```cpp
template <typename T = std::uint64_t, int Bits = sizeof(T) * 8>
class BinaryTrie {
public:
    int size();
    void insert(T value);
    void erase(T value);
    int count(T value);
    T max_xor(T value);
    T min_xor(T value);
};
```

- **模板参数与类型约束**：
  - `T`：**必须为无符号整数类型**（如默认的 `std::uint64_t` 或 `std::uint32_t`，传入有符号类型如 `int` 或 `long long` 会触发 `static_assert(std::is_unsigned<T>::value)` 编译错误）。
  - `Bits`：从高到低拆分的二进制位数，默认为 `sizeof(T) * 8`（如 `uint64_t` 为 64 位，`uint32_t` 为 32 位）。若手动指定较小的 `Bits`（如 30），则所有传入的 `value` 必须满足 `value < (1ULL << Bits)`（即 `(value >> Bits) == 0`，否则触发 `assert`）。
- `size()`：返回当前多重集合中存储的元素总数（含重复元素）。
- `insert(T value)`：向多重集合中插入一个 `value`，沿二进制高到低路径将经过节点的 `count` 加 $1$。
- `erase(T value)`：从多重集合中删除**一个** `value`。**调用前必须保证 `count(value) > 0`**（删除不存在的元素会触发 `assert(count(value) > 0)`）。
- `count(T value)`：返回 `value` 在当前多重集合中的出现次数。
- `max_xor(T value)`：返回 $\max_{x \in S} (x \oplus \text{value})$（注意返回值是**异或后的最大结果本身**，而非集合中的原元素 $x$；若需原元素 $x$，用返回值再异或一次 `value` 即可）。**调用前必须保证集合非空 `size() > 0`**。
- `min_xor(T value)`：返回 $\min_{x \in S} (x \oplus \text{value})$（同样返回**异或后的最小结果本身**，且要求 **`size() > 0`**）。

## 复杂度

- `size`：$O(1)$。
- 单次 `insert`、`erase`、`count`、`max_xor`、`min_xor`：时间复杂度均为 $O(\text{Bits})$。
- 空间复杂度：$O(q_{\text{insert}} \cdot \text{Bits})$。

## 使用示例

```cpp
#include "binary_trie.hpp"
#include <cassert>
#include <cstdint>

int main() {
    BinaryTrie<std::uint32_t, 30> bt;
    bt.insert(3);
    bt.insert(5);
    bt.insert(5);

    assert(bt.size() == 3);
    assert(bt.count(5) == 2);
    assert(bt.max_xor(6) == (3u ^ 6u)); // 3 ^ 6 = 5, 5 ^ 6 = 3 -> max = 5
    assert(bt.min_xor(6) == (5u ^ 6u)); // min = 3

    bt.erase(5);
    assert(bt.count(5) == 1);
    return 0;
}
```

## 相关文档

- [LinearBasis](linear_basis.md) —— 维护任意子集异或最大值的线性基
- [Trie](trie.md) —— 小写字母字符串字典树
