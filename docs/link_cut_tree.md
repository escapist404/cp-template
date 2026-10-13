# LinkCutTree

动态维护森林，支持连边、断边、连通性判断、修改点权和查询路径点权和。顶点编号从 1 开始。

```cpp
LinkCutTree<long long> tree(n);
tree.set_value(1, 5);
tree.link(1, 2);
long long sum = tree.path_query(1, 2);
tree.cut(1, 2);
```

所有操作均摊复杂度为 $O(\log n)$。`path_query` 使用 `operator+` 聚合点权，默认初值为 `T()`。

`link` 和 `cut` 成功时返回 `true`；操作不满足森林结构时返回 `false`。
