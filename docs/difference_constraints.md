# Difference Constraints (`difference_constraints.hpp`)

`difference_constraints.hpp` 使用基于队列的 Bellman-Ford（SPFA）算法求解形如 $x_v - x_u \le w$ 的差分约束系统 `difference_constraints`，自动检测负权环并给出可行解。

## 接口说明

```cpp
template <typename T = long long>
struct DifferenceConstraint {
    int u, v;
    T weight;
};

template <typename T = long long>
std::pair<bool, std::vector<T>> difference_constraints(
    int n, const std::vector<DifferenceConstraint<T>>& edges);
```

- **不等式方向与转化规则**：
  - 所有变量编号为 **`0-based`**，范围 $0 \le u, v < n$（越界会触发 `assert`）。
  - 一条边 **`DifferenceConstraint<T>{u, v, w}`（即有向边 $u \xrightarrow{w} v$）表示约束 $x_v \le x_u + w$（等价于 $x_v - x_u \le w$）**。
  - 其他常见约束的等价转化：
    - $x_v - x_u \ge w \iff x_u - x_v \le -w$：添加边 `{v, u, -w}`；
    - $x_v - x_u == w$：同时添加 `{u, v, w}` 与 `{v, u, -w}`。
- **返回值**：返回 `std::pair<bool, std::vector<T>>`（可用 `auto [ok, x] = difference_constraints(n, edges);` 接收）：
  - 若不等式组有解（对应的约束图中不存在负环），返回 `{true, x}`，其中 `x` 是长度为 $n$ 的一组可行解（初始从全 $0$ 超级源状态松弛得到，因此所有 $x_i \le 0$；对所有 $x_i$ 同时加上任意常数 $C$ 仍满足全部差分约束）；
  - 若存在矛盾（即约束图中存在负权环，某个点被松弛达到 $n$ 次），返回 `{false, {}}`。

## 复杂度

- 时间复杂度：最坏 $O(nm)$，其中 $n$ 为变量个数，$m = \text{edges.size()}$ 为约束条数。
- 空间复杂度：$O(n + m)$。

## 使用示例

```cpp
#include "difference_constraints.hpp"
#include <cassert>
#include <vector>

int main() {
    // 约束: x[1] - x[0] <= 3, x[2] - x[1] <= 2, x[0] - x[2] <= -4 (即 x[2] - x[0] >= 4)
    std::vector<DifferenceConstraint<long long>> edges = {
        {0, 1, 3},
        {1, 2, 2},
        {2, 0, -4}
    };
    auto [ok, x] = difference_constraints(3, edges);
    assert(ok);
    assert(x[1] - x[0] <= 3);
    assert(x[2] - x[1] <= 2);
    assert(x[0] - x[2] <= -4);

    // 矛盾约束: 再加一条 x[2] - x[0] <= 3 -> 与 x[2] - x[0] >= 4 矛盾
    edges.push_back({0, 2, 3});
    auto [ok2, _] = difference_constraints(3, edges);
    assert(!ok2);
    return 0;
}
```

## 相关文档

- [WeightedDSU](weighted_dsu.md) —— 仅含等式约束 $x_v - x_u = w$ 时的近线性时间维护
- [Dijkstra](dijkstra.md) —— 非负边权下的单源/多源最短路
