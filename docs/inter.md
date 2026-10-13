# Geometry Intersections

`inter.hpp` 提供二维直线与线段关系、交点、点在线段上判断和线段相交判断：

```cpp
int line_relation(const Line& a, const Line& b);
Point line_intersection(const Line& a, const Line& b);
bool on_segment(Point point, const Segment& segment);
bool segment_intersect(const Segment& a, const Segment& b);
```

详细语义见 [geometry.md](geometry.md)。
