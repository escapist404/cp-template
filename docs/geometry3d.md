# Geometry3D

三维几何基础定义：三维向量、点、直线、平面、球，以及投影、距离和直线平面交点。

## 接口

```cpp
struct Vector3;
using Point3 = Vector3;
struct Line3;
struct Plane3;
struct Sphere3;

Real dot(Vector3 a, Vector3 b);
Vector3 cross(Vector3 a, Vector3 b);
Real distance(Point3 a, Point3 b);
Real distance(Point3 p, Line3 line);
Real distance(Point3 p, Plane3 plane);
bool line_plane_intersection(Line3 line, Plane3 plane, Point3& result);
```

`Plane3(a,b,c)` 使用三个不共线点构造平面；`Plane3(point,normal)` 会将法向量单位化。

## 复杂度

所有基本运算均为 $O(1)$。

## 使用示例

```cpp
#include "geometry3d.hpp"
#include <cassert>

int main() {
    Point3 a(0, 0, 0), b(1, 0, 0), c(0, 1, 0);
    assert(cross(b - a, c - a) == Vector3(0, 0, 1));
    Plane3 plane(a, b, c);
    Point3 hit;
    assert(line_plane_intersection(Line3(Point3(0, 0, 1), Point3(0, 0, -1)), plane, hit));
    assert(hit == Point3(0, 0, 0));
}
```

### 补充说明与使用约定

- 点、线、平面退化时会除以零或触发数值误差；调用者需保证长度和叉积不为零。
- `Line3` 表示无限直线，不是线段；直线平面交点的参数不限制在 `[0,1]`。
- 当前文件是基础工具，不包含三维凸包、三维球面相交等高级算法。

## 相关文档

- [Geometry](geometry.md) —— 二维几何
- [MinimumEnclosingCircle](minimum_enclosing_circle.md) —— 二维最小圆覆盖
