# CustomHash

给 `unordered_map` / `unordered_set` 使用的随机化整数哈希，避免比赛数据针对默认哈希构造冲突。

```cpp
#include "hash.hpp"
std::unordered_map<long long, int, CustomHash> count;
std::unordered_set<int, CustomHash> seen;
```

支持可转换为 `uint64_t` 的整数类型和 `pair`。它只改变哈希分布，不改变 `unordered_map` 的平均复杂度保证；键类型复杂时应自行提供哈希函数。
