# DynamicBitset

动态位集，支持运行期调整大小、位访问和按位运算。

依赖 `bit.hpp`。

## 构造与大小

```cpp
DynamicBitset();
DynamicBitset(std::size_t n, bool value = false);
std::size_t size() const;
bool empty() const;
void resize(std::size_t n, bool value = false);
void push_back(bool value);
```

构造函数创建 `n` 位的位集，初值由 `value` 决定。`resize` 保留原有位；扩展出的位使用 `value` 初始化。

## 位访问

```cpp
void set(std::size_t pos, bool value = true);
void reset(std::size_t pos);
void reset();
bool test(std::size_t pos) const;
bool operator[](std::size_t pos) const;
```

`set` 访问越过当前末尾的位置时会自动扩容到 `pos + 1`。`reset` 和 `test` 对越界位置分别视为无操作和 `false`。

## 统计

```cpp
std::size_t count() const;
bool any() const;
bool none() const;
```

`count` 返回 1 的个数，复杂度为 $O(n / 64)$。

## 位运算

```cpp
DynamicBitset& operator|=(const DynamicBitset& rhs);
DynamicBitset& operator&=(const DynamicBitset& rhs);
DynamicBitset& operator^=(const DynamicBitset& rhs);
DynamicBitset operator|(const DynamicBitset& rhs) const;
DynamicBitset operator&(const DynamicBitset& rhs) const;
DynamicBitset operator^(const DynamicBitset& rhs) const;
bool operator==(const DynamicBitset& rhs) const;
bool operator!=(const DynamicBitset& rhs) const;
```

`|=` 和 `^=` 会把结果扩展到两个操作数的较大长度；`&=` 会把结果缩短到较小长度。不存在取反和移位运算。

## 使用示例

```cpp
#include "dynamic_bitset.hpp"
#include <iostream>

int main() {
    DynamicBitset bits(4);
    bits.set(1);
    bits.set(6);                         // 自动扩展到 7 位

    std::cout << bits.size() << '\n';    // 7
    std::cout << bits.count() << '\n';   // 2
    std::cout << bits[6] << '\n';        // 1

    bits.reset(1);
    DynamicBitset other(7);
    other.set(0);
    auto merged = bits | other;
    std::cout << merged.count() << '\n'; // 2
}
```

## 相关文档

- [LinearBasis](linear_basis.md) —— 线性基（异或线性空间）
- [ODT](old_driver_tree.md) —— 珂朵莉树（区间推平）
