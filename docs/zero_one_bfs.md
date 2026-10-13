# ZeroOneBFS

```cpp
std::vector<std::vector<std::pair<int, int>>> adj(n);
auto distance = zero_one_bfs(adj, source);
```

计算边权只能为 0 或 1 的有向图最短路。复杂度为 $O(n+m)$，不可达点为 `INT_MAX`。

边权不是 0/1 时应使用 [Dijkstra](dijkstra.md)。
