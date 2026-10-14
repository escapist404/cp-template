# MatrixTree

给定一张图，统计它的生成树数量，或计算所有生成树权值之和。

## 文件注释

```
// Weighted undirected graph. Parallel edges are allowed; self-loops are ignored.
// T must support +, -, *, /, unary -, equality, and construction from 0 and 1.
```

依赖 `determinant.hpp`（进而 `matrix.hpp`）。

## 类型

```cpp
template <typename T>
struct MatrixTreeEdge {
    int u, v;
    T weight;
    MatrixTreeEdge(const int u, const int v, const T &weight = T(1));
};
```

## 接口

```cpp
// 带权版本
template <typename T>
T matrix_tree(const int n, const std::vector<MatrixTreeEdge<T>> &edges);

// 无权版本，每条边权取 T(1)
template <typename T = long long>
T matrix_tree(const int n, const std::vector<std::pair<int, int>> &edges);
```

**返回值**：生成树的数目（带权时为按权相乘的生成树权值之和），即 Laplacian 的任一 $(n-1)$ 阶主子式的行列式。

**约束**
- 有 `assert(n >= 1)`
- **$n = 1$ 时直接返回 `T(1)`，此时不检查任何边**（边被完全忽略）
- 对每条边：有 `assert(0 <= edge.u && edge.u < n)`、`assert(0 <= edge.v && edge.v < n)`（在 $n=1$ 提前返回之后才执行）
- **自环被跳过**（`u == v`）
- **平行边被累加**（源码注释确认）

**复杂度**：由 `determinant` 的 Bareiss 消元决定，$O(n^3)$ 量级，源码未注明。

## 使用示例

### 无权图：统计生成树个数

```cpp
#include "matrix_tree.hpp"
#include <iostream>
#include <vector>

int main() {
    // 完全图 K3：三角形，有 3 棵生成树
    int n = 3;
    std::vector<std::pair<int, int>> edges = {
        {0, 1}, {1, 2}, {0, 2}
    };

    std::cout << matrix_tree(n, edges) << '\n';   // 3

    return 0;
}
```

### 带权图

```cpp
std::vector<MatrixTreeEdge<long long>> edges = {
    {0, 1, 2},
    {1, 2, 3},
    {0, 2, 1}
};

std::cout << matrix_tree<long long>(3, edges) << '\n';
// 所有生成树的 (边权乘积) 之和
```

### 模意义下计数

`T` 可以是 `ModInt`，此时结果是模意义下的生成树数目：

```cpp
using mint = ModInt<int, 1000000007>;
std::vector<MatrixTreeEdge<mint>> edges = { {0,1}, {1,2}, {0,2} };
std::cout << matrix_tree<mint>(3, edges) << '\n';
```

### 补充说明与使用约定

- **`T` 必须支持精确除法**。内部用 **Bareiss** 消元（源码注释：`// Bareiss elimination keeps every intermediate value integral.`），`int` / `long long` 可以（Bareiss 保证整除），`ModInt` 等域也可以。**浮点类型虽然能编译，但精度无保证**。
- **不连通图返回 `T(0)`**（行列式为 0）。源码**没有显式判定**，需要调用方自己意识到这一点。
- $n = 1$ 是特例：直接返回 1，**完全忽略传入的边**（哪怕边里写了自环或越界顶点）。
- 带权版本求的是「生成树权值之和」（每棵树的权 = 各边权之积），不是「最小生成树」——求最小生成树请用 [Kruskal](kruskal.md)。
- 平行边会被累加：两条平行的 $u$-$v$ 边（权重 $w_1$、$w_2$）在 Laplacian 中等价于一条权重 $w_1 + w_2$ 的边。
- 自环被跳过，因为自环不影响生成树数目（也不影响 Laplacian）。
- 无权重载会构造一份 `std::vector<MatrixTreeEdge<T>>` 再转调带权重载，有个小拷贝开销。

## 相关文档

- [Determinant](determinant.md) —— 行列式（本文件的依赖）
- [Matrix](matrix.md) —— 矩阵类
- [Kruskal](kruskal.md) —— 最小生成树（注意区别：不是计数）
- [LGV](lgv.md) —— LGV 引理，另一种用行列式计数的技巧
