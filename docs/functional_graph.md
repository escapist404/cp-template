# FunctionalGraph

给定每个顶点都恰有一个后继的有向图，预处理每个点所在的环、到环的距离、环长以及多次沿后继跳转后的顶点。

## 接口

```cpp
class FunctionalGraph;
FunctionalGraph(const std::vector<int>& next);
int jump(int vertex, std::uint64_t steps);
int component(int vertex);
int distance_to_cycle(int vertex);
int cycle_length(int vertex);
```

`next[u]` 是 `u` 的后继，编号为 0-based。同一个环及其入树具有相同 `component` 编号；编号由构造过程分配，不保证是某个顶点编号。

## 约束与复杂度

- 每个 `next[u]` 在 `[0,n)`，允许自环和空图。
- `distance_to_cycle(u)` 为第一次到达环的步数，环上点为 0。
- `cycle_length(u)` 为最终到达的环长；可查询树上点。
- `jump(u,0)=u`，步数支持完整 `uint64_t` 范围。
- 时间和空间 $O(64n)$，跳转 $O(64)$，其余查询 $O(1)$。

## 使用示例

```cpp
#include "functional_graph.hpp"
#include <cassert>

int main() {
    FunctionalGraph graph({1, 2, 0, 2});
    assert(graph.cycle_length(3) == 3);
    assert(graph.distance_to_cycle(3) == 1);
    assert(graph.jump(3, 2) == 0);
    assert(graph.component(3) == graph.component(0));
}
```

## 前置条件与实现约定

- 该图不是一般有向图；必须提供唯一后继，不能表示无出边的点。
- 后继不能修改；修改后需重新构造。
- `component` 相同不代表任意两点可以互相到达，入树方向只能朝环。
- 倍增表固定存储 64 层，注意大规模图的内存。

## 相关文档

- [BinaryLiftingLCA](binary_lifting_lca.md) —— 树上倍增
- [SCC](scc.md) —— 一般有向图强连通分量
