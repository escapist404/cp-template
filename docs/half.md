# Half-Plane Intersection

`half.hpp` 提供半平面交：

```cpp
std::vector<Point> half_plane_intersection(std::vector<Line> lines);
```

每条有向直线保留左侧半平面；空集或退化交集返回空数组。详细说明见 [geometry.md](geometry.md)。
