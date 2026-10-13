# kuhn_munkres

给定一个完全二分图上的边权，选择互不共享端点的边，使总权值最大；要求左侧点数不超过右侧点数。

## 接口

```cpp
template <typename T>
struct KuhnMunkresResult {
    T weight{};
    std::vector<int> left, right;
};

template <typename T>
KuhnMunkresResult<T> kuhn_munkres(
    const std::vector<std::vector<T>>& weight);
```

`left` 和 `right` 是双向匹配数组，未匹配时为 `-1`。`T` 应为有符号数值类型，且中间计算不溢出。

**复杂度**：`O(n^2 m)`，其中 `n` 为左侧点数，`m` 为右侧点数。

```cpp
auto result = kuhn_munkres(std::vector<std::vector<int>>{{3, 1}, {2, 4}});
// result.weight == 7
```
