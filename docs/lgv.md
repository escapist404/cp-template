# LGV

LGV 引理（Lindström–Gessel–Viennot），用行列式计数不相交路径族。

## 接口

```cpp
template <typename T, typename PathCount>
T lgv(const int k, PathCount count_paths);
```

依赖 `determinant.hpp`。

**约束**
- 有 `assert(k >= 0)`
- `k == 0` 直接返回 `T(1)`（源码注释：`// One empty path family.`）

**返回值**：$k \times k$ 矩阵 $\mathrm{paths}[i][j] = \mathrm{count\_paths}(i, j)$ 的行列式，即 Möbius 反演后带符号的和。

## 回调约定

```cpp
count_paths(i, j)
```

返回**从源 $i$ 到汇 $j$ 的所有路径权值之和**：单条路径的权值 = 边上权值之积（无权边用 1）。

**调用次数**：恰好 $k^2$ 次，$i, j \in [0, k)$，每个 $(i,j)$ 组合一次。

## 使用示例

```cpp
#include "lgv.hpp"
#include "modint.hpp"
#include <iostream>

using mint = ModInt<int, 1000000007>;

int main() {
    // k = 2 组源汇对，count_paths 由调用方实现
    const int k = 2;

    // 示例：调用方的路径计数 DP 可以替换这里的回调。
    auto count = [&](int i, int j) -> mint {
        return i == j ? mint(1) : mint(0);
    };

    mint result = lgv<mint>(k, count);
    std::cout << result << '\n';

    return 0;
}
```

### 典型建模步骤

1. 确定 $k$ 个源点 $s_0, \dots, s_{k-1}$ 与 $k$ 个汇点 $t_0, \dots, t_{k-1}$
2. 实现 `count_paths(i, j)`：从 $s_i$ 到 $t_j$ 的路径计数 DP
3. 调用 `lgv<T>(k, count_paths)` 得到不相交路径族的带符号计数

## 前置条件（由调用方保证）

源码注释明确写出 `// The caller must establish these graph/endpoint conditions.`：

- 图必须是 **DAG**
- 源、汇各 $k$ 个且**互不相同**
- 端点与图的合法性**不做任何检测**

## 计数语义

仅当**不存在非恒等置换**给出「顶点不相交路径族」时，返回值才等于「源 $i \to$ 汇 $i$（同下标配对）的族计数」。这是 LGV 引理的标准条件，源码注释也如此说明。

换句话说：若存在交叉的路径族，返回值会是差值而非计数。

## 复杂度

源码注释给出：

- $O(k^2)$ 次回调调用
- $O(k^3)$ 次算术运算
- $O(k^2)$ 空间

### 补充说明与使用约定

- **`T` 必须是域或不溢出的精确整型**。内部用 `determinant` 做行列式，需要除法。
- **图的合法性完全由调用方负责**：DAG、源汇互异、端点正确，全部不检测。违反条件时得到的是无意义数值而非报错。
- `count_paths` 被调用**恰好 $k^2$ 次**，若你的实现有较重开销，注意总代价。
- 除回调外不接触任何图结构，`lgv` 本身不「知道」图长什么样——所有信息都通过回调传入。
- 返回的是**行列式**，可能为负（在整型下）；只有在满足引理条件时它才等于非负的族计数。

## 相关文档

- [Determinant](determinant.md) —— 本文件的依赖
- [MatrixTree](matrix_tree.md) —— 矩阵树定理，另一种行列式计数
- [Comb](comb.md) —— 组合数（路径计数中常用）
