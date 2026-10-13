# CDQ

将数组递归分成左右两半，在递归过程中处理跨半区贡献。

## 接口

```cpp
template <typename T, typename Merge, typename Leaf>
void cdq_divide(std::vector<T>& items, Merge merge, Leaf leaf);

template <typename T, typename Cross, typename Leaf>
void cdq_dp(std::vector<T>& items, Cross cross, Leaf leaf);
```

`merge(items, l, m, r)` 或 `cross(items, l, m, r)` 处理半开区间 `[l,m)` 到 `[m,r)` 的贡献；`leaf(items, i)` 处理单元素区间。

## 使用示例

```cpp
#include "cdq.hpp"
#include "fenwick.hpp"
#include <vector>

struct Point { int x, y, z, answer = 0; };

int main() {
    std::vector<Point> points(5);
    Fenwick<int> fenwick(10);
    auto merge = [&](auto& a, int l, int m, int r) {
        (void)a; (void)l; (void)m; (void)r;
        // 在这里按 y 排序并用 fenwick 统计跨半区贡献。
    };
    auto leaf = [](auto&, int) {};
    cdq_divide(points, merge, leaf);
}
```

实现会原地修改 `items`；树状数组等临时状态必须在每次 `merge/cross` 返回前清空。典型复杂度为 $O(n\log^2 n)$。
