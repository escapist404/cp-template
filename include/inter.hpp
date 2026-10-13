#ifndef INTER_HPP
#define INTER_HPP

#include <cassert>
#include <vector>
#include "geometry_base.hpp"

inline int line_relation(const Line& a, const Line& b) {
    if (dcmp(cross(a.end - a.start, b.end - b.start)) != 0) {
        return 1;
    }
    return dcmp(distance(a.start, b)) == 0 ? -1 : 0;
}

inline Point line_intersection(const Line& a, const Line& b) {
    const Vector da = a.end - a.start;
    const Vector db = b.end - b.start;
    const Real denominator = cross(da, db);
    assert(dcmp(denominator) != 0);
    return a.start + da * (cross(b.start - a.start, db) / denominator);
}

inline bool on_segment(Point point, const Segment& segment) {
    return dcmp(cross(segment.end - segment.start, point - segment.start)) == 0 &&
           dcmp(dot(point - segment.start, point - segment.end)) <= 0;
}

inline bool segment_intersect(const Segment& a, const Segment& b) {
    const int c1 = dcmp(cross(a.end - a.start, b.start - a.start));
    const int c2 = dcmp(cross(a.end - a.start, b.end - a.start));
    const int c3 = dcmp(cross(b.end - b.start, a.start - b.start));
    const int c4 = dcmp(cross(b.end - b.start, a.end - b.start));
    if (c1 * c2 > 0 || c3 * c4 > 0) {
        return false;
    }
    return (c1 != 0 || on_segment(b.start, a)) &&
           (c2 != 0 || on_segment(b.end, a)) &&
           (c3 != 0 || on_segment(a.start, b)) &&
           (c4 != 0 || on_segment(a.end, b));
}


#endif
