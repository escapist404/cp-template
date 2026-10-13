# DSUOnTree

给定一棵树和可增删的子树状态，依次计算每个顶点子树对应的统计结果。

## 文件注释

```
// Sack/DSU-on-tree with iterative traversals. add/remove(vertex,state) are exact
// inverses; answer(state,vertex) observes the current subtree aggregate.
```

## 类型

```cpp
class DSUOnTree;
```

## 构造函数

```cpp
DSUOnTree(int n);
```

## 成员函数

### add_edge

```cpp
void add_edge(int u, int v);
```

**双向加边**，无 `assert`。

### solve

```cpp
template <typename State, typename Add, typename Remove, typename Answer>
auto solve(int root, State &state, Add add, Remove remove, Answer answer) const;
```

各回调的调用形状（**顺序敏感**）：

```cpp
add(vertex, state);
remove(vertex, state);
answer(state, vertex);
```

**返回类型**
- `std::vector<std::decay_t<decltype(answer(state, root))>>`，长度 $n$
- `result[u]` 是「状态中恰好包含 $u$ 的子树信息」时调用 `answer(state, u)` 的返回值

**约束**
- 有 `assert(0 <= root && root < n)`
- 建 DFS 序时对每个新发现的 $v$ 有 `assert(parent[v] == -1)`：图中不允许环 / 重复访问
- 有 `assert((int)order.size() == n)`：**要求 `root` 能到达全部 $n$ 个顶点**（图必须连通），否则 debug 下断言失败

## 使用示例：统计子树内不同颜色数

```cpp
#include "dsu_on_tree.hpp"
#include <iostream>
#include <vector>

int main() {
    int n = 5;
    std::vector<int> color = {1, 2, 1, 3, 1};

    DSUOnTree tree(n);
    tree.add_edge(0, 1);
    tree.add_edge(0, 2);
    tree.add_edge(1, 3);
    tree.add_edge(1, 4);

    // 状态：每种颜色出现次数 + 当前非零颜色数
    std::vector<int> cnt(n + 1, 0);
    int distinct = 0;

    auto add = [&](int v, auto &) {
        if (cnt[color[v]]++ == 0) ++distinct;
    };
    auto remove = [&](int v, auto &) {
        if (--cnt[color[v]] == 0) --distinct;
    };
    auto answer = [&](auto &, int) {
        return distinct;
    };

    auto result = tree.solve(0, cnt, add, remove, answer);
    for (int i = 0; i < n; ++i) {
        std::cout << i << ": " << result[i] << '\n';
    }

    return 0;
}
```

### 状态传什么

`state` 的参数类型是任意的（`State &`），上例把捕获变量作为「状态载体」传入。也可以定义一个结构体把所有统计量打包，这样回调签名更清晰：

```cpp
struct Agg {
    std::vector<int> cnt;
    int distinct = 0;
};

auto add = [&](int v, Agg &agg) { /* ... */ };
auto answer = [&](Agg &agg, int v) { return agg.distinct; };
```

## 算法性质

- **全程迭代**（用显式 `std::vector` 当栈），**无递归爆栈问题**。这是本实现相对手写 DSU-on-tree 的主要优势。
- `order` 是前序遍历序列，`tin[u]` 为其下标；**子树区间恰为 `[tin[u], tin[u] + size[u])`**，「整子树加」与「整子树移除」都依赖这条性质。
- 任务栈 push 顺序是逆执行序：先处理轻儿子（不保留）→ 重儿子（`keep = true`）→ 回加轻儿子子树 → 加 `u` 自身 → `answer` → 若 `keep == false` 则移除整棵子树。
- 根的任务为 `{0, root, false}`，因此 **`solve` 返回后 `state` 会被恢复成与进入前一致**（前提是 `add`/`remove` 互为严格逆操作）。
- `heavy` 用 `size` 比较选取（相等时不替换，取先遇到的）；`heavy[u] == -1` 表示叶子。
- 结果向量**按下标（顶点号）索引**，每个顶点都会被赋一次值（含叶子和根）。
- 每次 `solve` 都自行重建 DFS 序与重儿子，没有单独的 `prework` 阶段。

## 前置条件与实现约定

- **`add` 与 `remove` 必须互为严格逆操作**。这是正确性的前提，源码的注释也强调了这一点。若 `add` 有副作用而 `remove` 不能撤销，结果会错。
- **图必须连通**（`assert(order.size() == n)`），且**不能有环**（`assert(parent[v] == -1)`）。
- `add_edge` 无 `assert`，顶点需在 $[0, n)$ 内。
- 回调按值传入（`Add add, Remove remove, Answer answer`），`state` 按引用传入。回调通常写成捕获外部变量的 lambda，此时按值传递的是 lambda 对象本身，捕获仍然有效。
- `answer` 的参数顺序是 `(state, vertex)`，**state 在前**——与直觉可能相反。
- 复杂度 $O(n \log n)$（每个顶点被加/删 $O(\log n)$ 次），源码未注明。

## 相关文档

- [HLD](hld.md) —— 树链剖分，可处理路径问题
- [DynamicSegmentTree](dynamic_segtree.md) —— 线段树合并，另一类子树合并技巧
- [TreeHash](tree_hash.md) —— 树同构
