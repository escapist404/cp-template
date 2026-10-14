# Scapegoat Tree (`scapegoat.hpp`)

`scapegoat.hpp` 实现基于重量平衡因子 $\alpha = \frac{2}{3}$ 暴力拍平重构的替罪羊树 `ScapegoatTree<T, Compare>`（有序多重集合），支持插入重复元素、单次删除一个副本、集合大小查询、按值查排名、按排名查值以及严格前驱与后继查询。

## 接口说明

### 类模板与构造函数

```cpp
template <typename T = int, typename Compare = std::less<T>>
class ScapegoatTree {
public:
    ScapegoatTree();
    ScapegoatTree(Compare compare);
    ScapegoatTree(const ScapegoatTree&) = delete;
    ScapegoatTree& operator=(const ScapegoatTree&) = delete;
    ~ScapegoatTree();
};
```

- `T`：元素类型，默认 `int`。由于 `get_prev` 和 `get_next` 使用 `std::numeric_limits<T>::lowest()` 与 `std::numeric_limits<T>::max()` 作为不存在时的返回值，若调用这两个接口，`T` 需具有对应的 `std::numeric_limits` 特化。
- `Compare`：严格弱序比较器，默认 `std::less<T>`（升序）。相等元素共享同一个树节点并通过 `count` 计数。
- 树节点通过 `new` 动态分配并在析构函数中释放，因此**禁用了拷贝构造与拷贝赋值**。

### 成员函数

```cpp
void insert(T key);
void remove(T key);
int size() const;
bool empty() const;
int get_rank(T key) const;
T get_value(int rank) const;
T get_prev(T key) const;
T get_next(T key) const;
Compare key_comp() const;
```

- `insert(T key)`：向多重集中插入一个 `key`。若 `key` 是新键值且插入深度超过 $\lfloor \log_{3/2}(\text{max\_nodes}) \rfloor$，则沿插入路径自底向上找到第一个满足 `3 * child->nodes > 2 * parent->nodes` 的失衡祖先并将其拍平重构为完全平衡树。
- `remove(T key)`：从多重集中删除**一个**等于 `key` 的元素（若 `count > 1` 则仅减 $1$；若 `count == 1` 则真正移除树节点；若 `key` 不存在则无操作）。当树中剩余不同节点数低于历史峰值 `max_nodes` 的 $\frac{2}{3}$ 时，会对整棵树触发一次重构。
- `size() const` / `empty() const`：返回多重集中的**元素总数**（含重复元素的 `count` 累加）以及集合是否为空。
- `get_rank(T key) const`：返回 `key` 的 **`1-based` 排名**，即严格排在 `key` 前面（满足 `compare(x, key)`）的元素个数加 $1$（`key` 不必存在于树中）。
- `get_value(int rank) const`：返回 **`1-based` 排名**为 `rank` 的元素值。**调用者必须保证 $1 \le \text{rank} \le \text{size()}$**，越界访问将解引用空指针。
- `get_prev(T key) const`：返回严格小于 `key` 的最大元素；若不存在则返回 `std::numeric_limits<T>::lowest()`。
- `get_next(T key) const`：返回严格大于 `key` 的最小元素；若不存在则返回 `std::numeric_limits<T>::max()`。

## 复杂度

- `size`、`empty`：$O(1)$。
- `insert`、`remove`：均摊时间复杂度 $O(\log m)$，其中 $m$ 为不同键值的节点数（单次重构耗时 $O(\text{subtree\_nodes})$）。
- `get_rank`、`get_value`、`get_prev`、`get_next`：最坏时间复杂度 $O(\log m)$。
- 空间复杂度：$O(m)$。

## 使用示例

```cpp
#include "scapegoat.hpp"
#include <cassert>

int main() {
    ScapegoatTree<int> tree;
    tree.insert(15);
    tree.insert(10);
    tree.insert(20);
    tree.insert(10);

    assert(tree.size() == 4);
    assert(tree.get_rank(15) == 3);
    assert(tree.get_value(1) == 10);
    assert(tree.get_value(2) == 10);
    assert(tree.get_value(3) == 15);
    assert(tree.get_prev(15) == 10);
    assert(tree.get_next(15) == 20);

    tree.remove(10);
    assert(tree.size() == 3);
    assert(tree.get_rank(15) == 2);
    return 0;
}
```

## 相关文档

- [Treap](treap.md) —— 基于随机优先级的平衡二叉搜索树
- [ImplicitTreap](implicit_treap.md) —— 按序列位置分裂合并的无旋 Treap
