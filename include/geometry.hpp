#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include "geometry_base.hpp"
#include "geometry3d.hpp"
#include "inter.hpp"
#include "circle.hpp"
#include "hull.hpp"
#include "poly.hpp"
#include "dist.hpp"
#include "calipers.hpp"
#include "half.hpp"
#include "minkowski.hpp"

// ============================================================================
// 【二维计算几何全家桶统一入口头文件 (geometry.hpp)】
// ----------------------------------------------------------------------------
// 按依赖拓扑顺序一次性引入全部二维计算几何子模块：
//   1. geometry_base.hpp            ：精度常数 PI/EPS、符号比较 dcmp、点/向量 Vector/Point、直线/线段 Line/Segment、圆 Circle
//   2. inter.hpp                    ：两直线位置关系 line_relation、交点 line_intersection、点在线段上 on_segment、线段相交 segment_intersect
//   3. circle.hpp                   ：直线与圆交点 line_circle_intersection、两圆交点 circle_circle_intersection
//   4. poly.hpp                     ：多边形有向面积 polygon_signed_area、面积 polygon_area、点在多边形内判定 point_in_polygon
//   5. hull.hpp                     ：Andrew 单调链二维凸包 convex_hull
//   6. calipers.hpp                 ：旋转卡壳求凸包直径平方 convex_polygon_diameter_squared
//   7. half.hpp                     ：S&I 算法求半平面交凸核 half_plane_intersection
//   8. dist.hpp                     ：分治法求平面最近点对距离平方 closest_pair_distance_squared
//   9. minkowski.hpp                ：两个凸包的闵可夫斯基和 minkowski_sum
//  10. minimum_enclosing_circle.hpp ：随机增量法最小圆覆盖 minimum_enclosing_circle 及三点外接圆 circumcircle
// ============================================================================

#endif
