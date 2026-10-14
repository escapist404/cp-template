# Coordinate Compression (`coordinate_compression.hpp`)

`coordinate_compression.hpp` 提供离散化（坐标压缩）函数 `coordinate_compress`，将大值域或任意可比较类型的数组映射为从 `0` 开始的紧凑整数排名，同时返回去重后的升序原值表以便反查。

## 接口说明

```cpp
template <typename T>
std::pair<std::vector<T>, std::vector<int>> coordinate_compress(
    const std::vector<T>& values);
```

- **参数 constraints**：元素类型 `T` 必须支持严格弱序比较 `operator<` 和相等判断 `operator==`。输入向量 `values` 可以为空。
- **返回值**：返回一个 `std::pair<std::vector<T>, std::vector<int>>`（通常用结构化绑定 `auto [unique_values, index] = coordinate_compress(values);` 接收）：
  1. `unique_values`（`first`）：`values` 排序并去重后的升序数组，长度为不同元素的个数 $m$（$0 \le m \le n$）。
  2. `index`（`second`）：与 `values` 等长的 `0-based` 离散化下标数组，满足 `0 <= index[i] < unique_values.size()` 且 `unique_values[index[i]] == values[i]`。相等的元素会映射到同一个下标，严格保持原有的大小关系。
- 若需将某个未出现在原数组中的查询值 $x$ 映射到同一离散化空间，可直接在返回的 `unique_values` 上调用 `std::lower_bound(unique_values.begin(), unique_values.end(), x) - unique_values.begin()`。

## 复杂度

- 时间复杂度：$O(n \log n)$，其中 $n = \text{values.size()}$。
- 空间复杂度：$O(n)$，用于存储去重值表与映射下标。

## 使用示例

```cpp
#include "coordinate_compression.hpp"
#include <cassert>
#include <vector>

int main() {
    std::vector<long long> a = {1000, -50, 1000, 42, -50};
    auto [vals, id] = coordinate_compress(a);

    // vals = {-50, 42, 1000}
    assert(vals.size() == 3);
    // id   = {2, 0, 2, 1, 0}
    assert((id == std::vector<int>{2, 0, 2, 1, 0}));
    for (size_t i = 0; i < a.size(); ++i) {
        assert(vals[id[i]] == a[i]);
    }
    return 0;
}
```

## 相关文档

- [Fenwick](fenwick.md) —— 常配合离散化统计逆序对或偏序
- [WaveletTree](wavelet_tree.md) —— 静态区间第 $k$ 小与频率查询
