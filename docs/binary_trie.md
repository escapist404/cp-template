# BinaryTrie

维护一个无符号整数多重集，支持插入、删除、计数，以及寻找与给定值异或结果的最小值或最大值。

## 接口

```cpp
template <typename T = std::uint64_t, int Bits = sizeof(T) * 8>
class BinaryTrie;
int size();
void insert(T value);
void erase(T value);
int count(T value);
T max_xor(T value);
T min_xor(T value);
```

返回值 `max_xor/min_xor` 是集合元素与参数的**异或结果**，不是元素本身。重复插入会增加计数，`erase` 删除一份。

## 约束与复杂度

- `T` 为无符号整数类型；`1 <= Bits <= sizeof(T)*8`。
- 当 `Bits` 小于类型宽度时，所有存入、查询的值需在 `[0,2^Bits)`。
- `erase` 要求值存在；异或最值查询要求集合非空。
- 单次操作 $O(\mathrm{Bits})$；空间 $O(1+I\cdot\mathrm{Bits})$，`I` 为插入次数，节点不回收。

## 使用示例

```cpp
#include "binary_trie.hpp"
#include <cassert>

int main() {
    BinaryTrie<unsigned, 30> trie;
    trie.insert(1);
    trie.insert(5);
    assert(trie.max_xor(2) == 7);
    assert(trie.min_xor(2) == 3);
    trie.erase(5);
    assert(trie.count(5) == 0 && trie.size() == 1);
}
```

## 相关文档

- [Trie](trie.md) —— 字符串字典树
- [LinearBasis](linear_basis.md) —— 子集异或线性空间
