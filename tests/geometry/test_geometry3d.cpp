#include <cassert>
#include <cmath>
#include "../../include/geometry3d.hpp"

int main() {
    Point3 a(0, 0, 0), b(1, 0, 0), c(0, 1, 0);
    assert(cross(b - a, c - a) == Vector3(0, 0, 1));
    Plane3 plane(a, b, c);
    Point3 hit;
    assert(line_plane_intersection(Line3(Point3(0, 0, 1), Point3(0, 0, -1)), plane, hit));
    assert(hit == a);
    assert(std::fabs(distance(Point3(0, 0, 2), plane) - 2) < EPS);
}
