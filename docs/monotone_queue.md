# Monotone Queue (`monotone_queue.hpp`)

`monotone_queue.hpp` 提供维护带下标单调双端队列的类 `MonotoneQueue<T, Compare>` 以及一键计算固定宽度滑动窗口极值的函数 `sliding_window`，常用于滑动窗口最值统计与单调队列优化 DP。

## 接口说明

### 单调队列类 `MonotoneQueue`

```cpp
template <typename T, typename Compare = std::less<T>>
class MonotoneQueue {
public:
    MonotoneQueue(Compare compare = Compare{});
    void clear();
    bool empty() const;
    int size() const;
    void push(int index, T value);
    void pop_before(int first_index);
    int front_index() const;
    const T& front() const;
};
```

- **比较器方向约定（与 `std::priority_queue` 相同）**：
  - 在 `push(index, value)` 时，队尾所有满足 `compare(back.value, value)` 的旧元素都会被淘汰弹出；
  - 因此默认的 **`Compare = std::less<T>` 维护的是单调递减队列，队首 `front()` 为窗口最大值**；
  - 若需要维护**窗口最小值**，请显式传入 **`std::greater<T>`**。
- `push(int index, T value)`：向队尾加入下标为 `index`、权值为 `value` 的元素（调用者应按单调递增顺序传入 `index`）。注意淘汰条件为严格比较 `compare(back.value, value)`，若新元素与队尾相等，两者都会保留，意味着新元素可延長该极值的存活时间。
- `pop_before(int first_index)`：从队首不断弹出所有满足 **`index < first_index`** 的过期元素，使队列中仅保留下标 $\ge \text{first\_index}$ 的合法元素。
- `front() const` / `front_index() const`：返回当前极值本身或其对应的下标 `index`。**调用前必须保证 `!empty()`**（空队列调用会触发 `assert`）。
- `clear()` / `empty() const` / `size() const`：清空队列、判空或返回当前队列中保留的候选元素个数。

### 滑动窗口辅助函数 `sliding_window`

```cpp
template <typename T, typename Compare = std::less<T>>
std::vector<T> sliding_window(
    const std::vector<T>& values, int width,
    Compare compare = Compare{});
```

- **参数与返回值**：要求窗口宽度 `width > 0`。对长度为 $n$ 的数组 `values`，依次计算所有长度为 `width` 的连续子窗口 `[i - width + 1, i]`（$i = \text{width} - 1, \dots, n - 1$）的极值并返回。
- 当 $n \ge \text{width}$ 时，返回向量长度为 $n - \text{width} + 1$；当 $n < \text{width}$ 时返回空向量 `{}`。

## 复杂度

- `MonotoneQueue`：每个元素最多入队、出队各一次，$n$ 次 `push` 与 `pop_before` 总时间复杂度为严格线性 $O(n)$，单次操作均摊 $O(1)$；`front`、`front_index`、`empty`、`size` 为严格 $O(1)$。
- `sliding_window`：时间复杂度 $O(n)$，空间复杂度 $O(n)$。

## 使用示例

```cpp
#include "monotone_queue.hpp"
#include <cassert>
#include <functional>
#include <vector>

int main() {
    std::vector<int> a = {1, 3, -1, -3, 5, 3, 6, 7};

    // 窗口宽度为 3 的滑动窗口最大值（默认 std::less）
    auto max_win = sliding_window(a, 3);
    assert((max_win == std::vector<int>{3, 3, 5, 5, 6, 7}));

    // 窗口宽度为 3 的滑动窗口最小值（传入 std::greater）
    auto min_win = sliding_window(a, 3, std::greater<int>{});
    assert((min_win == std::vector<int>{-1, -3, -3, -3, 3, 3}));
    return 0;
}
```

## 相关文档

- [SparseTable](sparse_table.md) —— 任意长度静态区间的 $O(1)$ 最值查询
- [CartesianTree](cartesian_tree.md) —— 基于单调栈构造的笛卡尔树
