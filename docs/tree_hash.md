# TreeHash

给定两棵无标号树，为每棵树计算一个可比较的结构摘要，用于快速筛选是否可能同构。

## 文件注释

```
// Probabilistic, unlabelled tree isomorphism. Compare hashes with the SAME seed.
// All traversals are iterative. solve(root) is rooted; solve() is unrooted.
```

## 类型

```cpp
class TreeHash;
```

内部 `using u64 = std::uint64_t;`。

## 构造

```cpp
TreeHash(int n, u64 seed = 0x243f6a8885a308d3ULL);
```

**`seed` 只能在构造时指定**——它是私有成员，没有 getter / setter。

## 成员函数

### add_edge

```cpp
void add_edge(int u, int v);
```

**双向加边**，无 `assert`。必须在调用 `solve` / `centers` 之前完成。

### centers

```cpp
std::vector<int> centers() const;
```

返回树的**一个或两个重心**（叶剥法：初始把度数 $\le 1$ 的点入队，`remaining > 2` 时反复剥叶）。

- $n \le 2$ 时返回所有度 $\le 1$ 的点
- $n = 0$ 返回空；$n = 1$ 返回 `{0}`

**约束**
- 有 `assert(!leaves.empty())`（剥叶过程应始终有叶子）

### solve

```cpp
u64 solve(int root = -1) const;
```

**`root == -1`：无根哈希**
- 取 `centers()`（1 或 2 个），分别求有根哈希，排序后混合，并混入 `graph.size()`

**`root >= 0`：有根哈希**
- 返回以 `root` 为根的有根哈希
- 有 `assert(0 <= root && root < (int)graph.size())`

**边界**：`graph.empty()` 时直接返回 `mix(seed)`，**这个判断先于 `root` 的断言**。

**约束**
- 建树时有 `assert(father[v] == -1)`（禁止环）
- 有 `assert(order.size() == graph.size())`：**必须连通**

## 使用示例

```cpp
#include "tree_hash.hpp"
#include <iostream>

int main() {
    // 两棵结构相同的树（顶点重标号）
    TreeHash a(4);
    a.add_edge(0, 1); a.add_edge(1, 2); a.add_edge(1, 3);

    TreeHash b(4);
    b.add_edge(2, 0); b.add_edge(0, 1); b.add_edge(0, 3);

    // 无根同构判定
    std::cout << (a.solve() == b.solve()) << '\n';   // 1

    // 有根哈希
    std::cout << (a.solve(0) == b.solve(2)) << '\n';

    return 0;
}
```

### 判定两棵树（都以顶点 0 为根）是否同构

```cpp
bool rooted_isomorphic(const TreeHash &a, const TreeHash &b) {
    return a.solve(0) == b.solve(0);
}
```

### 比较时的注意事项

两个哈希值**只有用同一个 `seed` 计算出的才可以比较**。默认 `seed` 是固定常量，因此同一程序内直接比较总是安全的；若为了抗 Hack 而随机化 `seed`，必须让参与比较的所有树共用同一个 `seed`。

## 实现

- 混合函数 `mix` 为 splitmix64 风格（常量 `0x9e3779b97f4a7c15`、`0xbf58476d1ce4e5b9`、`0x94d049bb133111eb`）
- 子哈希**排序后依次混入**，这正是无标号同构判定的关键
- 有根哈希还混入子节点个数 `children.size()`；无根哈希混入 `graph.size()` 与重心个数
- 遍历均为**迭代式**，无爆栈问题

## 前置条件与实现约定

- **是概率算法**，不是严格判定。$2^{64}$ 空间的碰撞概率极低，但理论上存在；出题人若已知 `seed` 且刻意构造，可以 Hack。**默认 `seed` 是公开常量**，与 [RollingHash](rolling_hash.md) 的情况相同。
- **必须连通且无环**。有环或非连通输入在 debug 下断言失败，release 下产生错误哈希（源码未说明）。
- **必须在 `add_edge` 全部完成后才调用 `solve` / `centers`**。
- `seed` 无 getter / setter，只能在构造时指定。
- `solve` 与 `centers` 都是 `const`，可重复调用（会重新计算，无缓存）。
- 只能得到哈希值，**没有「返回两棵树是否同构」的直接接口**，需自行比较。

## 相关文档

- [Prufer](prufer.md) —— 树 ↔ 序列的双射
- [DSUOnTree](dsu_on_tree.md) —— 子树统计
- [HLD](hld.md) —— 树链剖分
