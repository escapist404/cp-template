# Old Driver Tree / Chtholly Tree (`old_driver_tree.hpp`)

`old_driver_tree.hpp` 实现基于 `std::map<int, T>` 维护颜色段（值相同连续区间）的珂朵莉树 `OldDriverTree<T>`，支持将半开区间 `[l, r)` 推平为同一值以及单点值查询。

## 接口说明

```cpp
template <typename T>
class OldDriverTree {
public:
    OldDriverTree(const std::vector<T>& arr);
    auto split(int pos);
    void set(int l, int r, T v);
    T get(int pos);
};
```

- **索引与区间约定**：下标采用 **`0-based`**，区间修改 `set(l, r, v)` 采用**左闭右开 `[l, r)`**。
- `OldDriverTree(const std::vector<T>& arr)`：用长度为 $n = \text{arr.size()}$ 的初始数组构造颜色段映射，并在右端点位置 `n` 插入哨兵节点 `map[n] = T()`。因此所有操作下标必须落在 $0 \le l \le r \le n$ 及 $0 \le \text{pos} < n$ 范围内。
- `split(int pos)`：在位置 `pos`（$0 \le \text{pos} \le n$）处将原本覆盖 `pos` 的颜色段切开为 `[left, pos)` 和 `[pos, right)` 两段，并返回指向以 `pos` 为左端点的 `map` 迭代器。
- `set(int l, int r, T v)`：将半开区间 **`[l, r)`**（$0 \le l < r \le n$）内的所有元素赋值为 `v`。内部先调用 `split(r)` 再调用 `split(l)`（注意顺序不可颠倒，否则 `split(r)` 可能使 `split(l)` 返回的迭代器失效），擦除 `[l, r)` 之间的所有旧段后插入 `map[l] = v`。注意当 `l == r` 时不应调用 `set(l, l, v)`，否则会覆盖 `map[l]` 的原有颜色段。
- `get(int pos)`：通过 `--map.upper_bound(pos)` 查找覆盖下标 `pos`（$0 \le \text{pos} < n$）的颜色段并返回其值。

## 复杂度

- 构造：时间复杂度 $O(n \log n)$，空间复杂度 $O(n)$。
- `get(pos)` / `split(pos)`：单次 $O(\log m)$，其中 $m$ 为当前 `map` 中的颜色段数。
- `set(l, r, v)`：由于每次 `set` 至多增加 $2$ 个颜色段，而每个被 `erase` 的颜色段只会被删除一次，因此在任意数据下，若仅执行 $q$ 次 `set` 和 `get`，**总时间复杂度均摊为 $O((n + q) \log n)$**（若在外部遍历 `split(l)` 到 `split(r)` 之间的迭代器做暴力区间统计，则需依赖随机数据保证段数期望为 $O(\log n)$）。

## 使用示例

```cpp
#include "old_driver_tree.hpp"
#include <cassert>
#include <vector>

int main() {
    OldDriverTree<int> odt({1, 2, 3, 4, 5});

    odt.set(1, 4, 9); // 将 [1, 4) 赋值为 9 -> {1, 9, 9, 9, 5}
    assert(odt.get(0) == 1);
    assert(odt.get(1) == 9);
    assert(odt.get(3) == 9);
    assert(odt.get(4) == 5);
    return 0;
}
```

## 相关文档

- [LazySegmentTree](lazy_segtree.md) —— 在任意非随机数据下均严格保证 $O(\log n)$ 的区间修改结构
