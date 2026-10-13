# EDCC

给定一张无向图，按桥把图分成若干个不含桥的连通块，并返回每个块的顶点集合。

Tarjan 的 `dfn/low` 遍历复用 `scc.hpp` 中的共享内部框架。

## 接口

```cpp
std::vector<std::vector<int>> get_edcc(const std::vector<std::vector<int>> &adj);
```

> **注意**：这个函数**没有 `inline` 修饰**（与 `scc.hpp` 不同）。若在多个 `.cpp` 中同时包含，会违反 ODR 导致链接错误。单文件提交时无碍。

## 返回值

- 边双连通分量列表，每个分量是**顶点编号的 `vector<int>`**
- 分量内顶点顺序是 BFS 发现顺序，**未排序**
- 外层每个元素就是一个点集合；孤立点也会单独返回一个只含自身的集合

## 语义

1. Tarjan 求全部桥
2. 在「删除桥后」的图上从顶点 0 起 BFS，划分连通块
3. 分量编号即 BFS 生成顺序

## 约束

- **无 `assert`**
- 顶点按 $0$ 到 $n-1$ 使用（源码未写 `// 0 index` 注释，但代码如此）
- 要求 `adj` 表示**无向图**：每条无向边在两侧各出现一次。源码不做校验

**复杂度**
- $O(n + m)$，源码未注明

## 使用示例

```cpp
#include "edcc.hpp"
#include <iostream>

int main() {
    // 三角形 0-1-2，通过桥 2-3 连到三角形 3-4-5
    std::vector<std::vector<int>> adj = {
        {1, 2}, {0, 2}, {0, 1, 3},
        {2, 4, 5}, {3, 5}, {3, 4}
    };

    auto edcc = get_edcc(adj);
    std::cout << edcc.size() << '\n';   // 2 个边双

    for (const auto &comp : edcc) {
        for (int v : comp) std::cout << v << ' ';
        std::cout << '\n';
    }

    return 0;
}
```

### 求桥的另一种用法

若只需要桥，可以比较顶点数：桥的两侧属于不同分量。

## 前置条件与实现约定

- **平行边处理依赖实现细节**。内部用 `std::unordered_set<int> bridge[n]` 存桥，BFS 时通过 `erase` **就地删除**已处理的桥（注释：`// to avoid possible bugs from multiple edges`）。父边只用 `to != f` 跳过，因此**第二条平行的父边会被当作回边**处理——平行边因此不会形成桥（这是正确行为，但依赖于这个实现细节）。
- 该函数**不修改传入的 `adj`**（`bridge` 是局部变量），只修改自己的局部结构。可安全重复调用。
- **孤点会各自成为一个只含自身的分量**。
- Tarjan 是递归 lambda，深图有爆栈风险。
- **缺少 `inline`**：多翻译单元场景需注意。

## 相关文档

- [SCC](scc.md) —— 有向图的强连通分量
- [VDCC](vdcc.md) —— 点双连通分量
- [RollbackDSU](rollback_dsu.md) —— 另一种动态连通性手段
