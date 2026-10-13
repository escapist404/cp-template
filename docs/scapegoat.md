# ScapegoatTree

维护一个支持重复键的有序多重集，提供排名、前驱、后继和按排名取值等查询。

## 模板参数

```cpp
template <typename T = int, typename Compare = std::less<T>>
class ScapegoatTree;
```

源码注释：

```
// Scapegoat tree (ordered multiset) with duplicate keys.
// All ordinary operations are amortized O(log n); rebuilding a subtree is
// linear in that subtree's size.
```

**平衡因子** `alpha = 2.0 / 3.0`，是 `static constexpr`，运行期不可配置。

## 构造与拷贝

```cpp
ScapegoatTree() = default;
ScapegoatTree(const Compare &compare);
```

**禁止拷贝**（拷贝构造与拷贝赋值均 `= delete`）。

## 成员函数

```cpp
void insert(const T &key);
void remove(const T &key);

int  size() const;      // 元素总数（含重复）
bool empty() const;

int get_rank(const T &key) const;      // 1-based
T   get_value(const int rank) const;   // 1-based
T   get_prev(const T &key) const;      // 哨兵 lowest()
T   get_next(const T &key) const;      // 哨兵 max()
Compare key_comp() const;
```

## 两个「大小」的区别

节点内部同时维护：

- `size`：**元素总数**（含重复）
- `nodes`：**节点个数**（重复键被压缩在同一节点）

因此 `size()` 返回的是元素数，与节点数不同。例如插入三次 `3`，`size() == 3`，而树中只有 1 个节点。

## 使用示例

```cpp
#include "scapegoat.hpp"
#include <iostream>

int main() {
    ScapegoatTree<int> tree;

    tree.insert(5);
    tree.insert(1);
    tree.insert(3);
    tree.insert(3);

    std::cout << tree.size() << '\n';          // 4（含重复）
    std::cout << tree.get_rank(3) << '\n';     // 1-based
    std::cout << tree.get_value(1) << '\n';    // 1
    std::cout << tree.get_prev(3) << '\n';     // 1
    std::cout << tree.get_next(3) << '\n';     // 5

    tree.remove(3);                             // 只减一个

    std::cout << tree.empty() << '\n';

    return 0;
}
```

### 在有序表上跑滑窗平衡

```cpp
// 维护一个可重复的有序多重集，求中位数
int median(ScapegoatTree<int> &tree) {
    int n = tree.size();
    if (n % 2 == 1) return tree.get_value(n / 2 + 1);
    return (tree.get_value(n / 2) + tree.get_value(n / 2 + 1)) / 2;
}
```

## 前置条件与实现约定

- **禁止拷贝**（与 [Treap](treap.md) 一致）。
- `get_prev` / `get_next` 用 `numeric_limits<T>::lowest()` / `::max()` 作哨兵，**没有额外的失败信号**，数据可能触及类型边界时无法区分「不存在」。
- `get_value(rank)` **无边界检查**，越界会解引用空指针。
- `alpha` 固定为 $2/3$，无法按题目调优。
- 重复键由 `count` 压缩，`size()` 计元素数、节点内部 `nodes` 计节点数，两者不同。
- 插入时若命中已有键（只增 `count`）**不触发重建**，因此纯重复插入不会失衡。
- 析构递归，深度大时可能栈溢出。
- `remove` 删除结点时用左子树最大值（前驱）替换，随后右子树指针重新接上。
- `max_nodes` 只增不减，直到触发整树重建才被重置。

## 与 Treap 的取舍

| | ScapegoatTree | Treap |
|---|---|---|
| 随机化 | 无 | 是 |
| 平衡方式 | 失衡时重建子树 | 随机优先级 |
| 最坏复杂度 | 摊还 $O(\log n)$ | 期望 $O(\log n)$ |
| 额外空间 | 无 | 每节点一个优先级 |

两者的公开接口**基本一致**（`insert` / `remove` / `get_rank` / `get_value` / `get_prev` / `get_next` / `key_comp`），可直接替换。区别是 `ScapegoatTree` 额外提供 `size()` 与 `empty()`。

## 相关文档

- [Treap](treap.md) —— 随机化版本
- [WaveletTree](wavelet_tree.md) —— 静态序列
- [LeftistHeap](leftist_heap.md) —— 可合并堆
