# bipartite_matching

给定一张二分图，求最多能选出多少条互不共享端点的边。左侧顶点为 `[0, adj.size())`，右侧顶点为
`[0, right_size)`。

## 接口

```cpp
struct BipartiteMatchingResult {
    int size;
    std::vector<int> left, right;
};

BipartiteMatchingResult bipartite_matching(
    const std::vector<std::vector<int>>& adj, int right_size);
```

`left[u]` 是左侧顶点 `u` 匹配到的右侧顶点，未匹配时为 `-1`；`right[v]` 含义相反。

**复杂度**：`O((V + E) sqrt(V))`。

```cpp
auto result = bipartite_matching({{0, 1}, {1}}, 2);
// result.size == 2
```
