# WaveletTree

维护不可修改的序列，支持区间第 `k` 小、区间小于某值的数量，以及半开值域区间计数。

## 接口

```cpp
template <typename T, typename Compare = std::less<T>>
class WaveletTree;

WaveletTree(const std::vector<T>& values, Compare compare = Compare{});
T kth(int l, int r, int k) const;                         // [l,r)，k 从 0 开始
int count_less(int l, int r, const T& value) const;
int count(int l, int r, const T& low, const T& high) const; // 值域 [low,high)
```

## 使用示例

```cpp
#include "wavelet_tree.hpp"
#include <iostream>
#include <vector>

int main() {
    std::vector<int> data = {3, 1, 4, 1, 5, 9, 2, 6};
    WaveletTree<int> wt(data);
    std::cout << wt.kth(0, 8, 3) << '\n';       // 3
    std::cout << wt.count(2, 6, 1, 5) << '\n';   // 3
    std::cout << wt.count_less(0, 8, 4) << '\n'; // 4
}
```

构造复杂度和空间复杂度为 $O(n\log\sigma)$；查询复杂度为 $O(\log\sigma)$。序列必须非空，查询区间必须满足 `0 <= l < r <= n`。
