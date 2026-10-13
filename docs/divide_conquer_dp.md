# DivideConquerDP

决策单调的分治 DP 优化，返回恰好分成 `groups` 个**非空段**时的最后一层 DP。

## 接口

```cpp
template <typename T, typename Cost>
std::vector<T> divide_conquer_dp(int groups, int n, Cost cost, T infinity);
```

递推为 `dp[g][i] = min(dp[g-1][j] + cost(j,i))`，`g-1 <= j < i`。初始 `dp[0][0]=T()`，其余不可达。

`cost(l,r)` 是 0-based 区间 `[l,r)` 的代价；返回长度 `n+1` 的数组，`result[i]` 为前 `i` 个元素恰好分为 `groups` 段的最小代价。不可达返回 `infinity`。

## 约束与复杂度

- `groups >= 0`、`n >= 0`；若 `groups > n`，全部位置不可达。
- 每层**最左最优决策**必须随 `i` 非递减；常见充分条件是代价满足相应的四边形不等式。算法不会验证这一前提。
- `T()` 为零值，支持加法和大小比较。`infinity` 大于所有合法结果；所有有限代价的加法不能溢出。
- 不可达前驱不会参与加法，允许使用整数类型的最大值作 `infinity`。
- 每层 $O(n\log n)$ 次代价查询，空间 $O(n)$；若代价查询 $O(1)$，总时间 $O(\mathrm{groups}\cdot n\log n)$。

## 使用示例

```cpp
#include "divide_conquer_dp.hpp"
#include <cassert>
#include <limits>

int main() {
    auto cost = [](int l, int r) { return 1LL * (r-l) * (r-l); };
    auto dp = divide_conquer_dp<long long>(2, 4, cost,
                                          std::numeric_limits<long long>::max());
    assert(dp[4] == 8); // 两段长度各为 2
}
```

## 前置条件与实现约定

- 输入任意 `cost` 并不保证正确；必须证明决策单调。
- 区间长度不能为零，因此 `dp[g][i]` 在 `i < g` 时不可达。
- 只返回最后一层的最优值，不返回分段方案。

## 相关文档

- [ConvexHullTrick](convex_hull_trick.md) —— 可转为直线的 DP
- [WQSBinarySearch](wqs_binary_search.md) —— 对段数加罚求解
