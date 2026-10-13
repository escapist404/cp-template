# RollbackDSU

并查集支持保存快照并回滚到旧版本。实现按大小合并，不使用路径压缩。

## 接口

```cpp
RollbackDSU(int n);
int get(int x) const;
bool merge(int a, int b);
int snapshot() const;
void rollback(int version);
int component_count() const;
int component_size(int x) const;
```

`merge` 成功时记录一条历史；失败时不记录。`rollback(version)` 将历史栈恢复到指定长度。

## 使用示例

```cpp
#include "rollback_dsu.hpp"
#include <iostream>

int main() {
    RollbackDSU dsu(5);
    dsu.merge(0, 1);
    int snap = dsu.snapshot();
    dsu.merge(1, 2);
    std::cout << dsu.component_size(0) << '\n';
    dsu.rollback(snap);
    std::cout << dsu.component_count() << '\n';
}
```

`get` 和 `merge` 为 $O(\log n)$，快照为 $O(1)$，回滚复杂度为撤销操作数。
