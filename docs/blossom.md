# GeneralMatching（Blossom）

给定一张允许含奇环的无向图，求最多能选出多少条互不共享端点的边。

## 接口

```cpp
class GeneralMatching;
GeneralMatching(int n);
void add_edge(int u, int v);
int max_matching();
const std::vector<int>& matching();
```

0-based。`add_edge` 添加无向边。`max_matching()` 返回匹配**边数**，从空匹配重新计算。`matching()[u]` 是匹配伙伴，未匹配时为 `-1`。

## 约束与复杂度

- `n >= 0`；点编号在 `[0,n)`，不允许自环，允许重边。
- 不支持边权；目标为匹配边数最多。
- 时间 $O(n^3)$（简单图的标准界），空间 $O(n+m)$；大量重边还会增加邻接扫描成本。

## 使用示例

```cpp
#include "blossom.hpp"
#include <cassert>

int main() {
    GeneralMatching graph(4);
    graph.add_edge(0, 1);
    graph.add_edge(1, 2);
    graph.add_edge(2, 0);
    graph.add_edge(2, 3);
    assert(graph.max_matching() == 2);
    auto match = graph.matching();
    assert(match[match[0]] == 0);
}
```

### 补充说明与使用约定

- 加边后需再次 `max_matching()`，之前的匹配不会自动扩充。
- 二分图优先使用 Hopcroft–Karp；本模块适用于一般图。
- 本实现确定性，不使用随机代数匹配，不支持一般图最大权匹配。

## 相关文档

- [BipartiteMatching](bipartite_matching.md) —— 二分图最大匹配
- [KM](km.md) —— 二分图最大权匹配
