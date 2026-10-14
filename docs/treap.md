# Treap (`treap.hpp`)

`treap.hpp` 实现基于旋转/分裂合并的随机化平衡二叉搜索树 `Treap<T, Compare>`（有序多重集合），支持插入重复元素、单次删除一个副本、按值查排名、按排名查值以及严格前驱与后继查询。

## 接口说明

### 类模板与构造

```cpp
template <typename T = int, typename Compare = std::less<T>>
class Treap {
public:
    Treap();
    Treap(const Compare& compare);
    Treap(const Treap&) = delete;
    Treap& operator=(const Treap&) = delete;
    ~Treap();
};
```

- `T`：元素类型，默认 `int`。由于 `get_prev` 和 `get_next` 使用 `std::numeric_limits<T>::lowest()` 与 `std::numeric_limits<T>::max()` 作为不存在时的返回值，若使用这两个接口，`T` 应当特化了 `std::numeric_limits`（如标准算术类型）。
- `Compare`：严格弱序比较器，默认 `std::less<T>`（升序排列）。内部通过 `!compare(a, b) && !compare(b, a)` 判定两元素相等并合并计数 `count`。
- 树节点通过 `new` 动态分配并在析构函数中递归释放，因此**禁用了拷贝构造与拷贝赋值**；传递 `Treap` 时请使用引用或指针。

### 成员函数

```cpp
void insert(const T& key);
void remove(const T& key);
int get_rank(const T& key) const;
T get_value(int rank) const;
T get_prev(const T& key) const;
T get_next(const T& key) const;
Compare key_comp() const;
```

- `insert(key)`：向多重集中插入一个 `key`。若 `key` 已存在则将对应节点的 `count` 加 $1$。
- `remove(key)`：从多重集中删除**一个**等于 `key` 的元素（若 `count > 1` 则仅减 $1$；若 `count == 1` 则移除节点；若 `key` 不存在则无操作）。
- `get_rank(key)`：返回 `key` 的 **`1-based` 排名**，定义为多重集中严格排在 `key` 前面（即满足 `compare(x, key)`）的元素个数加 $1$。即使 `key` 未插入树中，也可正常返回其假想插入位置的排名。
- `get_value(rank)`：返回 **`1-based` 排名**为 `rank` 的元素值。**调用者必须保证 $1 \le \text{rank} \le \text{总元素个数}$**，若越界将解引用空指针导致未定义行为。
- `get_prev(key)`：返回严格小于 `key`（即满足 `compare(x, key)`）的最后一个元素；若不存在则返回 `std::numeric_limits<T>::lowest()`。
- `get_next(key)`：返回严格大于 `key`（即满足 `compare(key, x)`）的第一个元素；若不存在则返回 `std::numeric_limits<T>::max()`。
- `key_comp()`：返回当前使用的比较器副本。

## 复杂度

- 单次 `insert`、`remove`、`get_rank`、`get_value`、`get_prev`、`get_next`：期望时间复杂度均为 $O(\log m)$，其中 $m$ 为树中不同键值的个数。
- 空间复杂度：$O(m)$，相同键值共享同一个节点。

## 使用示例

```cpp
#include "treap.hpp"
#include <cassert>
#include <limits>

int main() {
    Treap<int> tr;
    tr.insert(10);
    tr.insert(20);
    tr.insert(20);
    tr.insert(30);

    assert(tr.get_rank(20) == 2);
    assert(tr.get_value(1) == 10);
    assert(tr.get_value(2) == 20);
    assert(tr.get_value(3) == 20);
    assert(tr.get_value(4) == 30);

    assert(tr.get_prev(20) == 10);
    assert(tr.get_next(20) == 30);
    assert(tr.get_prev(10) == std::numeric_limits<int>::lowest());

    tr.remove(20); // 仅删除一个 20，还剩一个 20
    assert(tr.get_value(2) == 20 && tr.get_value(3) == 30);
    return 0;
}
```

## 相关文档

- [ImplicitTreap](implicit_treap.md) —— 维护序列下标、支持区间翻转与移动的无旋 Treap
- [ScapegoatTree](scapegoat.md) —— 基于重量平衡重构的替罪羊树
