#include "../../include/minimum_enclosing_circle.hpp"

#include <cassert>
#include <cmath>
#include <vector>
#include "../../include/geometry.hpp"

int main() {
    const std::vector<Point> square{{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    assert(convex_hull(square).size() == 4);
    assert(std::fabs(polygon_signed_area(square) - 1) < EPS);
    assert(point_in_polygon(square, {0.5, 0.5}));
    assert(point_in_polygon(square, {0, 0}));
    assert(!point_in_polygon(square, {2, 2}));
    assert(segment_intersect({{0, 0}, {1, 1}}, {{0, 1}, {1, 0}}));
    assert(!segment_intersect({{0, 0}, {1, 0}}, {{0, 1}, {1, 1}}));
    const Point crossing = line_intersection({{0, 0}, {1, 1}}, {{0, 1}, {1, 0}});
    assert(std::fabs(crossing.x - 0.5) < EPS);
    assert(line_circle_intersection({{-2, 0}, {2, 0}}, Circle({0, 0}, 1)).size() == 2);
    assert(circle_circle_intersection(Circle({0, 0}, 1), Circle({1, 0}, 1)).size() == 2);
    assert(std::fabs(closest_pair_distance_squared({{0, 0}, {3, 4}, {0, 1}}) - 1) < EPS);
    assert(std::fabs(convex_polygon_diameter_squared(square) - 2) < EPS);
    const auto sum = minkowski_sum(square, square);
    assert(sum.size() == 4);
    assert(std::fabs(polygon_area(sum) - 4) < EPS);
    const auto translated = minkowski_sum({{2, 3}}, square);
    assert(translated.size() == 4);
    assert(point_in_polygon(translated, {2.5, 3.5}));
    const auto half_plane = half_plane_intersection({
        {{0, 0}, {1, 0}}, {{1, 0}, {1, 1}},
        {{1, 1}, {0, 1}}, {{0, 1}, {0, 0}}
    });
    assert(half_plane.size() == 4);

    {
	auto circle = minimum_enclosing_circle({{0, 0}, {1, 0}, {0, 1}, {1, 1}});
	assert(std::fabs(circle.center.x - 0.5) < 1e-7);
	assert(std::fabs(circle.center.y - 0.5) < 1e-7);
	assert(std::fabs(circle.radius - std::sqrt(0.5)) < 1e-7);


    }
}
