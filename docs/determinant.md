# Determinant

给定一个方阵，计算其行列式。

依赖 `matrix.hpp`。

## 接口

```cpp
template <typename T, size_t R, size_t C>
T determinant(const Matrix<T, R, C>& input);
```

要求 `input` 是方阵。函数按 `const` 引用接收输入，并在内部复制后消元，不修改原矩阵。

**约束**

- `T` 需支持加减乘除、比较 0、取负以及构造 `0`、`1`。
- Bareiss 中间结果需要精确除法；整数通常可用，域类型如 `ModInt` 也可用。

算法复杂度为 $O(n^3)$。

## 使用示例

```cpp
#include "determinant.hpp"
#include "matrix.hpp"
#include <iostream>

int main() {
    Matrix<long long, 0, 0> mat(3, 3);
    mat[0][0] = 1; mat[0][1] = 2; mat[0][2] = 3;
    mat[1][0] = 0; mat[1][1] = 1; mat[1][2] = 4;
    mat[2][0] = 5; mat[2][1] = 6; mat[2][2] = 0;

    std::cout << determinant(mat) << '\n';
}
```

## 相关文档

- [Matrix](matrix.md) —— 矩阵类
- [MatrixTree](matrix_tree.md) —— 矩阵树定理
- [LGV](lgv.md) —— LGV 引理
