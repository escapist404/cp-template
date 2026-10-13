# Min25

`Min25` 计算满足特定素数值形式的积性函数前缀和。

## 接口

```cpp
class Min25;
Min25(i64 n);
i64 prime_count() const;
i64 prime_sum() const;
template <typename PrimePower>
i64 sum(i64 a, i64 b, const PrimePower& prime_power) const;
```

`prime_count()` 返回不超过 `n` 的素数个数，`prime_sum()` 返回素数和。`sum(a,b,prime_power)` 计算满足 `f(p)=a*p+b`、`f(p^e)=prime_power(p,e)` 且 `f(1)=1` 的积性函数前缀和。

## 使用示例

```cpp
#include "min25.hpp"
#include <iostream>

int main() {
    Min25 sieve(1000000000000LL);
    std::cout << sieve.prime_count() << '\n';
    std::cout << sieve.prime_sum() << '\n';
    // 计算 f(p)=1, f(p^e)=0 (e>=2) 的前缀和：
    std::cout << sieve.sum(0, 1, [](i64, int e, i64) {
        return e == 1 ? 1 : 0;
    }) << '\n';
}
```

注意：当前 `prime_power` 的实际签名为 `f(p, e, power)`，其中 `power=p^e`。`f(p)` 必须是 `a*p+b` 的形式；所有结果必须能表示为 `i64`。
