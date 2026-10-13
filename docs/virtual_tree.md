# VirtualTree

```cpp
VirtualTreeBuilder builder(tree, root);
VirtualTree result = builder.build(required_vertices);
```

将给定点集和它们之间必要的 LCA 压缩成虚树。`result.vertices` 是虚树顶点，`result.edges` 是父到子的有向边。预处理复杂度为 $O(n\log n)$，每次构建为 $O(k\log k)$。

输入必须是一棵连通树，所有顶点编号从 0 开始。
