# Persistent Segment Tree (`persistent_segtree.hpp`)

`persistent_segtree.hpp` 实现基于路径复制的可持久化线段树（函数式线段树 / 主席树）`PersistentSegmentTree<V, op, e>`，每次单点修改仅新建 $O(\log n)$ 个节点并返回新版本根编号，支持对任意历史版本执行区间查询、树上二分，以及基于两版本差分的静态区间第 $k$ 小查询。依赖 [`types.hpp`](types.md)。

## 接口说明

### 类模板与构造函数

```cpp
template <class V, auto op, auto e>
class PersistentSegmentTree {
public:
    PersistentSegmentTree(int n);
    int build(const std::vector<V>& init);
};
```

- **模板参数**：`V` 为节点聚合值类型，`V op(V, V)` 为满足结合律的二元合并函数，`V e()` 返回单位元。
- `PersistentSegmentTree(int n)`：指定底层下标范围为 **`0-based` 左闭右开 `[0, n)`**（**要求 $n > 0$**）。内部以节点编号 `0` 作为全 `e()` 的隐式空树版本，即使不调用 `build`，也可直接从 `root = 0` 开始执行 `set` 或 `add` 生成新版本。
- `build(const std::vector<V>& init)`：用长度为 $n$ 的初始数组 `init` 建出完整初始树，返回该初始版本的根节点编号。

### 版本修改、查询、树上二分与第 $k$ 小

所有下标 `p` 与查询区间 `[l, r)` 均在 **`[0, n)`** 范围内：

```cpp
int set(int root, int p, V value);
int add(int root, int p, V value);
V get(int root, int p);
V prod(int root, int l, int r);
V all_prod(int root);
template <class F> int max_right(int root, int l, F f);
template <class F> int min_left(int root, int r, F f);
int kth(int left_root, int right_root, i64 k);
```

- `set(int root, int p, V value)`：基于版本 `root` 复制路径，将位置 `p`（$0 \le p < n$）的值设为 `value`，**返回新版本的根节点编号**（原版本 `root` 保持不变）。
- `add(int root, int p, V value)`：等价于 `set(root, p, op(get(root, p), value))`，在位置 `p` 原值基础上复合 `value` 并返回新版本根编号。
- `get(int root, int p)` / `prod(int root, int l, int r)` / `all_prod(int root)`：在指定版本 `root` 上查询单点 `p`、半开区间 `[l, r)`（$0 \le l \le r \le n$）或全区间 `[0, n)` 的聚合值。
- `max_right(int root, int l, F f)` / `min_left(int root, int r, F f)`：在指定版本 `root` 上二分查找满足 `f(prod(root, l, r)) == true` 的最远右界 `r` 或最远左界 `l`（要求 `f(e()) == true` 且谓词单调）。
- `kth(int left_root, int right_root, i64 k)`：**专用于权值线段树（节点维护非负频数和）的两版本作差查询**。在 `right_root` 减去 `left_root` 的差分频数分布上，查找 **`0-based` 排名为 `k`** 的值域下标（即最小的位置 $p \in [0, n)$ 使得差分前缀和 $> k$）。调用前必须保证 $0 \le k < \text{tree[right\_root].value} - \text{tree[left\_root].value}$，且 `V` 支持减法并可转换为 `i64`。

## 复杂度

- `build`：时间与空间复杂度均为 $O(n)$。
- 单次 `set`、`add`：时间和新增空间复杂度均为 $O(\log n)$。
- 单次 `get`、`prod`、`max_right`、`min_left`、`kth`：时间复杂度 $O(\log n)$，额外空间 $O(1)$。

## 使用示例

```cpp
#include "persistent_segtree.hpp"
#include <cassert>
#include <vector>

long long add_op(long long a, long long b) { return a + b; }
long long zero_e() { return 0LL; }

int main() {
    // 权值范围 [0, 10)
    PersistentSegmentTree<long long, add_op, zero_e> pst(10);
    std::vector<int> roots = {0}; // roots[0] = 0 表示空前缀版本

    std::vector<int> a = {5, 1, 7, 1, 3};
    for (int x : a) {
        roots.push_back(pst.add(roots.back(), x, 1));
    }

    // 查询原数组区间 [1, 4) 即 {1, 7, 1} 中的 0-based 第 k 小:
    assert(pst.kth(roots[1], roots[4], 0) == 1);
    assert(pst.kth(roots[1], roots[4], 1) == 1);
    assert(pst.kth(roots[1], roots[4], 2) == 7);
    return 0;
}
```

## 相关文档

- [SegmentTree](segtree.md) —— 静态非可持久化线段树
- [DynamicSegmentTree](dynamic_segtree.md) —— 大值域动态开点线段树
- [WaveletTree](wavelet_tree.md) —— 常数更小的静态区间第 $k$ 小与频率查询结构
