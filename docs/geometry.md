# Geometry

计算几何模块。`geometry.hpp` 是二维几何的**聚合头文件**；三维基础定义和运算见 `geometry3d.hpp`。

```cpp
#include "geometry_base.hpp"       // 点、向量、直线、圆，以及基本运算
#include "inter.hpp"      // 直线/线段求交
#include "circle.hpp"     // 直线与圆、圆与圆的交点
#include "hull.hpp"       // 凸包
#include "poly.hpp"       // 多边形面积、点在多边形内
#include "dist.hpp"       // 平面最近点对
#include "calipers.hpp"   // 旋转卡壳（凸包直径）
#include "half.hpp"       // 半平面交
#include "minkowski.hpp"  // 闵可夫斯基和
#include "geometry3d.hpp" // 三维点、线、面、球
```

需要哪个就单独 include 哪个子文件，可以少编译一些东西。

## 全局常量与比较

```cpp
inline constexpr Real PI;
inline constexpr Real EPS = 1E-8;

int dcmp(const Real a, const Real b = 0);
```

- `dcmp(a, b)` 返回 `-1 / 0 / 1`，`|a-b| < EPS` 时判为相等（`0`）
- 默认参数使得 `dcmp(x)` 等价于 `dcmp(x, 0)`，即判断 `x` 是否与零在 `EPS` 内相等

**注意**：`EPS` 是**全局常量**，写在头文件里无法逐题调整。精度敏感的题目需要自行复制并修改。

## [geometry_base.hpp](./geometry_base.md) —— 基础类型

### Vector / Point

```cpp
struct Vector {
    Real x, y;
    Vector(Real x = 0, Real y = 0);

    Vector operator+(Vector other) const;
    Vector operator-(Vector other) const;
    Vector operator*(Real) const;
    Vector operator/(Real) const;
    bool   operator==(Vector other) const;   // 按 EPS 比较

    Real length_squared() const;
    Real length() const;
    Vector unit() const;                  // 单位化
    Vector normal() const;                // 逆时针旋转 90° 并单位化
    Vector rotated(Real angle) const;     // 逆时针旋转 angle 弧度
};

inline Vector operator*(Real, Vector vector);   // 支持 2.0 * v
using Point = Vector;
```

### 自由函数

```cpp
Real dot(Vector a, Vector b);
Real cross(Vector a, Vector b);
Real distance_squared(Point a, Point b);
Real distance(Point a, Point b);
Real angle(Vector a, Vector b);
```

`angle` 返回 $[0,\pi]$ 之间的夹角，用的是 `std::acos`，对接近 $0$ 或 $\pi$ 的输入数值上不稳定。

### Line / Segment

```cpp
struct Line {
    Point start, end;

    Line();
    Line(Point p1, Point p2);
    Line(Point point, Real angle);   // 过 point、倾角 angle
    Line(Real a, Real b, Real c);           // ax + by + c = 0

    Point projection(Point point) const;  // 点在直线上的投影
    Point reflection(Point point) const;  // 关于直线的对称点
};
using Segment = Line;
```

**线段与直线是同一个类型**，`Line` 与 `Segment` 只是别名。因此「求交」系列函数不会自动区分二者，需按语义选用。

```cpp
Real distance(Point point, const Line& line);  // 点到直线距离
```

### Circle

```cpp
struct Circle {
    Point center;
    Real radius;
    Circle();
    Circle(Point center, Real radius);
    Circle(Real x, Real y, Real radius);
};
```

## [hull.hpp](./hull.md) —— 凸包

```cpp
std::vector<Point> convex_hull(std::vector<Point> points);
```

**返回**
- 点集的凸包，**逆时针**方向
- **共线边界点被剔除**
- 输入为 0 或 1 个点时原样返回；重复点会被先去重

**复杂度**
- $O(n \log n)$（先按 `(x, y)` 排序）

**注意**：参数**按值传递**，可以放心 `convex_hull(std::move(v))` 或直接传副本。

## [inter.hpp](./inter.md) —— 直线与线段

```cpp
int   line_relation(const Line &a, const Line &b);
Point line_intersection(const Line &a, const Line &b);
bool  on_segment(Point point, const Segment& segment);
bool  segment_intersect(const Segment &a, const Segment &b);
```

- `line_relation` 返回：`1` 相交，`-1` 重合，`0` 平行
- `line_intersection` 求**直线**交点；两线平行时有 `assert(dcmp(denominator) != 0)` 会失败，调用前应先用 `line_relation` 判断
- `on_segment` 判断点是否在**线段**上（含端点）
- `segment_intersect` 判断两线段是否相交，**含端点接触与共线重叠**

