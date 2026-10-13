# WQSBinarySearch

给定一个可按选取次数划分的最优化问题，通过给次数加线性罚值，恢复恰好选取指定次数时的最小代价。

## 接口

```cpp
template <typename T, typename Solve>
T wqs_binary_search(i64 low, i64 high, int required, Solve solve);
```

`solve(p)` 返回 `{count, penalized_cost}`，最小化 `original_cost + p*count`。**罚后代价相同必须选择 count 更大的方案**。

二分最后一个 `count >= required` 的整数罚值，返回 `penalized_cost - p*required`。`T` 是代价类型，罚值范围使用 `i64`。

## 约束与复杂度

- `low <= high`，`required >= 0`，指定次数必须可行。
- 原问题的最优代价 `F(k)` 在所考虑的连续整数次数域上具有离散凸性：边际代价 `F(k+1)-F(k)` 非递减。
- `F(k)` 使用整数代价，边际斜率应能由整数罚值界定；`solve` 必须精确求出罚后最优解。
- 需满足 `solve(low).first >= required >= solve(high).first`；次数随罚值非递增。
- 即使最优次数跳过 `required`，凸性也允许通过罚值恢复答案；仅有次数单调不足以保证恢复正确。
- 代价乘法和减法不能溢出。时间 $O(\log(high-low+1)\cdot T_{solve})$；罚值跨度的中点用 `i128`，避免 `i64` 相减溢出。

## 使用示例

```cpp
#include "wqs_binary_search.hpp"
#include <cassert>
#include <vector>

int main() {
    std::vector<i64> cost{2, 2, 7};
    auto solve = [&](i64 p) {
        std::pair<int, i64> result{0, 0};
        for (i64 x : cost) if (x+p <= 0) { // 同价时多选
            ++result.first;
            result.second += x+p;
        }
        return result;
    };
    assert(wqs_binary_search<i64>(-10, 10, 1, solve) == 2);
}
```

## 相关文档

- [DivideConquerDP](divide_conquer_dp.md) —— 分段 DP 优化
- [BinarySearch](binary_search.md) —— 普通二分
