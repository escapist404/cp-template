# StirlingNumbers

预处理第一类、第二类斯特林数。

## 接口

```cpp
template <typename Z>
class StirlingNumbers;
StirlingNumbers(int n);
Z first(int n, int k);   // 无符号第一类 c(n,k)
Z second(int n, int k);  // 第二类 S(n,k)
```

递推为：

```text
c(n,k) = c(n-1,k-1) + (n-1)c(n-1,k)
S(n,k) = S(n-1,k-1) + kS(n-1,k)
```

下标越界但 `0 <= n <= 预处理上界` 时返回 `Z(0)`；`n` 超过预处理上界会触发断言。

## 复杂度

预处理时间、空间 $O(n^2)$；每次查询 $O(1)$。

## 使用示例

```cpp
#include "stirling.hpp"
#include "modint.hpp"
#include <cassert>

int main() {
    using Z = ModInt<int, 998244353>;
    StirlingNumbers<Z> s(10);
    assert(s.first(4, 2) == Z(11));
    assert(s.second(4, 2) == Z(7));
}
```

## 相关文档

- [Comb](comb.md) —— 阶乘与组合数
- [Lagrange](lagrange.md) —— 多项式插值
