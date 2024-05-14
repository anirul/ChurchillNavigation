
#pragma once

#include <memory>
#include <vector>

#include "point_search.h"
#include "priority_queue.h"


inline Rect MakeRect(float lx, float ly, float hx, float hy) {
    return Rect{ lx, ly, hx, hy };
}

inline bool RectContains(const Rect& r, float x, float y) {
    return (x >= r.lx && x < r.hx && y >= r.ly && y < r.hy);
}
inline bool RectIntersects(const Rect& l, const Rect& r) {
    return !(
        r.lx > l.hx || r.hx < l.lx ||
        r.ly > l.hy || r.hy < l.ly);
}

// Define the Quadtree Node
class QuadTree {
public:
    QuadTree(const Rect& boundary)
        : boundary_(boundary), divided_(false) {}
    bool Insert(const Point& point);
    void Query(const Rect& range, PriorityList& found) const;

private:
    static const size_t kCapacity = 1024;
    Rect boundary_;
    std::vector<Point> points_;
    bool divided_;
    std::unique_ptr<QuadTree> northeast_;
    std::unique_ptr<QuadTree> northwest_;
    std::unique_ptr<QuadTree> southeast_;
    std::unique_ptr<QuadTree> southwest_;

    void Subdivide();
};
