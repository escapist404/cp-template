#include <cassert>
#include <cmath>
#include <vector>
#include "../../include/calipers.hpp"
#include "../../include/circle.hpp"
#include "../../include/dist.hpp"
#include "../../include/geometry_base.hpp"
#include "../../include/half.hpp"
#include "../../include/hull.hpp"
#include "../../include/inter.hpp"
#include "../../include/minkowski.hpp"
#include "../../include/poly.hpp"

int main() {
    std::vector<Point> square{{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    assert(convex_hull(square).size() == 4);
    assert(std::fabs(polygon_area(square) - 1) < EPS);
    assert(point_in_polygon(square, {0.5, 0.5}) && !point_in_polygon(square, {2, 2}));
    assert(std::fabs(convex_polygon_diameter_squared(square) - 2) < EPS);
    assert(std::fabs(closest_pair_distance_squared(square) - 1) < EPS);
    auto intersections = circle_circle_intersection(Circle({0, 0}, 1), Circle({1, 0}, 1));
    assert(intersections.size() == 2);
    auto sum = minkowski_sum(square, square);
    assert(std::fabs(polygon_area(sum) - 4) < EPS);
    std::vector<Line> box{
        Line({0, 0}, {1, 0}), Line({1, 0}, {1, 1}),
        Line({1, 1}, {0, 1}), Line({0, 1}, {0, 0})};
    assert(half_plane_intersection(box).size() == 4);
}
