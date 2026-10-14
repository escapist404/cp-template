# TwoSat

给定若干个布尔变量和每条含两个文字的约束，判断是否存在满足所有约束的赋值，并在有解时返回一组赋值。

## 类型

```cpp
class TwoSat;
```

非模板类。

## 编码约定

- 变量 $x \in [0, n)$
- 文字索引 `2 * x + (xv ? 1 : 0)`，即 **`2x` 表示 `x == false`，`2x+1` 表示 `x == true`**
- 取反即 `^1`
- 内部图大小为 $2n$

## 构造函数

```cpp
TwoSat(const int n);
```

创建 $n$ 个变量的 2-SAT 实例。

## 成员函数

### add_implication

```cpp
void add_implication(const int x, const bool xv, const int y, const bool yv);
```

添加蕴含式，源码注释：`// (x == xv) implies (y == yv), including its contrapositive.`

即 $(x = xv) \Rightarrow (y = yv)$，同时自动加入逆否命题 $(y \neq yv) \Rightarrow (x \neq xv)$。

**约束**
- 有 `assert(0 <= x && x < n && 0 <= y && y < n)`

### add_or

```cpp
void add_or(const int x, const bool xv, const int y, const bool yv);
```

添加子句 $(x = xv) \lor (y = yv)$。

**实现**为 `add_implication(x, !xv, y, yv)`：即「$x$ 不等于 $xv$」蕴含「$y$ 等于 $yv$」。

**约束**
- 无自己的 `assert`，委托给 `add_implication`

### add_true

```cpp
void add_true(const int x, const bool value);
```

强制 $x = \mathrm{value}$。

**实现**为 `add_implication(x, !value, x, value)`。

### solve

```cpp
bool solve();
```

求解。有解返回 `true`，无解返回 `false`。

**行为**
- 会**重置** `component`、`answer`，因此可重复调用
- 但**已加入的边不会清除**，重复调用不会改变结果
- 复用 `scc.hpp` 的 Tarjan 强连通分量实现，并从返回的点集合建立内部颜色数组

### assignment

```cpp
const std::vector<int> &assignment() const;
```

源码注释：`// Valid only after solve() returns true.`

**返回值**
- 每个变量的取值：1 表示真，0 表示假
- 返回类型是 `std::vector<int>`（**不是 `vector<bool>`**），元素为 0/1

**赋值规则**：`answer[i] = component[2 * i] > component[2 * i + 1]`

## 使用示例

```cpp
#include "two_sat.hpp"
#include <iostream>

int main() {
    int n = 3;
    TwoSat sat(n);

    // (x0 = true) or (x1 = false)
    sat.add_or(0, true, 1, false);

    // x2 必须为真
    sat.add_true(2, true);

    // (x0 = true) implies (x1 = true)
    sat.add_implication(0, true, 1, true);

    if (sat.solve()) {
        const auto &ans = sat.assignment();
        for (int i = 0; i < n; ++i) {
            std::cout << "x" << i << " = " << ans[i] << '\n';
        }
    } else {
        std::cout << "unsatisfiable\n";
    }

    return 0;
}
```

### 无解检测

```cpp
TwoSat sat(2);
sat.add_true(0, true);
sat.add_true(0, false);

std::cout << sat.solve() << '\n';   // 0 (false)
```

### 补充说明与使用约定

- **`assignment()` 只在 `solve()` 返回 `true` 时有效**。无解时内容无意义。
- `solve()` 可重复调用，但**不会清除已加入的边**——想求解另一个实例必须新建对象。
- **没有 `add_xor`、`add_and`、`add_false` 等便捷接口**，源码中不存在。需要时自行用 `add_implication` 拼：
  - `add_false(x)` = `add_implication(x, true, x, false)`
  - `add_and(x, xv, y, yv)` = `add_true(x, xv)` + `add_true(y, yv)`
  - `x != y` = `add_or(x, true, y, true)` + `add_or(x, false, y, false)`
- 变量多时 DFS 是递归 lambda，**有爆栈风险**。
- 分量编号的拓扑语义源码未说明；本实现只在内部用相对大小判定，不对外暴露。
- `answer` 是 `vector<int>` 而非 `vector<bool>`，与 `std::vector<bool>` 的位压缩版本行为不同，取值时是普通 `int`。

## 相关文档

- [SCC](scc.md) —— 强连通分量（2-SAT 的本质工具）
- [Kruskal](kruskal.md) —— 另一个常用图论模板
