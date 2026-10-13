# MinimumEnclosingCircle

```cpp
Circle circle = minimum_enclosing_circle(points);
```

随机增量算法求平面点集的最小覆盖圆，期望复杂度 $O(n)$，最坏复杂度 $O(n^3)$。点和圆使用 `geometry.hpp` 中的 `Point`、`Circle` 类型。

空点集返回半径为 0、圆心为原点的圆。浮点判断使用全局 `EPS`。