## [circle.hpp](./circle.md) —— 圆

```cpp
std::vector<Point> line_circle_intersection(const Line &line, const Circle &circle);
std::vector<Point> circle_circle_intersection(const Circle &a, const Circle &b);
```

- 返回**交点列表**：相离返回空 `vector`，相切返回 1 个点，相交返回 2 个点
- `line` 参数按**直线**处理（会与整条直线求交，不裁剪到线段范围）

## [poly.hpp](./poly.md) —— 多边形

```cpp
Real polygon_signed_area(const std::vector<Point> &polygon);  // 有向面积
Real polygon_area(const std::vector<Point> &polygon);         // 绝对值
bool point_in_polygon(const std::vector<Point>& polygon, Point point);
```

- `polygon_signed_area` 逆时针为正
- `point_in_polygon` **包含边界**，对顺时针与逆时针多边形均正确，任意简单多边形（不必是凸的）

**注意参数顺序**：多边形在前，点在**后**。

## [dist.hpp](./dist.md) —— 最近点对

```cpp
Real closest_pair_distance_squared(std::vector<Point> points);
```

返回平面最近点对的**距离平方**（不是距离）。

**约束**
- 点数少于 2 时返回 `std::numeric_limits<Real>::infinity()`

**复杂度**
- $O(n \log n)$（分治 + 按 y 归并）

**注意**：按值传参，内部会就地排序与重排。

## [calipers.hpp](./calipers.md) —— 旋转卡壳

```cpp
Real convex_polygon_diameter_squared(const std::vector<Point> &polygon);
```

返回凸多边形直径的**平方**，即最远点对距离的平方。

**约束**
- 输入应当是**凸多边形**（通常先 `convex_hull`）
- 点数少于 2 时返回 $0$

**复杂度**
- $O(n)$

## [half.hpp](./half.md) —— 半平面交

```cpp
std::vector<Point> half_plane_intersection(std::vector<Line> lines);
```

**语义（重要）**
- 每条有向直线保留的是它的**左侧半平面**（`cross(direction, point - start) >= 0` 的一侧）
- 方向决定了保留哪边：把 `Line` 的 `start`、`end` 反过来，保留的就变成另一侧

**返回**
- 交集的顶点序列（多边形）
- 交集为空或退化（少于 3 条有效边）时返回空 `vector`

**复杂度**
- $O(n \log n)$（按极角排序 + 双端队列维护）

## [minkowski.hpp](./minkowski.md) —— 闵可夫斯基和

```cpp
std::vector<Point> minkowski_sum(std::vector<Point> a, std::vector<Point> b);
```

返回两个**凸**多边形的闵可夫斯基和。

**行为**
- 内部会先对两个输入各做一次 `convex_hull`，因此输入顺序任意、起点任意、可含共线点
- 结果同样经过凸包处理，共线边界点被剔除
- 任一输入为空时返回空 `vector`；某一边退化为单点时，返回另一点集平移后的结果

**复杂度**
- $O((n + m) \log(n + m))$

## 综合示例

```cpp
#include "geometry.hpp"
#include <iostream>

int main() {
    std::vector<Point> points = {
        {0, 0}, {1, 0}, {1, 1}, {0, 1}, {0.5, 0.5}
    };

    auto hull = convex_hull(points);   // 逆时针，内部点 (0.5,0.5) 被剔除

    std::cout << polygon_area(hull) << '\n';                       // 1
    std::cout << point_in_polygon(hull, Point(0.5, 0.5)) << '\n';  // 1 (在内部)
    std::cout << convex_polygon_diameter_squared(hull) << '\n';    // 2 (对角线长度平方)

    return 0;
}
```

## 实现约定

- **`EPS` 与 `PI` 是全局常量**，无法按题目调整；精度题请复制后自行修改。
- `Line` 与 `Segment` 是同一类型，函数不会替你区分。
- 求距离的函数常返回**平方**：`distance_squared`、`closest_pair_distance_squared`、`convex_polygon_diameter_squared`。需要真距离请自己 `std::sqrt`。
- `convex_hull` 会**剔除共线点**。若题目需要保留边上的整点或共线边界点，此实现不适用。
- 半平面交保留**左侧**，用错方向会得到完全相反的半平面。
- `point_in_polygon` 的参数顺序是 `(polygon, point)`。
- `circle_circle_intersection` 对同心圆直接返回空；内含（半径差大于圆心距）也返回空。
- `Real` 默认为 `double`（见 `types.hpp`）。定点坐标或大整数坐标需要 `long double` / 整数几何时，本模块需自行改造。

## 相关文档

- [types.hpp](types.md) —— `Real`、`Complex` 等类型别名
