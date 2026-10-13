# Treap

FHQ Treap，带重复键的有序多重集。

## 模板参数

```cpp
template <typename T = int, typename Compare = std::less<T>>
class Treap;
```

源码注释：`// FHQ Treap with duplicate keys.`

## 构造与拷贝

```cpp
Treap() = default;
Treap(const Compare &compare);
```

**禁止拷贝**：拷贝构造与拷贝赋值均被 `= delete`。需要传递时用引用。

## 成员函数

```cpp
void insert(const T &key);
void remove(const T &key);

int get_rank(const T &key) const;      // 1-based
T   get_value(const int rank) const;   // rank 为 1-based
T   get_prev(const T &key) const;      // 严格小于 key 的最大值
T   get_next(const T &key) const;      // 严格大于 key 的最小值
Compare key_comp() const;
```

### 名次语义

- `get_rank` 与 `get_value` 都是 **1-based**，且统计的是**元素个数**（含重复）
- `get_rank` 对**不存在的键**返回「若插入应处的位置」，不报错、不返回 0
- `get_value(rank)` 越界时**无边界检查**，会解引用空指针

### 前驱与后继的哨兵

- `get_prev` 在不存在时返回 `std::numeric_limits<T>::lowest()`
- `get_next` 在不存在时返回 `std::numeric_limits<T>::max()`

**没有 `bool` 或 `std::optional` 返回**，调用者需自行判范围。这也意味着 `T` 必须支持 `numeric_limits`。

## 使用示例

```cpp
#include "treap.hpp"
#include <iostream>

int main() {
    Treap<int> treap;

    treap.insert(5);
    treap.insert(1);
    treap.insert(3);
    treap.insert(3);   // 重复键

    std::cout << treap.get_rank(3) << '\n';    // 1-based 名次
    std::cout << treap.get_value(1) << '\n';   // 最小值 1
    std::cout << treap.get_prev(3) << '\n';    // 1
    std::cout << treap.get_next(3) << '\n';    // 5

    treap.remove(3);   // 只减一个

    return 0;
}
```

### 当作有序集合用

```cpp
// 前驱判空
int prev = treap.get_prev(x);
if (prev != std::numeric_limits<int>::lowest()) {
    // 存在严格小于 x 的元素
}

// 后继判空
int next = treap.get_next(x);
if (next != std::numeric_limits<int>::max()) {
    // 存在严格大于 x 的元素
}
```

### 逆序（大顶）

```cpp
Treap<int, std::greater<int>> treap;
```

## 前置条件与实现约定

- **禁止拷贝**。`Treap<int> b = a;` 无法编译，函数返回 / 按值传参也不行，必须用引用或指针。
- `get_prev` / `get_next` 用 `numeric_limits` 哨兵表示「不存在」。当 `T` 为 `int` 且数据中真的含有 `INT_MIN` / `INT_MAX` 时，无法区分「不存在」与「找到了这个值」。数据范围可能触及类型边界时，改用其他容器或自行封装。
- `get_value(rank)` **无边界检查**，rank 越界会崩溃。
- `get_rank` 对不存在的键返回应插入位置，不会失败——若业务上需要「查不到就报错」，得自己再加一次查询。
- 优先级用 `std::uniform_int_distribution<int>`，可能出现相等；源码在 `priority > cur->priority` 时才分裂，相等不触发，treap 性质仍保持。
- 随机源来自 `std::random_device{}()`，**非确定性**。需要可复现的调试时注意。
- 析构是递归的，**深度很大时可能栈溢出**（源码未说明深度上界或迭代化处理）。
- 两个指针都是裸指针，**没有内存池**，频繁插入删除会有较多 `new` / `delete` 开销。

## 相关文档

- [ScapegoatTree](scapegoat.md) —— 无随机化的替代，同样带重复键
- [WaveletTree](wavelet_tree.md) —— 静态序列的 k 小查询，空间更省
- [PersistentSegmentTree](persistent_segtree.md) —— 区间第 k 小
- [LeftistHeap](leftist_heap.md) —— 只需堆操作时的可合并版本
