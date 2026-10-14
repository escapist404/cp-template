# Leftist Heap (`leftist_heap.hpp`)

`leftist_heap.hpp` 实现可合并的左偏树（可并堆）`LeftistHeap<T, Compare>`，支持 $O(\log n)$ 插入元素、弹出堆顶以及将另一个同类型堆原地合并到当前堆中。

## 接口说明

### 类模板与构造函数

```cpp
template <typename T = int, typename Compare = std::less<T>>
class LeftistHeap {
public:
    LeftistHeap();
    LeftistHeap(const Compare& compare);
    LeftistHeap(const LeftistHeap&) = delete;
    LeftistHeap& operator=(const LeftistHeap&) = delete;
    ~LeftistHeap();
};
```

- **比较器方向约定（与 `std::priority_queue` 相反）**：
  - `Compare(a, b)` 为 `true` 表示 **`a` 比 `b` 更优先（更靠近堆顶）**；
  - 因此默认的 **`Compare = std::less<T>` 构造的是小根堆**（最小值在 `top()`）；若需要**大根堆**（最大值在 `top()`），请显式指定 `LeftistHeap<T, std::greater<T>>`。
- 节点通过 `new` 动态分配并在 `pop()` 和析构函数中释放，因此**禁用了拷贝构造与拷贝赋值**。

### 成员函数

```cpp
bool empty() const;
int size() const;
const T& top() const;
void push(const T& key);
void pop();
void merge(LeftistHeap& other);
```

- `empty() const` / `size() const`：判断堆是否为空，或返回当前堆中的元素个数。
- `top() const`：返回堆顶元素的常引用。**调用前必须保证 `!empty()`**，对空堆调用将解引用空指针。
- `push(const T& key)`：向堆中插入一个元素 `key`（支持重复键值）。
- `pop()`：移除并释放堆顶节点。**调用前必须保证 `!empty()`**。
- `merge(LeftistHeap& other)`：将另一个左偏树 `other` 的所有节点合并进当前堆，并**将 `other` 清空**（`other.empty() == true, other.size() == 0`）。若传入自身 `h.merge(h)` 则安全无操作。

## 复杂度

- `empty`、`size`、`top`：$O(1)$。
- `push`、`pop`、`merge`：最坏时间复杂度均为 $O(\log n)$，其中 $n$ 为参与合并的两堆大小之和。
- 空间复杂度：$O(n)$。

## 使用示例

```cpp
#include "leftist_heap.hpp"
#include <cassert>
#include <functional>

int main() {
    // 默认小根堆
    LeftistHeap<int> h1, h2;
    h1.push(5);
    h1.push(2);
    h2.push(4);
    h2.push(1);

    h1.merge(h2);
    assert(h2.empty());
    assert(h1.size() == 4);
    assert(h1.top() == 1);

    h1.pop();
    assert(h1.top() == 2);

    // 大根堆使用 std::greater
    LeftistHeap<int, std::greater<int>> max_heap;
    max_heap.push(3);
    max_heap.push(9);
    assert(max_heap.top() == 9);
    return 0;
}
```

## 相关文档

- [CartesianTree](cartesian_tree.md) —— 静态序列上的线性时间笛卡尔树构造
