# Rollback DSU (`rollback_dsu.hpp`)

`rollback_dsu.hpp` 实现仅按大小合并（不使用路径压缩）的可撤销并查集 `RollbackDSU`，支持 $O(1)$ 记录历史版本快照（`snapshot`）并将并查集状态精确回滚（`rollback`）到任意先前的快照点，常用于线段树分治（离线动态图连通性）、整体二分及回溯搜索。

## 接口说明

```cpp
class RollbackDSU {
public:
    RollbackDSU(int n);
    int get(int x) const;
    bool merge(int a, int b);
    int snapshot() const;
    void rollback(int version);
    int component_count() const;
    int component_size(int x) const;
};
```

- **索引约定**：所有顶点编号均为 **`0-based`**，合法范围为 $0 \le x < n$（越界会触发 `assert`）。
- `RollbackDSU(int n)`：初始化 $n$ 个独立顶点，初始连通分量数为 $n$，每个分量大小为 $1$。
- `get(int x) const`：沿父指针向上查找并返回 `x` 所在集合的根节点（为支持回滚，不修改父指针，因此是 `const` 方法）。
- `merge(int a, int b)`：合并 `a` 与 `b` 所在的集合：
  - 若 `a` 与 `b` 已在同一连通分量，则**不压入历史栈**并返回 `false`；
  - 若不在同一连通分量，则将较小集合的根挂到较大集合的根下，将本次修改记录压入 `history` 栈，连通分量数减 $1$，并返回 `true`。
- `snapshot() const`：返回当前历史操作栈的大小 `version`，作为后续 `rollback(version)` 的回滚锚点。
- `rollback(int version)`：要求 $0 \le \text{version} \le \text{snapshot()}$。不断弹出并撤销 `history` 栈顶的合并记录，直到历史栈大小恢复为 `version`。
- `component_count() const`：返回当前连通分量总数。
- `component_size(int x) const`：返回顶点 `x` 所在连通分量的大小。

## 复杂度

- 构造：时间与空间复杂度均为 $O(n)$。
- `get`、`merge`、`component_size`：单次严格 $O(\log n)$。
- `snapshot`、`component_count`：$O(1)$。
- `rollback(version)`：$O(\Delta)$，其中 $\Delta$ 为从 `version` 之后实际发生有效合并的次数。

## 使用示例

```cpp
#include "rollback_dsu.hpp"
#include <cassert>

int main() {
    RollbackDSU dsu(4);
    dsu.merge(0, 1);
    assert(dsu.component_count() == 3);

    int ver = dsu.snapshot();
    dsu.merge(1, 2);
    dsu.merge(2, 3);
    assert(dsu.component_count() == 1);
    assert(dsu.component_size(0) == 4);

    // 一键回滚到 ver 状态
    dsu.rollback(ver);
    assert(dsu.component_count() == 3);
    assert(dsu.get(0) == dsu.get(1));
    assert(dsu.get(1) != dsu.get(2));
    return 0;
}
```

## 相关文档

- [DSU](dsu.md) —— 含路径压缩、均摊 $O(\alpha(n))$ 的常规并查集
- [OverallBinarySearch](overall_binary_search.md) —— 常配合可回滚数据结构使用的整体二分框架
