# Matrix / Gaussian / Determinant

提供矩阵的构造、访问、加法、数乘、乘法，以及基于矩阵的方程组求解和行列式计算。

依赖 `matrix.hpp`；高斯消元和行列式分别见 [Gaussian](gaussian.md) 与 [Determinant](determinant.md)。

## Matrix

### 模板参数

```cpp
template <typename T, size_t R = 0, size_t C = 0>
class Matrix;
```

`R`、`C` 都为 0 时使用运行期尺寸；否则使用编译期固定尺寸。

### 构造函数

动态矩阵：

```cpp
Matrix(size_t rows, size_t columns);
Matrix(size_t rows, size_t columns, std::initializer_list<T> values);
```

静态矩阵：

```cpp
Matrix();
Matrix(std::initializer_list<T> values);
```

初始化列表按行优先顺序填充，元素个数必须等于矩阵大小。动态矩阵的行数和列数都必须非零。

### 访问与大小

```cpp
size_t rows() const;
size_t columns() const;
template <typename IsZero = MatrixRankZero<T>>
int rank(IsZero is_zero = IsZero{}) const;
T* operator[](size_t row);
const T* operator[](size_t row) const;       // 动态矩阵
std::array<T, C>& operator[](size_t row);    // 静态矩阵
T& at(size_t row, size_t column);            // 仅动态矩阵
```

访问下标从 0 开始，使用 `mat[i][j]`。`at` 会检查边界；`operator[]` 不做边界检查。

### 运算

```cpp
Matrix& operator*=(T scalar);
Matrix& operator+=(const Matrix& rhs);
Matrix operator+(const Matrix& lhs, const Matrix& rhs);
Matrix operator*(const Matrix& lhs, const Matrix& rhs);
```

矩阵乘法要求左矩阵的列数等于右矩阵的行数；静态矩阵还支持编译期维度推导。动态矩阵可用 `resize(row, column)` 调整尺寸，传 0 表示保留对应的原尺寸。

## 使用示例

```cpp
#include "matrix.hpp"
#include <iostream>

int main() {
    Matrix<int, 2, 2> a{1, 2, 3, 4};
    Matrix<int, 2, 2> b{5, 6, 7, 8};
    auto c = a * b;
    std::cout << c[0][0] << ' ' << c[0][1] << '\n';

    Matrix<double, 0, 0> dynamic(2, 3);
    dynamic[0][0] = 2;
    dynamic[1][2] = 7;
}
```

矩阵支持流式输入输出，输出按行换行。

`rank()` 返回矩阵的秩。它要求元素类型支持域上的除法；默认使用 `value == T{}` 判断零，也可以传入自定义零判断器。秩为 `0` 的情况包括零矩阵；空维度矩阵的秩也定义为 `0`，但当前动态矩阵构造函数要求行数和列数非零。

## 相关文档

- [Gaussian](gaussian.md) —— 高斯消元
- [Determinant](determinant.md) —— 行列式
- [ModInt](modint.md) —— 模运算（常与矩阵配合）
- [MatrixTree](matrix_tree.md) —— 矩阵树定理
- [LGV](lgv.md) —— LGV 引理
