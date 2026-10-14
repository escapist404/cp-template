# Disjoint Set Union (`dsu.hpp`)

`dsu.hpp` 实现同时包含**路径压缩**与**按大小合并**的并查集 `DisjointSetUnion`，用于高效维护元素的等价类划分、连通性查询、集合大小及连通块总数。

## 接口说明

```cpp
class DisjointSetUnion {
public:
    DisjointSetUnion(int n);
    int get(int x);
    void merge(int x, int y);
    bool same(int x, int y);
    int size(int x);
    bool is_root(int x);
    int component_number();
};
```

- **索引约定**：所有元素编号均为 **`0-based`**，合法范围为 $0 \le x, y < n$（内部未加 `assert`，调用者需自行保证下标不越界）。
- `DisjointSetUnion(int n)`：初始化包含 $n$ 个独立元素的并查集，初始有 $n$ 个连通分量，每个分量大小为 $1$。内部使用单个数组 `data` 同时存储根节点大小（正数）与非根节点的父指针编码（负数 `-(parent + 1)`）。
- `get(int x)`：查找元素 `x` 所在集合的代表元（根节点），并在查找过程中执行路径压缩。
- `merge(int x, int y)`：合并 `x` 和 `y` 所在的集合。若已在同一集合则无操作；否则将较小集合的根挂到较大集合的根下，并将连通块计数减 $1$。注意合并时的根选取由集合大小决定，若需指定根方向，应在外部额外维护代表元映射。
- `same(int x, int y)`：判断 `x` 和 `y` 是否处于同一连通分量。
- `size(int x)`：返回 `x` 所在连通分量的元素总数。
- `is_root(int x)`：判断 `x` 当前是否为所在集合的根节点（即 `data[x] > 0`）。
- `component_number()`：返回当前剩余的连通分量总数。

## 复杂度

- 构造：时间与空间复杂度均为 $O(n)$。
- `get`、`merge`、`same`、`size`：均摊时间复杂度 $O(\alpha(n))$，其中 $\alpha$ 为反阿克曼函数。
- `is_root`、`component_number`：严格 $O(1)$。

## 使用示例

```cpp
#include "dsu.hpp"
#include <cassert>

int main() {
    DisjointSetUnion dsu(5);
    assert(dsu.component_number() == 5);

    dsu.merge(0, 1);
    dsu.merge(1, 2);
    assert(dsu.same(0, 2));
    assert(!dsu.same(0, 3));
    assert(dsu.size(0) == 3);
    assert(dsu.component_number() == 3);
    return 0;
}
```

## 相关文档

- [WeightedDSU](weighted_dsu.md) —— 维护节点间有向势能差的带权并查集
- [RollbackDSU](rollback_dsu.md) —— 支持撤销操作的可回滚并查集
- [Kruskal](kruskal.md) —— 基于并查集的最小生成树算法
