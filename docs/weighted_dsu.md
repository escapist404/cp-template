# WeightedDSU

维护形如 `value[y] - value[x] = w` 的相对差值约束，并支持合并约束、判断一致性和查询已连通点的差值。下标为 0-based。

## 接口

```cpp
template <typename T = long long>
class WeightedDSU;
WeightedDSU(int n);
bool merge(int x, int y, T difference);
bool same(int x, int y);
T difference(int x, int y);
```

`merge(x,y,w)` 加入约束 `value[y] - value[x] = w`。约束与已有关系一致时返回 `true`；矛盾时返回 `false`。即使两点已经连通，一致的约束也返回 `true`。

`difference(x,y)` 返回 `value[y] - value[x]`，要求两点已连通。

## 约束与复杂度

- `n >= 0`，点编号在 `[0,n)`。
- `T()` 是加法零元，支持 `+=`、加减法和精确相等比较；加法应满足交换律。可使用整数或模整数。
- 按大小合并、路径压缩，单次操作摊还 $O(\alpha(n))$，空间 $O(n)$；需保证差值运算不溢出。

## 使用示例

```cpp
#include "weighted_dsu.hpp"
#include <cassert>

int main() {
    WeightedDSU<long long> dsu(3);
    assert(dsu.merge(0, 1, 2));
    assert(dsu.merge(1, 2, 3));
    assert(dsu.difference(0, 2) == 5);
    assert(!dsu.merge(0, 2, 4));
}
```

## 相关文档

- [DSU](dsu.md) —— 普通并查集
- [DifferenceConstraints](difference_constraints.md) —— 不等式约束
