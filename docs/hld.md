# HLD

给定一棵树，把每个子树映射为连续的 DFS 序区间，并支持最近公共祖先及路径拆分所需的信息查询。


## 类型与构造

```cpp
class HeavyLightDecomposition;
HeavyLightDecomposition(int n = 0);
```

顶点编号为 `0..n-1`。

## 成员函数

```cpp
void add_edge(int u, int v);
void prework(int root);

int parent(int u) const;
int depth_of(int u) const;
int head(int u) const;
int pos(int u) const;
std::pair<int, int> subtree_range(int u) const;
int lca(int u, int v) const;
```

`add_edge` 添加无向边。`prework` 应在所有边添加完后调用一次；之后 `pos(u)` 是 DFS 序位置，`subtree_range(u)` 返回半开区间 `[pos(u), pos(u) + subtree_size(u))`。

`head(u)` 返回 `u` 所在重链的链头，`parent(u)` 和 `depth_of(u)` 分别返回父节点与深度。

## 使用示例

```cpp
#include "hld.hpp"
#include <iostream>

int main() {
    HeavyLightDecomposition hld(5);
    hld.add_edge(0, 1);
    hld.add_edge(0, 2);
    hld.add_edge(1, 3);
    hld.add_edge(1, 4);
    hld.prework(0);

    std::cout << hld.lca(3, 4) << '\n';       // 1
    std::cout << hld.lca(2, 3) << '\n';       // 0

    auto [left, right] = hld.subtree_range(1);
    std::cout << left << ' ' << right << '\n';
}
```

### 配合线段树遍历路径

类本身不维护路径查询或修改；使用 `head`、`parent` 和 `pos` 把路径拆成重链段，再交给自己的线段树：

```cpp
while (hld.head(u) != hld.head(v)) {
    if (hld.depth_of(hld.head(u)) < hld.depth_of(hld.head(v)))
        std::swap(u, v);
    int l = hld.pos(hld.head(u));
    int r = hld.pos(u) + 1;             // 半开区间 [l, r)
    // 在线段树上处理 [l, r)
    u = hld.parent(hld.head(u));
}
if (hld.depth_of(u) > hld.depth_of(v))
    std::swap(u, v);
// 最后一段为 [pos(u), pos(v) + 1)，是否包含 u 由题意决定。
```

## 相关文档

- [BinaryLiftingLCA](binary_lifting_lca.md) —— 倍增 LCA
- [TarjanLCA](tarjan_lca.md) —— 离线 LCA
- [SegmentTree](segtree.md) / [LazySegmentTree](lazy_segtree.md) —— 配合使用
- [DSUOnTree](dsu_on_tree.md) —— 另一种子树统计
