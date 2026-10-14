# Cartesian Tree (`cartesian_tree.hpp`)

`cartesian_tree.hpp` 使用单调栈在 $O(n)$ 时间内构造给定序列的笛卡尔树 `cartesian_tree`，使其树中序遍历严格对应原数组下标顺序，且父子权值关系满足由比较器 `Compare` 定义的堆性质。

## 接口说明

```cpp
struct CartesianTreeResult {
    int root = -1;
    std::vector<int> left, right, father;
};

template <typename T, typename Compare = std::less<T>>
CartesianTreeResult cartesian_tree(
    const std::vector<T>& values,
    Compare compare = Compare{});
```

- **堆序与相等元素稳定性约定**：
  - `Compare` 默认为 `std::less<T>`，对应**小根笛卡尔树**（父节点权值 $\le$ 子节点权值，子树根为对应区间内的最小值）；若传入 `std::greater<T>` 则构造**大根笛卡尔树**。
  - 当数组中存在相等元素（即 `compare(values[i], values[j])` 为 `false`）时，单调栈只在严格满足 `compare(values[index], values[stack.back()])` 时弹出栈顶，因此**下标靠前的相等元素恒作为下标靠后者的祖先**，保证树形态唯一且严格确定。
- **返回值 `CartesianTreeResult`**：
  - 所有节点编号均为 **`0-based` 原数组下标** $[0, n)$，空节点用 **`-1`** 表示；
  - `root`：整棵笛卡尔树的根节点下标（当 `values` 为空时返回 `-1`，树的根节点满足 `father[root] == -1`）；
  - `left[i]` / `right[i]` / `father[i]`：长度均为 $n$，分别表示节点 `i` 的左儿子、右儿子和父节点下标（不存在则为 `-1`）。
  - 在笛卡尔树中，节点 `i` 的子树所管辖的原数组连续极值区间可通过左右子树边界递推获得。

## 复杂度

- 时间复杂度：严格线性 $O(n)$（每个下标最多入栈、出栈各一次）。
- 空间复杂度：$O(n)$。

## 使用示例

```cpp
#include "cartesian_tree.hpp"
#include <cassert>
#include <vector>

int main() {
    std::vector<int> a = {3, 1, 4, 1, 5};
    auto tree = cartesian_tree(a); // 默认小根堆

    // 最小值为 a[1] = 1（与 a[3] = 1 相等，下标靠前的 1 作为整棵树的根）
    assert(tree.root == 1);
    assert(tree.father[1] == -1);
    assert(tree.left[1] == 0);   // 左儿子为 a[0] = 3
    assert(tree.right[1] == 3);  // 右儿子为 a[3] = 1
    assert(tree.left[3] == 2);   // a[3] 的左儿子为 a[2] = 4
    assert(tree.right[3] == 4);  // a[3] 的右儿子为 a[4] = 5
    return 0;
}
```

## 相关文档

- [SparseTable](sparse_table.md) —— 静态区间最值查询（RMQ）
- [MonotoneQueue](monotone_queue.md) —— 滑动窗口单调队列最值
