# MergeSort

`merge_sort` 对数组做稳定的归并排序。实现只负责排序，不返回逆序对数量。

## 接口

```cpp
template <typename T>
void merge_sort(std::vector<T>& arr, int l, int r);
```

排序半开区间 `[l, r)`；调用者应传入 `0` 和 `arr.size()`。

## 使用示例

```cpp
#include "merge_sort.hpp"
#include <iostream>
#include <vector>

int main() {
    std::vector<int> a = {3, 1, 4, 1, 5};
    merge_sort(a, 0, a.size());
    for (int x : a) std::cout << x << ' ';
}
```

复杂度为 $O(n\log n)$，相等元素保持原有相对顺序。
