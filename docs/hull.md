# ConvexHull

给定平面点集，求其凸包，并在凸多边形上计算最远点对等几何量。

依赖 `geometry_base.hpp`、`types.hpp`。

## 接口

```cpp
std::vector<Point> convex_hull(std::vector<Point> points);
```

**算法**：Andrew 算法（Graham 扫描的改进版）

**返回值**：凸包上的点，**逆时针**顺序

**约束**
- 点数 $\ge 3$ 时才有意义
- 共线的点会被去除（严格凸包）

**复杂度**：$O(n \log n)$

## 使用示例

```cpp
#include "geometry_base.hpp"
#include "hull.hpp"
#include <iostream>

int main() {
    std::vector<Point> points = {
        {0, 0}, {1, 0}, {0, 1}, {0.5, 0.5}
    };
    
    auto hull = convex_hull(points);
    
    for (auto p : hull) {
        std::cout << p.x << ' ' << p.y << '\n';
    }
    
    return 0;
}
```

### 旋转卡壳求最远点对

```cpp
Real farthest_pair(const std::vector<Point> &hull) {
    int n = hull.size();
    if (n <= 1) return 0;
    
    Real ans = 0;
    for (int i = 0, j = 1; i < n; ++i) {
        while (cross(hull[(i+1)%n] - hull[i], hull[j] - hull[i]) <
               cross(hull[(i+1)%n] - hull[i], hull[(j+1)%n] - hull[i])) {
            j = (j + 1) % n;
        }
        ans = std::max(ans, (hull[i] - hull[j]).norm());
    }
    return ans;
}
```

### 补充说明与使用约定

- **严格凸包**：共线点被去除，只保留拐点。
- 返回顺序为**逆时针**。
- 点数过少（$< 3$）时返回退化凸包。

## 相关文档

- [Geometry](geometry.md) —— 几何综合
