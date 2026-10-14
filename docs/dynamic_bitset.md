# Dynamic Bitset (`dynamic_bitset.hpp`)

`dynamic_bitset.hpp` 实现按 64 位无符号整数（`uint64_t`）分块压缩的运行期动态长度位集 `DynamicBitset`，支持动态扩容、单比特读写、全集统计（`count` / `any` / `none`）以及 $O(n / 64)$ 的按位与、或、异或运算。依赖 [`bit.hpp`](bit.md)。

## 接口说明

### 构造与大小管理

```cpp
class DynamicBitset {
public:
    DynamicBitset();
    DynamicBitset(std::size_t n, bool value = false);
    std::size_t size() const;
    bool empty() const;
    void resize(std::size_t new_size, bool value = false);
    void push_back(bool value);
};
```

- **索引约定**：所有位下标均为 **`0-based`**。内部始终通过 `trim()` 保证最后一个 64 位块中超出 `size()` 的高位比特恒为 $0$，从而确保 `count()` 和相等比较 `==` 的准确性。
- `DynamicBitset(std::size_t n, bool value = false)`：构造长度为 $n$、初始所有位均为 `value` 的位集。
- `resize(std::size_t new_size, bool value = false)`：将位集长度调整为 `new_size`；若扩容且 `value == true`，新增的高位下标 `[old_size, new_size)` 会被填充为 $1$。
- `push_back(bool value)`：在末尾追加一位 `value`，长度加 $1$。

### 单比特访问与统计

```cpp
void set(std::size_t p, bool value = true);
void reset(std::size_t p);
void reset();
bool test(std::size_t p) const;
bool operator[](std::size_t p) const;
std::size_t count() const;
bool any() const;
bool none() const;
```

- `set(p, value = true)`：将下标 `p` 设为 `value`。**注意若 `p >= size()`，`set` 会自动调用 `resize(p + 1)` 扩容位集**。
- `reset(p)`：若 `p < size()`，将下标 `p` 清零；若 `p >= size()` 则忽略（不会自动扩容）。
- `reset()`：保持当前 `size()` 不变，将所有位全部清零。
- `test(p)` / `operator[](p)`：读取下标 `p` 的布尔值（只读，不返回代理引用，不能写 `bs[p] = true`；若 `p >= size()` 安全返回 `false`）。
- `count()` / `any()` / `none()`：分别返回 $1$ 的总个数、是否至少存在一个 $1$、是否全为 $0$。

### 集合按位运算与比较

```cpp
DynamicBitset& operator|=(const DynamicBitset& other);
DynamicBitset& operator&=(const DynamicBitset& other);
DynamicBitset& operator^=(const DynamicBitset& other);
friend DynamicBitset operator|(DynamicBitset lhs, const DynamicBitset& rhs);
friend DynamicBitset operator&(DynamicBitset lhs, const DynamicBitset& rhs);
friend DynamicBitset operator^(DynamicBitset lhs, const DynamicBitset& rhs);
friend bool operator==(const DynamicBitset& lhs, const DynamicBitset& rhs);
friend bool operator!=(const DynamicBitset& lhs, const DynamicBitset& rhs);
```

- **不同长度位集的运算语义**：
  - 按位或 `|=` 与按位异或 `^=`：结果长度自动扩展为 **`std::max(lhs.size(), rhs.size())`**（较短一方超出部分视为 $0$）；
  - 按位与 `&=`：结果长度自动收缩为 **`std::min(lhs.size(), rhs.size())`**；
  - 相等比较 `==`：仅当两者 `size()` 相同且每一位完全一致时返回 `true`。

## 复杂度

- `size`、`empty`、`test`、`operator[]`、`reset(p)`：$O(1)$。
- `count`、`any`、`none`、`reset()`、`operator|`、`operator&`、`operator^`、`operator==`：时间复杂度均为 $O(n / 64)$。
- 空间复杂度：$O(n / 64)$ 个 64 位字。

## 使用示例

```cpp
#include "dynamic_bitset.hpp"
#include <cassert>

int main() {
    DynamicBitset a(5, false), b(5, false);
    a.set(1);
    a.set(3);
    b.set(3);
    b.set(4);

    DynamicBitset c = a ^ b; // 位 1 和位 4 为 1
    assert(c.count() == 2);
    assert(c.test(1) && !c.test(3) && c[4]);
    return 0;
}
```

## 相关文档

- [Bit Utilities](bit.md) —— 提供底层的 `popcount` 位统计函数
