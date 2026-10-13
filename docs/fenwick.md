# Fenwick Tree (Binary Indexed Tree)

维护一个可修改的一维数组，支持单点加法和前缀或区间聚合查询。

## 模板参数

```cpp
template <typename T>
class Fenwick;
```

`T` 类型需要支持：
- 默认构造 `T()`，返回零元
- 加法运算 `+=` 和减法运算 `-`
- 常见类型：`int`、`long long`、`ModInt` 等

## 构造函数

```cpp
Fenwick<T> fenwick(int n)
Fenwick<T> fenwick(vector<T> init)
```

- 第一种：创建大小为 $n$ 的树状数组，初始值全为 $0$
- 第二种：用 `init` 数组初始化

**约束**
- $0 \leq n \leq 10^8$

**复杂度**
- $O(n)$ 或 $O(n \log n)$（用 `init` 初始化）

## 成员函数

### add

```cpp
void fenwick.add(int p, T value)
```

将位置 $p$ 的值加上 `value`。

**约束**
- $0 \leq p < n$

**复杂度**
- $O(\log n)$

### get

```cpp
T fenwick.get(int p)
T fenwick.get(int l, int r)
```

- `get(p)`：返回区间 $[0, p)$ 的和（前缀和）
- `get(l, r)`：返回区间 $[l, r)$ 的和

**约束**
- $0 \leq p \leq n$
- $0 \leq l \leq r \leq n$

**复杂度**
- $O(\log n)$

### lower_bound

```cpp
int fenwick.lower_bound(T value)
```

返回最小的 $p$ 使得 $\mathrm{get}(p+1) \geq \mathrm{value}$。如果不存在返回 $n$。

要求树状数组中的值**非负**。

**约束**
- 树状数组中所有值 $\geq 0$

**复杂度**
- $O(\log n)$

## 使用示例

### 基本用法

```cpp
#include "fenwick.hpp"
#include <iostream>

int main() {
    Fenwick<long long> fenwick(5);
    
    fenwick.add(0, 1);
    fenwick.add(1, 2);
    fenwick.add(2, 3);
    fenwick.add(3, 4);
    fenwick.add(4, 5);
    
    std::cout << fenwick.get(3) << std::endl;     // 6 (1+2+3)
    std::cout << fenwick.get(1, 4) << std::endl;  // 9 (2+3+4)
    std::cout << fenwick.get(5) << std::endl;     // 15 (总和)
    
    return 0;
}
```

### 用数组初始化

```cpp
#include "fenwick.hpp"
#include <vector>
#include <iostream>

int main() {
    std::vector<int> arr = {1, 2, 3, 4, 5};
    Fenwick<int> fenwick(arr);
    
    std::cout << fenwick.get(3) << std::endl;  // 6
    
    fenwick.add(2, 10);  // arr[2] += 10
    std::cout << fenwick.get(3) << std::endl;  // 16
    
    return 0;
}
```

### 使用 lower_bound

```cpp
// 找到最小的 p，使得前缀和 >= value
Fenwick<int> fenwick(5);
fenwick.add(0, 3);
fenwick.add(1, 2);
fenwick.add(2, 5);
fenwick.add(3, 1);
fenwick.add(4, 4);

int p = fenwick.lower_bound(10);
// 返回 3，因为 get(3) = 10
```

### 求逆序对

```cpp
#include "fenwick.hpp"
#include <vector>
#include <algorithm>

long long count_inversions(std::vector<int> arr) {
    int n = arr.size();
    
    // 离散化
    auto sorted = arr;
    std::sort(sorted.begin(), sorted.end());
    sorted.erase(std::unique(sorted.begin(), sorted.end()), sorted.end());
    
    for (int& x : arr) {
        x = std::lower_bound(sorted.begin(), sorted.end(), x) - sorted.begin();
    }
    
    Fenwick<long long> fenwick(sorted.size());
    long long inversions = 0;
    
    for (int i = n - 1; i >= 0; --i) {
        inversions += fenwick.get(arr[i]);
        fenwick.add(arr[i], 1);
    }
    
    return inversions;
}
```

## 对比：Fenwick vs SegmentTree

| 特性 | Fenwick | SegmentTree |
|-----|---------|-------------|
| 单点修改 | $O(\log n)$ | $O(\log n)$ |
| 区间查询 | $O(\log n)$ | $O(\log n)$ |
| 支持的运算 | 仅前缀和（可逆运算）| 任意满足结合律的运算 |
| 常数因子 | 小 | 较大 |
| 代码复杂度 | 简单 | 较复杂 |
| 内存使用 | 小 | 较大 |

**建议**：
- 只需维护前缀和时，优先使用 Fenwick（更快更简洁）
- 需要维护最值、GCD 等非前缀和信息时，使用 SegmentTree
- 需要区间修改时，使用 LazySegmentTree

## 实现说明

- 使用标准的树状数组实现
- `lower_bound` 使用倍增法实现，从高位到低位逐步确定答案
- 1-indexed 内部实现，对外接口为 0-indexed
