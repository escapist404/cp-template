# CentroidDecomposition

```cpp
CentroidDecomposition decomposition(tree);
int parent = decomposition.parent_of(vertex);
```

构造树的重心分解，并返回重心树的父节点。重心树根的父节点为 `-1`，复杂度为 $O(n\log n)$。

原树必须连通且无环。该模板只建立分解结构，具体统计信息应在外部按重心树层次维护。
