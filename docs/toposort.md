# TopologicalSort

给定一张有向图，输出一个使每条边都从前指向后的顶点顺序；有环时报告不存在这样的顺序。

```cpp
std::vector<int> order = topological_sort(adj);
```

对有向图返回一个拓扑序；图中有环时返回空数组。复杂度为 $O(n+m)$。

图的边必须使用合法的 0-based 顶点编号。拓扑序不唯一时，返回顺序取决于队列中的入度为零顶点。
