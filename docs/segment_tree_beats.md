# SegmentTreeBeats

维护数值数组，支持区间加法、取上界、取下界、求和与最值。0-based，所有区间为 `[l,r)`。

## 接口

```cpp
template <typename T = long long, typename Sum = T>
class SegmentTreeBeats;

SegmentTreeBeats(const std::vector<T>& a);
SegmentTreeBeats(int n, T value = T());
void add(int l, int r, T value);
void chmin(int l, int r, T value);  // a[i] = min(a[i], value)
void chmax(int l, int r, T value);  // a[i] = max(a[i], value)
Sum sum(int l, int r);
T min(int l, int r);
T max(int l, int r);
```

`T` 使用有符号整数类型，`Sum` 可以使用更宽的整数类型。需要大小比较、加减乘法和 `numeric_limits<T>`。

## 约束与复杂度

- `n >= 0`；修改和求和允许空区间，最值查询要求非空。
- 所有当前值、第二最值、加法标记及其中间运算需能放进 `T`，且严格位于 `numeric_limits<T>` 的两个边界之间；边界值保留作哨兵。
- 求和及其更新中间量需能放进 `Sum`；例如使用 `SegmentTreeBeats<int, long long>`。
- 构造时间与空间 $O(n)$。查询 $O(\log n)$；修改是摊还复杂度；标准分析为每次 $O(\log n)$ 的摊还界，单次修改最坏可能遍历整棵树。

## 使用示例

```cpp
#include "segment_tree_beats.hpp"
#include <cassert>
#include <vector>

int main() {
    SegmentTreeBeats<int, long long> st(std::vector<int>{1, 2, 3, 4});
    st.chmin(0, 4, 2);  // 1 2 2 2
    st.add(1, 3, 3);    // 1 5 5 2
    st.chmax(0, 2, 4);  // 4 5 5 2
    assert(st.sum(0, 4) == 16);
    assert(st.min(0, 4) == 2 && st.max(0, 4) == 5);
}
```

## 前置条件与实现约定

- `chmin` 是取上界，`chmax` 是取下界，不是赋值操作。
- 模整数不满足所需的大小比较，不能作为 `T`。
- 不保证每次更新都是对数时间；查询会下传标记。

## 相关文档

- [LazySegmentTree](lazy_segtree.md) —— 普通懒标记更新
- [SegmentTree](segtree.md) —— 通用结合运算
