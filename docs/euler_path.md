# euler_path

求有向或无向图的一条欧拉路径。边编号是输入 `edges` 中的下标，支持重边和自环。

## 接口

```cpp
struct EulerPath {
    bool exists = false;
    std::vector<int> vertices, edges;
};

EulerPath euler_path(
    int n, const std::vector<std::pair<int, int>>& edges,
    bool directed = false, int start = -1);
```

`start == -1` 时自动选择合法起点。空图在 `n > 0` 时返回单点路径；无解时返回默认的 `EulerPath{}`。

**复杂度**：`O(n + m)` 时间，`O(n + m)` 空间。

```cpp
auto path = euler_path(3, {{0, 1}, {1, 2}, {2, 0}});
// path.exists == true，path.edges.size() == 3
```
