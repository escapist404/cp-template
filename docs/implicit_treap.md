# Implicit Treap (`implicit_treap.hpp`)

`implicit_treap.hpp` 实现按序列下标分裂与合并的无旋 Treap（FHQ Treap），维护一个 **`0-based`** 动态序列，支持在任意位置插入、删除区间、单点修改、区间翻转、区间剪切移动以及带方向（支持非交换律）的区间聚合查询。

## 接口说明

### 类模板与构造函数

```cpp
template <typename T = int, typename Merge = std::plus<T>>
class ImplicitTreap {
public:
    ImplicitTreap(std::vector<T> values = {}, Merge merge = Merge(),
                  T identity = T(), unsigned seed = 712367821);
};
```

- `T`：序列元素与聚合结果的类型，默认 `int`。
- `Merge`：满足**结合律**的二元合并仿函数 `merge(a, b)`，默认 `std::plus<T>`。树节点内部同时维护正向聚合值 `forward` 与反向聚合值 `backward`，在执行区间翻转时直接交换两者，因此**即便 `Merge` 不满足交换律（如矩阵乘法、仿射变换复合、字符串哈希拼接），`reverse` 后的 `query` 结果依然完全正确**。
- `identity`：关于 `merge` 的双边幺元（单位元），满足 `merge(identity, x) == x` 且 `merge(x, identity) == x`；空区间的查询将返回 `identity`。若 `Merge` 为求最值或矩阵乘法，构造时务必显式传入对应的单位元（如 `INF` 或单位矩阵），不能依赖默认的 `T()`。

### 序列修改与查询

所有下标均为 **`0-based`**，所有区间参数 `[l, r)` 均为**左闭右开区间**：

```cpp
int size();
void insert(int position, T value);
void erase(int l, int r);
void reverse(int l, int r);
void move(int l, int r, int p);
void set(int position, T value);
T query(int l, int r);
T get(int position);
```

- `size()`：返回当前序列的长度 $n$。
- `insert(position, value)`：在下标 `position` 之前插入元素 `value`（要求 $0 \le \text{position} \le n$；传入 `size()` 即在末尾追加）。
- `erase(l, r)`：删除子区间 `[l, r)`（要求 $0 \le l \le r \le n$）。注意被删除的节点仍保留在底层 `std::vector<Node> tree` 池中不回收内存。
- `reverse(l, r)`：将子区间 `[l, r)` 原地翻转（要求 $0 \le l \le r \le n$）。
- `move(l, r, p)`：将子区间 `[l, r)` 从序列中剪下，并插入到**移除 `[l, r)` 之后的剩余序列**的下标 `p` 处。因此参数必须满足 $0 \le l \le r \le n$ 且 **$0 \le p \le n - (r - l)$**。
- `set(position, value)`：将下标 `position` 处的元素修改为 `value`（要求 $0 \le \text{position} < n$）。
- `query(l, r)`：返回子区间 `[l, r)` 从左到右按 `merge` 聚合的结果（要求 $0 \le l \le r \le n$；当 $l == r$ 时返回 `identity`）。注意 `query` 和 `get` 内部需要执行 `split` 与 `merge_nodes`，因此不是 `const` 成员函数。
- `get(position)`：返回下标 `position` 处的元素值（要求 $0 \le \text{position} < n$）。

## 复杂度

- 构造函数：时间复杂度 $O(n \log n)$，其中 $n = \text{values.size()}$。
- `size`：$O(1)$。
- `insert`、`erase`、`reverse`、`move`、`set`、`query`、`get`：期望时间复杂度均为 $O(\log n)$。
- 空间复杂度：$O(n + q_{\text{insert}})$，与历史累计插入的元素总数线性相关。

## 使用示例

```cpp
#include "implicit_treap.hpp"
#include <cassert>
#include <string>

int main() {
    ImplicitTreap<std::string> seq({"a", "b", "c", "d"}, std::plus<std::string>(), "");
    assert(seq.query(0, 4) == "abcd");

    seq.reverse(1, 4); // 翻转 [1, 4) -> "adcb"
    assert(seq.query(0, 4) == "adcb");

    seq.move(1, 3, 0); // 剪下 [1, 3) 即 "dc"，插到剩余序列 "ab" 的下标 0 处 -> "dcab"
    assert(seq.query(0, 4) == "dcab");

    seq.insert(2, "X"); // -> "dcXab"
    seq.erase(0, 1);    // -> "cXab"
    assert(seq.get(1) == "X");
    assert(seq.query(0, seq.size()) == "cXab");
    return 0;
}
```

## 相关文档

- [Treap](treap.md) —— 维护有序多重集的平衡二叉搜索树
- [SegmentTree](segtree.md) —— 静态序列上的单点修改与区间半群聚合
