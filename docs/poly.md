# Polygon

`poly.hpp` 提供多边形有向面积、面积和点在多边形内判断：

```cpp
Real polygon_signed_area(const std::vector<Point>& polygon);
Real polygon_area(const std::vector<Point>& polygon);
bool point_in_polygon(const std::vector<Point>& polygon, Point point);
```

`point_in_polygon` 包含边界。详细说明见 [geometry.md](geometry.md)。
