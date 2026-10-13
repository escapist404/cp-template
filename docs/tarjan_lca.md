# TarjanLCA

给定一棵树和一批顶点对，离线求出每一对顶点的最近公共祖先。

## 接口

```cpp
std::vector<int> tarjan_lca(const std::vector<std::vector<int>> &adj,
                            const int root,
                            const std::vector<std::pair<int, int>> &queries);
```

文件注释：`// 0 index`。

> **注意**：这个函数**没有 `inline`**，多翻译单元包含有 ODR 风险。

## 返回值

长度等于 `queries.size()` 的数组，`res[i]` 为第 $i$ 个询问 $(u, v)$ 的 LCA。

## 约束

- **无任何 `assert`**
- `adj` 必须是**以 `root` 为根的树**。源码只跳过 `to == f`：**有环会无限递归**；有多个连通块时，块外的顶点不会被访问，其相关询问结果**保持初值 0**
- 顶点编号 $0$ 到 $n-1$
- 查询端点越界源码不校验

**复杂度**
- $O((n + q) \alpha(n))$，源码未注明

## 使用示例

```cpp
#include "tarjan_lca.hpp"
#include <iostream>

int main() {
    int n = 5;
    std::vector<std::vector<int>> adj(n);

    auto add = [&](int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    };

    add(0, 1);
    add(0, 2);
    add(1, 3);
    add(1, 4);

    std::vector<std::pair<int, int>> queries = {
        {3, 4}, {2, 3}, {1, 1}, {0, 4}
    };

    auto answers = tarjan_lca(adj, 0, queries);
    for (int x : answers) std::cout << x << ' ';
    std::cout << '\n';
    // 1 0 1 0

    return 0;
}
```

## 与倍增 LCA 的取舍

| | TarjanLCA | BinaryLiftingLCA |
|---|---|---|
| 方式 | 离线（一次性回答全部询问） | 在线 |
| 复杂度 | $O((n+q)\alpha(n))$ | 预处理 $O(n\log n)$，单次 $O(\log n)$ |
| 常数 | 小 | 较大 |
| 内存 | $O(n + q)$ | $O(n \log n)$ |
| 递归 | 递归 lambda（有爆栈风险） | 递归成员函数 |

询问量大且可离线时，Tarjan 更优；需要边算边答时用倍增。

## 前置条件与实现约定

- **必须传入真正的树**。有环会无限递归；不连通时块外顶点的询问**静默返回 0**，与真实的 LCA = 0 无法区分。
- 询问会**双向登记**（`queries_on_node[u]` 与 `[v]` 各存一份），答案在两端点中「较晚完成」的那次填入。自询问 $(u, u)$ 可正常得到 $u$。
- 内部并查集是**手写内联版本**（`father[cur] == cur` 表示根，带路径压缩）。DFS 退出时执行 `father[cur] = f` 把当前点重新挂回 DFS 父亲——**这是该离线算法的关键，不要误以为可省略而删掉**。
- 递归 lambda，链状树上深递归有爆栈风险。
- **缺少 `inline`**。

## 相关文档

- [BinaryLiftingLCA](binary_lifting_lca.md) —— 在线版本
- [HLD](hld.md) —— 树链剖分，顺带提供 LCA
- [DSU](dsu.md) —— 并查集
