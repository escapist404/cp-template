# ConvexHullTrick

维护形如 `slope * x + intercept` 的若干函数，并回答指定 `x` 处的最小值查询。

## 接口

```cpp
ConvexHullTrick<long long> hull;
hull.add_line(slope, intercept);
long long answer = hull.query(x);
```

```cpp
void add_line(T slope, T intercept);
T query(T x) const;
```

要求插入直线的斜率非递减；查询的 `x` 可以任意顺序。插入均摊 $O(1)$，查询 $O(\log n)$。

默认求最小值。求最大值时插入 `(-slope, -intercept)`，再将查询结果取负。

相同斜率只保留截距较小的直线。

如果斜率不能单调加入，应使用 [LiChao](lichao.md)。
