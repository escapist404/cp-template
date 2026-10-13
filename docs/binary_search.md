# BinarySearch / TernarySearch

提供整数边界二分、实数二分和实数三分。

## 接口

```cpp
template <class T, class Predicate>
T first_true(T low, T high, Predicate predicate); // 搜索 [low, high)

template <class T, class Predicate>
T last_true(T low, T high, Predicate predicate);  // 搜索 [low, high)

template <class T, class Predicate>
T real_binary_search(T low, T high, Predicate predicate, int iterations = 80);

template <typename Function>
Real real_ternary_search_max(Real low, Real high, Function function,
                             int iterations = 80);
template <typename Function>
Real real_ternary_search_min(Real low, Real high, Function function,
                             int iterations = 80);
```

`first_true` 要求谓词先假后真，返回第一个真值位置；若不存在，返回 `high`。`last_true` 要求谓词先真后假，返回最后一个真值位置；若不存在，返回 `low - 1`。实数二分要求谓词在区间内由假变真，返回近似分界点。

## 使用示例

```cpp
#include "binary_search.hpp"
#include "ternary_search.hpp"
#include "types.hpp"
#include <iostream>

int main() {
    std::cout << first_true<i64>(0, 11, [](i64 x) { return x >= 5; }) << '\n'; // 5
    std::cout << last_true<i64>(0, 11, [](i64 x) { return x <= 7; }) << '\n';  // 7
    std::cout << real_binary_search(0.0, 2.0,
        [](double x) { return x * x >= 2.0; }) << '\n';
    std::cout << real_ternary_search_max(0.0, 10.0,
        [](Real x) { return -(x - 3) * (x - 3) + 5; }) << '\n';
}
```

## 约束与复杂度

- 整数区间使用半开区间 `[low, high)`，并要求 `low <= high`。
- 三分要求目标函数在区间内单峰；实数搜索默认迭代 80 次。
- 整数和实数二分复杂度分别为 $O(\log n)$ 和 $O(\text{iterations})$。

## 相关文档

- [ParallelBinarySearch](parallel_binary_search.md) —— 并行二分
- [OverallBinarySearch](overall_binary_search.md) —— 整体二分
