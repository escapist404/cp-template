# PersistentSegmentTree

维护数组的多个历史版本，支持在旧版本基础上修改并查询任意版本的区间聚合值。值域为 `[0,n)`，聚合类型为满足结合律的 `V`。

```cpp
template<class V, auto op, auto e>
class PersistentSegmentTree;
```

`build(init)` 创建版本，`set/add` 返回新根，旧版本保持不变；`prod(root,l,r)`、`get(root,p)`、`all_prod(root)` 查询半开区间；`max_right(root,l,f)` 和 `min_left(root,r,f)` 返回满足单调谓词的边界。根编号 `0` 表示全是 `e()` 的空版本，可直接作为 `add/set` 的起点。

`kth(left_root,right_root,k)` 是计数型加法树的专用接口：返回两个前缀版本差的第 `k` 个位置（0-indexed），要求节点值可相减且差值为非负计数。

每次修改新增 `O(log n)` 个节点，查询 `O(log n)`。
