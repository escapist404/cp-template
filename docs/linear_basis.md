# LinearBasis

`LinearBasis` 维护 `int64_t` 的异或线性基。

## 接口

```cpp
class LinearBasis;
void insert(std::int64_t value);        // 插入，线性相关时不改变基
bool contains(std::int64_t value) const;
std::int64_t max_xor(std::int64_t value = 0) const;
```

实现固定维护第 `0..62` 位，因此应使用非负的 63 位整数。`max_xor(x)` 返回当前基中与 `x` 异或后的最大值。

## 使用示例

```cpp
#include "linear_basis.hpp"
#include <iostream>

int main() {
    LinearBasis basis;
    basis.insert(3);
    basis.insert(5);
    std::cout << basis.contains(6) << '\n';
    std::cout << basis.max_xor() << '\n';
}
```
