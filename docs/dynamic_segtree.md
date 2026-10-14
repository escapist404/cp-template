# Dynamic Segment Tree (`dynamic_segtree.hpp`)

`dynamic_segtree.hpp` 实现共享节点池的动态开点线段树森林 `DynamicSegmentTree<V, op, e, Index>`，支持在大值域（如 $[-10^{18}, 10^{18})$）上进行单点修改、区间聚合查询、树上二分以及两棵动态开点线段树的合并。依赖 [`binary_search.hpp`](binary_search.md)。

## 接口说明

### 类模板与构造函数

```cpp
template <class V, auto op, auto e, class Index = long long>
class DynamicSegmentTree {
public:
    DynamicSegmentTree(Index low, Index high);
};
```

- **模板参数**：
  - `V`、`op`、`e`：与 [`SegmentTree`](segtree.md) 一致，分别表示幺半群值类型、满足结合律的二元合并函数 `V op(V, V)`（不要求交换律）以及单位元函数 `V e()`。
  - `Index`：值域下标类型，默认为 `long long`。
- `DynamicSegmentTree(Index low, Index high)`：指定所有树共享的左闭右开全局下标范围 **`[low, high)`**（要求 `low < high`）。内部使下标 `0` 作为空节点哨兵（其值为 `e()`）。
- **根节点表示**：每棵动态线段树由一个外部 `int root = 0;` 标识（初始为 `0` 表示整棵树所有位置均为单位元 `e()`）。同一个 `DynamicSegmentTree` 实例可同时管理多棵树（森林）。

### 单点修改、查询、树上二分与线段树合并

所有下标 `p` 与查询区间 `[l, r)` 均在 **`[low, high)`** 范围内：

```cpp
void set(int& root, Index p, V value);
void add(int& root, Index p, V value);
V get(int root, Index p);
V prod(int root, Index l, Index r);
V all_prod(int root);
template <class F> Index max_right(int root, Index l, F f);
template <class F> Index min_left(int root, Index r, F f);
int merge(int a, int b);
```

- `set(int& root, Index p, V value)`：将树 `root` 在位置 `p`（`low <= p < high`）的值设为 `value`。注意 **`root` 按引用传递**，若原先 `root == 0`，调用后 `root` 会被更新为新分配的根节点编号。
- `add(int& root, Index p, V value)`：等价于 `set(root, p, op(get(root, p), value))`，在位置 `p` 原值右侧复合 `value`。
- `get(int root, Index p)`：返回位置 `p` 的当前值（未访问过的位置返回 `e()`）。
- `prod(int root, Index l, Index r)`：返回半开区间 `[l, r)`（`low <= l <= r <= high`）的聚合值。
- `all_prod(int root)`：$O(1)$ 返回整棵树在 `[low, high)` 上的总聚合值。
- `max_right(int root, Index l, F f)` / `min_left(int root, Index r, F f)`：在线段树上二分查找满足 `f(prod(root, l, r)) == true` 的最大右界 `r` 或最小左界 `l`（要求 `f(e()) == true` 且谓词单调）。
- `merge(int a, int b)`：将同一节点池中的两棵树 `a` 和 `b` 原地合并，对应叶子节点的值按 `op(val_a, val_b)` 合并，返回合并后的根节点编号。**合并操作会破坏并复用 `a` 和 `b` 的原有节点，合并后切勿再单独使用旧的 `b`（且调用前 `a` 与 `b` 不能共享任何节点）**。

## 复杂度

- 令 $L = \text{high} - \text{low}$ 为值域跨度：
- 单次 `set`、`add`、`get`、`prod`、`max_right`、`min_left`：时间复杂度 $O(\log L)$，每次修改最多新增 $O(\log L)$ 个节点。
- `merge`：将 $k$ 棵总计做过 $m$ 次单点插入的树两两合并，总时间复杂度均摊 $O(m \log L)$。
- 空间复杂度：$O(q_{\text{modify}} \log L)$。

## 使用示例

```cpp
#include "dynamic_segtree.hpp"
#include <cassert>

long long op(long long a, long long b) { return a + b; }
long long e() { return 0LL; }

int main() {
    DynamicSegmentTree<long long, op, e> dst(0, 1000000000000LL);
    int root1 = 0, root2 = 0;

    dst.add(root1, 10, 5);
    dst.add(root1, 1000000000LL, 7);
    dst.add(root2, 10, 3);

    assert(dst.prod(root1, 0, 100) == 5);
    assert(dst.all_prod(root1) == 12);

    root1 = dst.merge(root1, root2);
    assert(dst.get(root1, 10) == 8);
    assert(dst.all_prod(root1) == 15);
    return 0;
}
```

## 相关文档

- [SegmentTree](segtree.md) —— 紧凑下标范围上的静态线段树
- [PersistentSegmentTree](persistent_segtree.md) —— 保留历史版本的可持久化线段树
