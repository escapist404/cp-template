# Mo

离线维护静态数组区间询问。所有区间使用半开表示 `[l,r)`。

## 接口

```cpp
template <typename Query, typename State, typename Add,
          typename Remove, typename Answer>
auto mo_algorithm(std::vector<Query> queries, int n, State& state,
                  Add add, Remove remove, Answer answer, int block = 0);
```

`Query` 必须包含 `l`、`r`、`id`；`id` 是 `[0, queries.size())` 的排列。回调形式为 `add(index, state)`、`remove(index, state)`、`answer(state, query)`。返回值类型由 `answer` 推导。

## 使用示例

```cpp
#include "mo.hpp"
#include <iostream>
#include <vector>

struct Query { int l, r, id; };

int main() {
    std::vector<int> a = {3, 1, 3, 2, 1};
    std::vector<Query> queries = {{0, 3, 0}, {1, 5, 1}};
    std::vector<int> count(4), state;
    int distinct = 0;
    auto add = [&](int i, auto&) { if (count[a[i]]++ == 0) ++distinct; };
    auto remove = [&](int i, auto&) { if (--count[a[i]] == 0) --distinct; };
    auto answer = [&](auto&, const Query&) { return distinct; };
    auto result = mo_algorithm(queries, a.size(), state, add, remove, answer);
    std::cout << result[0] << ' ' << result[1] << '\n';
}
```

带修版本使用 `mo_with_modify`，额外提供 `apply`、`rollback` 回调和带 `time` 字段的查询类型。复杂度取决于块大小，默认静态莫队块长为 `sqrt(n)`。
