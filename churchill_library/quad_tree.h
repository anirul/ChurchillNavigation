
#pragma once

#include <memory>
#include <vector>

#include "point_search.h"
#include "priority_queue.h"
#include "storage_interface.h"
#include "rect_util.h"

// Define the Quadtree Node
class QuadTree : public StorageInterface {
public:
    QuadTree(const Rect& boundary)
        : boundary_(boundary), divided_(false) {}
    bool Insert(const Point& point);
    void Query(const Rect& range, PriorityList& found) const;

private:
    static const std::size_t kCapacity = 4 * 1024;
    Rect boundary_;
    std::vector<Point> points_;
    bool divided_;

    std::unique_ptr<QuadTree> northeast_;
    std::unique_ptr<QuadTree> northwest_;
    std::unique_ptr<QuadTree> southeast_;
    std::unique_ptr<QuadTree> southwest_;

    void Subdivide();
};
