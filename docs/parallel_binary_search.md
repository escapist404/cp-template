# ParallelBinarySearch

批量求解共享答案域上的单调判定问题。

## 接口

```cpp
template <typename Query, typename Integer, typename Reset,
          typename Advance, typename Check>
std::vector<Integer> parallel_binary_search(
    const std::vector<Query>& queries, Integer low, Integer high,
    Reset reset, Advance advance, Check check);
```

搜索半开答案域 `[low, high)`；`high` 可作为“无解”哨兵。每轮先调用 `reset()`，再按中点递增调用 `advance(mid)`，然后调用 `check(query, mid)`。`check` 返回 `true` 时收缩右端点，否则令左端点为 `mid+1`。

## 使用示例

```cpp
#include "parallel_binary_search.hpp"
#include "types.hpp"
#include <iostream>
#include <vector>

struct Query { i64 threshold; };

int main() {
    std::vector<Query> queries = {{50}, {30}, {70}};
    i64 current = 0;
    auto reset = [&] { current = 0; };
    auto advance = [&](i64 x) { current = x; };
    auto check = [&](const Query& q, i64) { return current >= q.threshold; };
    auto answer = parallel_binary_search(queries, (i64)0, (i64)101,
                                         reset, advance, check);
    std::cout << answer[0] << '\n';
}
```

复杂度约为 $O(q\log V\cdot(T_{reset}+T_{advance}+T_{check}))$。
