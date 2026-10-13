# coordinate_compress

对一组值做离散化，返回去重排序后的值和原数组对应的下标。

## 接口

```cpp
template <typename T>
std::pair<std::vector<T>, std::vector<int>>
coordinate_compress(const std::vector<T>& values);
```

返回 `{unique_values, index}`：`unique_values` 升序且无重复，`index[i]` 是
`values[i]` 在 `unique_values` 中的下标。

**复杂度**：`O(n log n)` 时间，`O(n)` 额外空间。

```cpp
auto [values, index] = coordinate_compress(std::vector<int>{5, 1, 5, 3});
// values == {1, 3, 5}
// index  == {2, 0, 2, 1}
```
