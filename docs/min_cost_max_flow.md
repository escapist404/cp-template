# MinCostMaxFlow

ACL 风格的最小费用最大流实现。使用势能和 Dijkstra，边费用必须非负。

## 接口

```cpp
template <typename Cap, typename Cost>
struct MinCostMaxFlow;

MinCostMaxFlow(int n);
int add_edge(int from, int to, Cap cap, Cost cost);
std::pair<Cap, Cost> flow(int s, int t);
std::pair<Cap, Cost> flow(int s, int t, Cap flow_limit);
std::vector<std::pair<Cap, Cost>> slope(int s, int t);
```

`flow` 返回 `{总流量, 总费用}`。`slope` 返回每个费用斜率变化点的 `{流量, 费用}`。

### 边信息

```cpp
struct edge { int from, to; Cap cap, flow; Cost cost; };

edge get_edge(int i) const;
std::vector<edge> edges() const;
```

**复杂度**：每次增广使用 Dijkstra，整体复杂度为 `O(F E log V)` 的常见上界形式。

## 示例

```cpp
MinCostMaxFlow<int, int> graph(3);
graph.add_edge(0, 1, 2, 1);
graph.add_edge(1, 2, 2, 2);
auto [flow, cost] = graph.flow(0, 2);
// flow == 2, cost == 6
```
