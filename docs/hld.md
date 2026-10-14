# Heavy-Light Decomposition (`hld.hpp`)

`hld.hpp` 实现重链剖分（树链剖分）`HeavyLightDecomposition`，将树上简单路径与子树映射为連続的 DFS 序区间，支持快速求最近公共祖先（LCA）以及配合线段树或树状数组维护路径与子树信息。

## 接口说明

```cpp
class HeavyLightDecomposition {
public:
    HeavyLightDecomposition(int n = 0);
    void add_edge(int u, int v);
    void prework(int root);

    int parent(int u) const;
    int depth_of(int u) const;
    int head(int u) const;
    int pos(int u) const;
    std::pair<int, int> subtree_range(int u) const;
    int lca(int u, int v) const;
};
```

- **初始化与建树流程**：
  - 顶点编号为 **`0-based`**，范围 $0 \le u < n$。
  - 先通过 `HeavyLightDecomposition(n)` 构造实例，调用 $n - 1$ 次 `add_edge(u, v)` 添加无向树边，最后**必须调用一次 `prework(root)`** 完成两次 DFS 剖分（在调用 `prework` 之前不可调用任何查询接口）。
- **查询接口说明**：
  - `parent(u)`：返回节点 `u` 的父节点编号；根节点 `root` 的父节点为 **`-1`**。
  - `depth_of(u)`：返回节点 `u` 的深度（根节点深度为 `0`）。
  - `head(u)`：返回节点 `u` 所在重链的链顶（深度最小的顶点）编号。
  - `pos(u)`：返回节点 `u` 在剖分序（优先遍历重儿子）中的 **`0-based` 下标** $[0, n)$。**同一条重链上的顶点以及同一棵子树内的顶点在 `pos` 序中均连续分布**：重链顶 `head(u)` 到 `u` 的重链段对应闭区间 `[pos(head(u)), pos(u)]`（即半开区间 **`[pos(head(u)), pos(u) + 1)`**）。
  - `subtree_range(u)`：返回节点 `u` 的整棵子树在 `pos` 序中对应的**左闭右开区间 `[l, r)`**（即 `{pos(u), pos(u) + size[u]}`），可直接传入 [`SegmentTree::prod(l, r)`](segtree.md) 或 [`Fenwick::get(l, r)`](fenwick.md)。
  - `lca(u, v)`：返回顶点 `u` 与 `v` 的最近公共祖先。

## 复杂度

- `prework(root)`：时间与空间复杂度均为 $O(n)$。
- `parent`、`depth_of`、`head`、`pos`、`subtree_range`：严格 $O(1)$。
- `lca(u, v)`：最坏时间复杂度 $O(\log n)$（常数极小）。
- 任意路径 $u \leftrightarrow v$ 拆分为重链连续区间的段数：至多 $O(\log n)$ 段。

## 使用示例

```cpp
#include "hld.hpp"
#include "fenwick.hpp"
#include <cassert>
#include <utility>

int main() {
    HeavyLightDecomposition hld(5);
    hld.add_edge(0, 1);
    hld.add_edge(0, 2);
    hld.add_edge(1, 3);
    hld.add_edge(1, 4);
    hld.prework(0);

    assert(hld.lca(3, 4) == 1);
    assert(hld.lca(3, 2) == 0);

    // 配合树状数组维护点权并查询路径点权和与子树点权和
    Fenwick<long long> bit(5);
    for (int u = 0; u < 5; ++u) {
        bit.add(hld.pos(u), u + 1); // 点权设为 1..5
    }

    auto [sub_l, sub_r] = hld.subtree_range(1); // 子树 1 包含 {1, 3, 4}，权值 2 + 4 + 5 = 11
    assert(bit.get(sub_l, sub_r) == 11);

    // 树链剖分路径求和模板写法
    auto query_path = [&](int u, int v) {
        long long res = 0;
        while (hld.head(u) != hld.head(v)) {
            if (hld.depth_of(hld.head(u)) < hld.depth_of(hld.head(v))) std::swap(u, v);
            res += bit.get(hld.pos(hld.head(u)), hld.pos(u) + 1);
            u = hld.parent(hld.head(u));
        }
        if (hld.depth_of(u) > hld.depth_of(v)) std::swap(u, v);
        res += bit.get(hld.pos(u), hld.pos(v) + 1);
        return res;
    };
    assert(query_path(3, 2) == 4 + 2 + 1 + 3); // 路径 3 - 1 - 0 - 2
    return 0;
}
```

## 相关文档

- [BinaryLiftingLCA](binary_lifting_lca.md) —— 支持树上 $k$ 级祖先查询的倍增 LCA
- [LinkCutTree](link_cut_tree.md) —— 支持动态连边断边的实链剖分
