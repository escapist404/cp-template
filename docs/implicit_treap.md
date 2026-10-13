# ImplicitTreap

按位置分裂的 FHQ Treap，维护动态序列。支持插入、删除、单点修改、区间翻转、区间移动和聚合查询。

## 接口

```cpp
template <typename T = int, typename Merge = std::plus<T>>
class ImplicitTreap;
ImplicitTreap(std::vector<T> values = {}, Merge merge = Merge(),
              T identity = T(), unsigned seed = 712367821);
int size();
void insert(int position, T value);
void erase(int l, int r);
void reverse(int l, int r);
void move(int l, int r, int p);
void set(int position, T value);
T get(int position);
T query(int l, int r);
```

0-based，区间为 `[l,r)`。`insert(p,x)` 在现有位置 `p` 前插入，也可在末尾 `p=size()` 插入。

`move(l,r,p)` 先移除 `[l,r)`，再在**移除后的序列**位置 `p` 前插入；要求 `0 <= p <= size()-(r-l)`。

`Merge` 满足结合律，`identity` 为双侧单位元。默认求和；同时维护正向、反向聚合，因此也支持字符串拼接、矩阵乘法等非交换运算。`T` 需可复制、赋值。

## 复杂度

初始构造期望 $O(n\log n)$，单次操作期望 $O(\log n)$；以上按合并 $O(1)$ 计。空间为 $O(n+I)$，`I` 为后续插入次数，删除的节点不会回收复用。

## 使用示例

```cpp
#include "implicit_treap.hpp"
#include <cassert>
#include <string>

int main() {
    ImplicitTreap<std::string> seq({"a", "b", "c", "d"});
    seq.reverse(1, 4);              // a d c b
    seq.move(1, 3, 0);              // d c a b
    seq.insert(2, "x");            // d c x a b
    assert(seq.query(0, 5) == "dcxab");
}
```

## 前置条件与实现约定

- 查询允许空区间，返回 `identity`；删除和翻转空区间不改变序列。
- 自定义 `min/max` 聚合时必须提供对应的单位元，不能默认用 `T()`。
- 使用索引内存池，不返回裸节点指针；删除只解除引用，内存随整个对象销毁。
- 优先级由种子生成，可复现；随机树高度为期望界，递归深度最坏可能为 $O(n)$。
- 字符串等变长聚合需计入合并、复制成本。

## 相关文档

- [Treap](treap.md) —— 按键排序的多重集
- [SegmentTree](segtree.md) —— 固定长度序列
