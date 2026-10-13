# difference_constraints

求差分约束系统。边 `(u, v, w)` 表示 `x[v] <= x[u] + w`。

## 接口

```cpp
template <typename T = long long>
struct DifferenceConstraint {
    int u, v;
    T weight;
};

template <typename T = long long>
std::pair<bool, std::vector<T>> difference_constraints(
    int n, const std::vector<DifferenceConstraint<T>>& edges);
```

返回 `{是否有解, values}`。无解时 `values` 为空；有解时返回一组满足全部约束的值。

**复杂度**：SPFA 实现，最坏情况 `O(VE)`。

```cpp
auto result = difference_constraints<long long>(2, {{0, 1, 3}});
// result.first == true，result.second[1] <= result.second[0] + 3
```
