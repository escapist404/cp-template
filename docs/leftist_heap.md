# LeftistHeap

维护若干可合并的优先队列，支持插入、查询或删除最小值以及把两个队列合并。

依赖 `types.hpp`（`i64`）。

## 模板参数

```cpp
template <typename T, typename Compare = std::less<T>>
class LeftistHeap;
```

- `T`：元素类型
- `Compare`：比较器，默认 `std::less<T>`（小顶堆）

## 构造函数

```cpp
LeftistHeap() = default;
LeftistHeap(const Compare &compare);
```

**禁止拷贝**（拷贝构造与拷贝赋值均 `= delete`）。

## 成员函数

### push

```cpp
void push(const T &value);
```

插入一个元素。

**复杂度**：均摊 $O(\log n)$

### pop

```cpp
void pop();
```

删除最小（顶）元素。

**约束**
- 有 `assert(!empty())`

### top

```cpp
const T &top() const;
```

返回最小元素的常引用。

**约束**
- 有 `assert(!empty())`

### merge

```cpp
void merge(LeftistHeap &other);
```

把 `other` 合并到当前堆，**`other` 被清空**。

**复杂度**：$O(\log(n + m))$

### empty / size

```cpp
bool empty() const;
i64 size() const;
```

`size()` 返回元素个数（`i64` 类型）。

## 使用示例

```cpp
#include "leftist_heap.hpp"
#include <iostream>

int main() {
    LeftistHeap<int> heap;

    heap.push(5);
    heap.push(3);
    heap.push(7);

    std::cout << heap.top() << '\n';   // 3
    heap.pop();
    std::cout << heap.top() << '\n';   // 5

    return 0;
}
```

### 堆合并

```cpp
LeftistHeap<int> a, b;
a.push(1); a.push(3);
b.push(2); b.push(4);

a.merge(b);   // a 现在有全部 4 个元素，b 为空
// 此后 b 不可再使用（实际上可以，但已被清空）
```

### 大顶堆

```cpp
LeftistHeap<int, std::greater<int>> max_heap;
```

## 前置条件与实现约定

- **禁止拷贝**。`LeftistHeap<int> b = a;` 无法编译。
- `merge` 会**消费 `other`**：合并后 `other.root = nullptr`，变成空堆。这是标准的堆合并语义。
- `top()` 与 `pop()` 都有 `assert(!empty())`，空堆调用会崩溃。
- 析构是递归的（`delete root` 触发子节点的递归 `delete`），**深度很大时可能栈溢出**。
- 两个指针都是裸指针，**没有内存池**。
- 内部用 `dist` 维护左偏性质（左子树的 `dist` 不小于右子树）。

## 相关文档

- [Treap](treap.md) —— 有序多重集（也是基于堆性质）
- [ScapegoatTree](scapegoat.md) —— 无随机化的平衡树
- [SparseTable](sparse_table.md) —— 静态区间最值
