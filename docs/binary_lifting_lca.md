# BinaryLiftingLCA

给定一棵树和一个根，预处理后回答任意两点的最近公共祖先查询。

## 类型

```cpp
class BinaryLiftingLCA;
```

非模板类。**文件中没有任何注释**。

## 构造函数

```cpp
BinaryLiftingLCA(const int n = 0);
```

内部：`logn = n ? (32 - __builtin_clz(n)) : 1`，即 $n \ge 1$ 时 `logn = floor(log2 n) + 1`，$n = 0$ 时 `logn = 1`。

## 成员函数

### add_edge

```cpp
void add_edge(const int u, const int v);
```

**双向加边**（无向），**无 `assert`**。

### prework

```cpp
void prework(const int root);
```

以 `root` 为根建表。会**整体重置** `depth`（长度 $n$，全 0）与 `anc`（$n \times \mathrm{logn}$，全 $-1$）后重新迭代建表。

**可对同一对象用不同 `root` 反复调用**。

### lca

```cpp
int lca(int u, int v) const;
```

返回 $u$、$v$ 的最近公共祖先。`const` 成员函数。

## 使用示例

```cpp
#include "binary_lifting_lca.hpp"
#include <iostream>

int main() {
    int n = 5;
    BinaryLiftingLCA lca(n);

    lca.add_edge(0, 1);
    lca.add_edge(0, 2);
    lca.add_edge(1, 3);
    lca.add_edge(1, 4);

    lca.prework(0);   // 必须先 prework

    std::cout << lca.lca(3, 4) << '\n';   // 1
    std::cout << lca.lca(2, 3) << '\n';   // 0
    std::cout << lca.lca(1, 1) << '\n';   // 1

    return 0;
}
```

## 调用顺序（重要）

必须严格按此顺序：

1. 构造 `BinaryLiftingLCA(n)`
2. 全部 `add_edge`
3. `prework(root)`
4. 才能调用 `lca`

源码**没有** `assert` 或注释提示这一点。跳过 `prework` 直接 `lca` 会因 `depth` / `anc` 为空而产生未定义行为。

### 补充说明与使用约定

- **输入必须是树**（以 `root` 为根）。DFS 只用 `to == f` 避免回头，**含环图会无限递归**。
- **不连通图**：只有 `root` 所在连通块会被初始化，其余顶点 `depth == 0`、`anc` 全 `-1`，`lca` 结果无意义。源码不报错。
- `add_edge` **不做越界检查**，顶点编号需在 $[0, n)$ 内。
- `lca` 默认构造（$n = 0$）后调用会崩溃。
- 建表时 `anc[to][i] = anc[anc[to][i-1]][i-1]` 的循环条件含 `~anc[to][i-1]`（上一级存在才继续），未填处保持 `-1`。这是正常的设计，但对 `-1` 敏感的代码要留意。
- 依赖 `__builtin_clz`（GCC / Clang 内建），$n = 0$ 被显式短路以免 UB。
- 只有 `lca` 一个查询接口，没有 `depth_of`、`distance`、k 级祖先等便捷方法——需要时从 `lca` 推导或自行改造。

## 相关文档

- [TarjanLCA](tarjan_lca.md) —— 离线算法，常数更优
- [HLD](hld.md) —— 树链剖分，顺带提供 LCA 与路径查询
- [TreeHash](tree_hash.md) —— 树同构
