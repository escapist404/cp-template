# Gaussian

给定一个线性方程组的增广矩阵，求其秩、是否有解、一个特解以及零空间基。

主接口接受 `Matrix<T, R, C>`；原有的 `std::vector<std::vector<T>>` 接口仍保留，并在内部转换为动态矩阵。增广矩阵列数必须是 `variables + 1`。

```cpp
template <typename T, size_t R, size_t C, typename IsZero = GaussianZero<T>>
GaussianResult<T> gaussian_elimination(
    Matrix<T, R, C> augmented, int variables, IsZero is_zero = IsZero{});
```

元素类型需要支持加减乘除和零判断。整数模意义下应使用 `ModInt`，浮点数则可使用默认的近零判断策略。
