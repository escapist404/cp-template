# Link-Cut Tree (`link_cut_tree.hpp`)

`link_cut_tree.hpp` 实现基于 Splay 维护实链剖分并支持换根（`make_root`）的动态树 `LinkCutTree<T>`，动态维护一个无向森林，支持动态连边、断边、连通性判定、单点点权修改与读取以及任意连通两点间的简单路径点权和查询。

## 接口说明

```cpp
template <typename T>
class LinkCutTree {
public:
    LinkCutTree(int n = 0);
    void set_value(int x, T value);
    T value(int x) const;
    bool connected(int x, int y);
    bool link(int x, int y);
    bool cut(int x, int y);
    T path_query(int x, int y);
};
```

- **索引约定（`1-based`）**：由于底层以节点编号 `0` 作为空节点哨兵（`tree[0].sum = T()`），**所有顶点编号必须为 `1-based`，合法范围为 $1 \le x, y \le n$**（传入 `0` 或越界会触发内部 `check(x)` 的 `assert` 断言失败）。
- **权值类型要求**：类型 `T` 必须支持默认构造 `T()`（作为零元）和二元加法 `operator+`，且由于 `make_root` 会翻转 Splay 子树左右儿子但 `pull` 不区分聚合方向，**`operator+` 必须满足交换律与结合律**（如数值加法、异或等）。
- `LinkCutTree(int n = 0)`：初始化包含 $n$ 个孤立顶点（编号 $1 \sim n$）的无边森林，所有顶点初始权值为 `T()`。
- `set_value(int x, T value)`：将顶点 `x` 的点权修改为 `value`（内部先 `access(x)` 再更新 `value` 与 `sum`）。
- `value(int x) const`：$O(1)$ 返回顶点 `x` 当前自身的点权。
- `connected(int x, int y)`：判断顶点 `x` 和 `y` 当前是否处于同一棵连通树中。
- `link(int x, int y)`：尝试在顶点 `x` 和 `y` 之间添加一条无向边：
  - 若 `x` 与 `y` 原本不在同一连通块，则将 `x` 换为所在树的根并连向 `y`，返回 `true`；
  - 若 `x` 与 `y` **已经在同一棵树中**（连边会成环），则**不修改结构并返回 `false`**。
- `cut(int x, int y)`：尝试断开顶点 `x` 和 `y` 之间的直接无向边：
  - 若森林中**确实存在直接连接 `x` 与 `y` 的边**，则将其断开并返回 `true`；
  - 若 `x` 与 `y` 不连通，或者虽然连通但中间隔了其他顶点（无直接边），则**不修改结构并返回 `false`**。
- `path_query(int x, int y)`：将 `x` 到 `y` 的唯一简单路径提取到同一棵 Splay 中并返回路径上所有顶点的点权和。**调用前必须保证 `connected(x, y)` 为真**（若不连通，返回值无意义）。

## 复杂度

- 构造：时间与空间复杂度均为 $O(n)$。
- `value(x)`：严格 $O(1)$。
- `set_value`、`connected`、`link`、`cut`、`path_query`：单次均摊时间复杂度均为 $O(\log n)$。

## 使用示例

```cpp
#include "link_cut_tree.hpp"
#include <cassert>

int main() {
    LinkCutTree<long long> lct(4); // 顶点编号 1..4
    for (int i = 1; i <= 4; ++i) {
        lct.set_value(i, i * 10);  // 点权: 10, 20, 30, 40
    }

    assert(lct.link(1, 2));
    assert(lct.link(2, 3));
    assert(!lct.link(1, 3));       // 已连通，拒绝成环并返回 false

    assert(lct.connected(1, 3));
    assert(!lct.connected(1, 4));
    assert(lct.path_query(1, 3) == 60); // 10 + 20 + 30 = 60

    lct.set_value(2, 50);
    assert(lct.path_query(1, 3) == 90); // 10 + 50 + 30 = 90

    assert(!lct.cut(1, 3));        // (1, 3) 无直接边，返回 false
    assert(lct.cut(2, 3));         // 断开边 (2, 3)
    assert(!lct.connected(1, 3));
    return 0;
}
```

## 相关文档

- [HLD](hld.md) —— 静态树上的重链剖分（常数更小，支持子树操作）
- [RollbackDSU](rollback_dsu.md) —— 离线动态图连通性（仅需判定连通性时可配合线段树分治）
