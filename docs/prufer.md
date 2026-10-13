# Prufer

在标号树和长度为 $n-2$ 的序列之间互相转换。

## 接口

```cpp
inline std::vector<int> prufer_degrees(const std::vector<int> &code);
inline std::vector<int> tree_to_prufer(const std::vector<std::vector<int>> &graph);
inline std::vector<std::pair<int, int>> prufer_to_tree(const std::vector<int> &code, int n = -1);
```

全部为 `inline` 自由函数。

## prufer_degrees

```cpp
std::vector<int> prufer_degrees(const std::vector<int> &code);
```

由 Prufer 序列还原每个顶点的度数。

**返回值**：长度 `code.size() + 2` 的数组，初值全为 1，对 `code` 中每个 $x$ 执行 `++degree[x]`。

**约束**
- 有 `assert(0 <= x && x < n)`，其中 $n = \mathrm{code.size()} + 2$

## tree_to_prufer

```cpp
std::vector<int> tree_to_prufer(const std::vector<std::vector<int>> &graph);
```

由树得到 Prufer 序列。

源码注释：`// Requires an undirected labelled tree. Leaves are removed in increasing order.`

**返回值**
- 长度 $n - 2$ 的序列
- $n \le 2$ 时返回 `{}`

**约束**
- 有 `assert(!leaves.empty())`、`assert(parent != -1)`
- 必须是无向的、带标号的**树**（$n-1$ 条边、连通、无环）

## prufer_to_tree

```cpp
std::vector<std::pair<int, int>> prufer_to_tree(const std::vector<int> &code, int n = -1);
```

由 Prufer 序列还原树。

源码注释：`// Empty code represents n=2 by default; pass n=0 or n=1 explicitly for those trees.`

**参数**
- `n == -1` 时自动取 $n = \mathrm{code.size()} + 2$
- 空序列默认表示 $n = 2$ 的树；要表示 $n = 0$ 或 $n = 1$ 必须显式传入

**返回值**
- `n <= 1` 时返回 `{}`（无边）
- 否则返回 $n - 1$ 条边

**约束**
- 有 `assert(n >= 0 && ((n <= 1 && code.empty()) || n == (int)code.size() + 2))`

## 使用示例

```cpp
#include "prufer.hpp"
#include <iostream>

int main() {
    // 树: 0-1, 1-2, 1-3
    std::vector<std::vector<int>> graph = {
        {1}, {0, 2, 3}, {1}, {1}
    };

    auto code = tree_to_prufer(graph);
    for (int x : code) std::cout << x << ' ';
    std::cout << '\n';   // 长度为 2

    // 还原
    auto edges = prufer_to_tree(code, 4);
    for (auto [u, v] : edges) std::cout << u << '-' << v << ' ';
    std::cout << '\n';

    // 度数
    auto degrees = prufer_degrees(code);
    for (int d : degrees) std::cout << d << ' ';
    std::cout << '\n';

    return 0;
}
```

### 无标号树的计数（Cayley 公式）

Prufer 序列的长度为 $n-2$，每个位置可填 $n$ 种顶点编号，因此标号树的个数是 $n^{n-2}$。这也是 Prufer 序列最经典的用途。

### 给定度数的树计数

$$
\text{方案数} = \frac{(n-2)!}{\prod_i (d_i - 1)!}
$$

其中 $d_i$ 是各顶点的度数，需满足 $\sum (d_i - 1) = n - 2$。

## 前置条件与实现约定

- **`prufer_to_tree` 返回的边的两端点顺序无统一语义**（构造时写作 `{leaf, x}`，最后一对是剩下的两个叶子），应视作**无序对**使用。
- `tree_to_prufer` 找父节点的方式是「第一个 `degree[to] > 0` 的邻居」并 `break`，**依赖输入确实是树**。自环或重边会导致结果错误，源码不校验。
- $n \le 2$ 的边界：`tree_to_prufer` 返回空序列；`prufer_to_tree` 需要**显式传 `n`** 才能区分 $n=0$、$n=1$、$n=2$。
- 两个方向都用最小堆（`priority_queue<int, vector<int>, greater<int>>`）取最小叶子，因此 Prufer 序列是**规范化**的（同一棵树总得到同一序列）。
- `prufer_degrees` 只检查下标范围，不检查序列本身是否是合法的 Prufer 序列（例如出现次数超过 $n-2$ 的顶点不会被拦下）。

## 相关文档

- [EulerPath](euler_path.md) —— 欧拉路径
- [MatrixTree](matrix_tree.md) —— 矩阵树定理，另一种生成树计数方式
- [TreeHash](tree_hash.md) —— 树同构判定
