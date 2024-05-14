#pragma once

#include <vector>
#include <algorithm>
#include <memory>
#include <queue>
#include <limits>

#include "point_search.h"
#include "aligned_point.h"

struct QuadTreeNode {
    Rect bounds;
    std::vector<AlignedPoint> points;
    std::unique_ptr<QuadTreeNode> nw, ne, sw, se;
    bool is_leaf;
    QuadTreeNode(const Rect& b) : bounds(b), is_leaf(true) {}
};

class QuadTree {
public:
    QuadTree(const Rect& bounds, size_t capacity) : 
        root_(new QuadTreeNode(bounds)), max_capacity_(capacity) {}
    void Insert(const AlignedPoint& point);
    void Query(
        const Rect& rect, 
        std::priority_queue<AlignedPoint, std::vector<AlignedPoint>>& pq, 
        int32_t count) const;

private:
    void Insert(QuadTreeNode* node, const AlignedPoint& point);
    void Subdivide(QuadTreeNode* node);
    void Query(
        const QuadTreeNode* node, 
        const Rect& rect, 
        std::priority_queue<AlignedPoint, std::vector<AlignedPoint>>& pq,
        int32_t count) const;
    bool Contains(const Rect& rect, const AlignedPoint& point) const;
    bool Intersects(const Rect& a, const Rect& b) const;

    std::unique_ptr<QuadTreeNode> root_;
    size_t max_capacity_;
};
