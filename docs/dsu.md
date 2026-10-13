# DSU (Disjoint Set Union)

维护一组元素的划分，支持合并两个集合、判断是否同组以及查询集合代表。

## 构造函数

```cpp
DisjointSetUnion dsu(int n)
```

创建 $n$ 个元素的并查集，初始时每个元素独立成一组。

**约束**
- $0 \leq n \leq 10^8$

**复杂度**
- $O(n)$

## 成员函数

### get

```cpp
int dsu.get(int x)
```

返回元素 $x$ 所在集合的代表元素。

**约束**
- $0 \leq x < n$

**复杂度**
- 均摊 $O(\alpha(n))$

### merge

```cpp
void dsu.merge(int x, int y)
```

合并元素 $x$ 和 $y$ 所在的集合。如果已在同一集合则不做任何操作。

**约束**
- $0 \leq x, y < n$

**复杂度**
- 均摊 $O(\alpha(n))$

### same

```cpp
bool dsu.same(int x, int y)
```

判断 $x$ 和 $y$ 是否在同一集合中。

**约束**
- $0 \leq x, y < n$

**复杂度**
- 均摊 $O(\alpha(n))$

### size

```cpp
int dsu.size(int x)
```

返回包含元素 $x$ 的集合的大小。

**约束**
- $0 \leq x < n$

**复杂度**
- 均摊 $O(\alpha(n))$

### component_number

```cpp
int dsu.component_number()
```

返回当前连通分量的个数。

**复杂度**
- $O(1)$

### is_root

```cpp
bool dsu.is_root(int x)
```

判断 $x$ 是否为某个集合的代表元素。

**约束**
- $0 \leq x < n$

**复杂度**
- $O(1)$

## 使用示例

```cpp
#include "dsu.hpp"
#include <iostream>

int main() {
    DisjointSetUnion dsu(5);
    
    dsu.merge(0, 1);
    dsu.merge(2, 3);
    
    std::cout << dsu.same(0, 1) << std::endl;  // 1 (true)
    std::cout << dsu.same(0, 2) << std::endl;  // 0 (false)
    
    std::cout << dsu.size(0) << std::endl;     // 2
    std::cout << dsu.component_number() << std::endl;  // 3
    
    return 0;
}
```

## 实现说明

本实现使用**按秩合并**和**路径压缩**优化，保证了优秀的均摊时间复杂度。

- 使用负数存储集合大小（根节点）
- 使用负数索引存储父节点指针（非根节点）
- 合并时按大小合并，较小的集合合并到较大的集合
