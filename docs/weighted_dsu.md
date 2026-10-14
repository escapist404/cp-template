# Weighted DSU (`weighted_dsu.hpp`)

`weighted_dsu.hpp` 实现维护节点间相对势能（有向边权差）的带权并查集 `WeightedDSU<T>`，支持动态添加形如 $v[y] - v[x] = w$ 的等式约束、一致性检验以及查询任意连通两点间的势能差 $v[y] - v[x]$。

## 接口说明

```cpp
template <typename T = long long>
class WeightedDSU {
public:
    WeightedDSU(int n);
    bool merge(int x, int y, T difference);
    bool same(int x, int y);
    T difference(int x, int y);
};
```

- **模板参数与索引约定**：权值类型 `T` 默认为 `long long`，要求支持加减法与相等比较（`+`、`-`、`+=`、`==`），即构成阿贝尔群。所有顶点编号均为 **`0-based`**，合法范围为 $0 \le x, y < n$（越界会触发 `assert`）。
- `WeightedDSU(int n)`：初始化包含 $n$ 个独立顶点的带权并查集，每个顶点初始自成一个集合，到自身根节点的相对势能为 `T()`（即 $0$）。
- `merge(int x, int y, T difference)`：尝试添加约束 **`value[y] - value[x] == difference`**（注意方向是 **$y$ 减去 $x$**）：
  - 若 `x` 与 `y` 原本不在同一连通分量，则按大小合并两棵树，维护根节点间的势能偏移，并返回 `true`；
  - 若 `x` 与 `y` 已在同一连通分量，则检查现有势能差 `value[y] - value[x]` 是否恰好等于 `difference`：若一致则保持现状并返回 `true`，若矛盾则**不修改结构并返回 `false`**。
- `same(int x, int y)`：判断 `x` 与 `y` 是否在同一连通分量（即 `value[y] - value[x]` 是否已唯一确定），并在查找过程中执行带权路径压缩。
- `difference(int x, int y)`：返回 `value[y] - value[x]`。**调用前必须保证 `same(x, y)` 为真**，否则会触发内部 `assert(same(x, y))` 断言失败。

## 复杂度

- 构造：时间与空间复杂度均为 $O(n)$。
- `merge`、`same`、`difference`：均摊时间复杂度 $O(\alpha(n))$。

## 使用示例

```cpp
#include "weighted_dsu.hpp"
#include <cassert>

int main() {
    WeightedDSU<long long> dsu(4);

    // 约束: v[1] - v[0] = 5, v[2] - v[1] = 3
    assert(dsu.merge(0, 1, 5));
    assert(dsu.merge(1, 2, 3));

    assert(dsu.same(0, 2));
    assert(dsu.difference(0, 2) == 8);  // v[2] - v[0] = 8
    assert(dsu.difference(2, 0) == -8); // v[0] - v[2] = -8

    // 一致约束返回 true，矛盾约束返回 false
    assert(dsu.merge(0, 2, 8));
    assert(!dsu.merge(0, 2, 10));
    return 0;
}
```

## 相关文档

- [DSU](dsu.md) —— 基础无权并查集
- [RollbackDSU](rollback_dsu.md) —— 支持操作回滚的可撤销并查集
- [DifferenceConstraints](difference_constraints.md) —— 基于最短路求解不等式组差分约束
