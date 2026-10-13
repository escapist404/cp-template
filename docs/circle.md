# Circle

`circle.hpp` 提供直线与圆、圆与圆的交点计算：

```cpp
std::vector<Point> line_circle_intersection(const Line& line, const Circle& circle);
std::vector<Point> circle_circle_intersection(const Circle& a, const Circle& b);
```

详细返回值约定见 [geometry.md](geometry.md)。
