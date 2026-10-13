# cartesian_tree

给定一个序列和元素比较器，构造一棵中序遍历仍对应原序列、同时满足比较器堆序的树。

## 接口

```cpp
struct CartesianTreeResult {
    int root;
    std::vector<int> left, right, father;
};

template <typename T, typename Compare = std::less<T>>
CartesianTreeResult cartesian_tree(
    const std::vector<T>& values, Compare compare = Compare{});
```

下标与输入数组一致，空数组时 `root == -1`。相等元素保留较早下标作为较高节点。

**复杂度**：`O(n)` 时间，`O(n)` 空间。

```cpp
auto tree = cartesian_tree(std::vector<int>{3, 1, 2, 1});
// tree.root == 1
```
