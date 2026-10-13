# Minkowski Sum

`minkowski.hpp` 提供凸多边形闵可夫斯基和：

```cpp
std::vector<Point> minkowski_sum(std::vector<Point> a,
                                 std::vector<Point> b);
```

输入会先取凸包，结果会去除共线边界点。详细说明见 [geometry.md](geometry.md)。
