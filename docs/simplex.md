# simplex

单纯形法求解：最大化 `c * x`，约束为 `A * x <= b` 且 `x >= 0`。

## 接口

```cpp
template <typename T, size_t R, size_t C>
std::pair<T, std::vector<T>> simplex(
    const Matrix<T, R, C>& A,
    const std::vector<T>& b,
    const std::vector<T>& c);
```

返回 `{objective, x}`：目标值为负无穷表示无解，正无穷表示无界。

```cpp
Matrix<double, 0, 0> A(3, 2, {1, 1, 1, 0, 0, 1});
auto result = simplex(A, {1, 1, 1}, {1, 1});
```
