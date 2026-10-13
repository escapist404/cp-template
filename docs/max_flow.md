# MaxFlow

ACL 风格的 Dinic 最大流实现。

## 接口

```cpp
template <typename Cap>
struct MaxFlow;

MaxFlow(int n);
int add_edge(int from, int to, Cap cap);
Cap flow(int s, int t);
Cap flow(int s, int t, Cap flow_limit);
```

边编号是 `add_edge` 的返回值。容量必须非负，支持自环。

### 边信息

```cpp
struct edge { int from, to; Cap cap, flow; };

edge get_edge(int i) const;
std::vector<edge> edges() const;
void change_edge(int i, Cap new_cap, Cap new_flow);
std::vector<bool> min_cut(int s) const;
```

`cap` 是原始容量，`flow` 是当前流量。`min_cut[s]` 为真，表示顶点在残量网络中从 `s` 可达。

**复杂度**：Dinic 的一般复杂度为 `O(V^2 E)`。

## 示例

```cpp
MaxFlow<int> graph(4);
graph.add_edge(0, 1, 2);
graph.add_edge(0, 2, 1);
graph.add_edge(1, 3, 1);
graph.add_edge(2, 3, 1);
int answer = graph.flow(0, 3);
```
