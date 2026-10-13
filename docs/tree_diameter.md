# TreeDiameter

给定一棵树，求距离最远的两个顶点及它们之间的距离。

```cpp
auto [length, ends] = tree_diameter(tree);
```

无权树使用 `vector<vector<int>>`，边权树使用 `vector<vector<pair<int, T>>>`。返回直径长度和两个端点，复杂度为 $O(n)$。

输入必须是一棵连通树；加权版本默认边权非负。
