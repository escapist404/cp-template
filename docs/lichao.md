# LiChao

在一组固定离散坐标上维护若干函数，回答指定坐标处的最优函数值，也可限制函数只在某个坐标区间内生效。

## 模板参数

```cpp
template <typename X, typename Function, typename Compare = std::less<>>
class LiChaoTree;
```

- `X` 是坐标类型。
- `Function` 可调用为 `value = function(x)`，且需要默认构造。
- `Compare` 用来判断哪个值更优。默认 `std::less<>` 求最小值，`std::greater<>` 求最大值。

任意两条函数在坐标序列上的优劣顺序必须至多切换一次，这是李超树成立的前提。

## 构造函数

```cpp
LiChaoTree(std::vector<X> coordinates, Compare better = Compare{});
```

构造时会排序并去重 `coordinates`；坐标集合随后固定不变，且不能为空。

## 成员函数

```cpp
void add(const Function& function);
void add(const Function& function, const X& low, const X& high);
std::optional<Value> get(const X& x) const;
```

第一种 `add` 让函数在所有坐标上生效。第二种只让它在坐标区间 `[low, high)` 内生效；区间端点会按坐标集合做 `lower_bound`，因此只影响集合中实际存在的坐标。

`get(x)` 返回坐标 `x` 处的最优值；`x` 必须在构造时给出的坐标集合中。没有任何函数覆盖该点时返回空 `std::optional`。

插入和查询的复杂度均为 $O(\log n)$，空间复杂度为 $O(n)$，其中 `n` 是去重后的坐标数。

## 使用示例

```cpp
#include "lichao.hpp"
#include <functional>
#include <iostream>

struct Line {
    long long slope = 0, intercept = 0;
    long long operator()(long long x) const {
        return slope * x + intercept;
    }
};

int main() {
    std::vector<long long> xs = {-10, -5, 0, 5, 10};
    LiChaoTree<long long, Line> tree(xs);             // 默认求最小值
    tree.add({1, 0});                                 // y = x
    tree.add({-1, 10}, -5, 10);                       // 只在 [-5, 10) 生效

    auto value = tree.get(5);
    if (value) std::cout << *value << '\n';
}
```

### 求最大值

```cpp
LiChaoTree<long long, Line, std::greater<>> tree(xs);
tree.add({1, 0});
```

## 典型应用：DP 斜率优化

当 DP 转移能写成「某个函数在 `x = a[i]` 处的值」时，可把所有候选函数加入李超树，在离散的 `a[i]` 上查询最优值。

## 相关文档

- [SegmentTree](segtree.md) —— 普通线段树
- [ConvexHullTrick](convex_hull_trick.md) —— 若斜率单调，用单调栈更快
- [DynamicSegmentTree](dynamic_segtree.md) —— 动态开点
