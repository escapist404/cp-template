# Rotating Calipers

`calipers.hpp` 提供凸多边形直径：

```cpp
Real convex_polygon_diameter_squared(const std::vector<Point>& polygon);
```

返回最远点对距离的平方，要求输入为凸多边形，复杂度为 $O(n)$。详细说明见 [geometry.md](geometry.md)。
