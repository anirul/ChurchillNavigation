#include "quad_tree.h"

void QuadTree::Insert(const AlignedPoint& point) {
    Insert(root_.get(), point);
}

void QuadTree::Query(
    const Rect& rect, 
    std::priority_queue<AlignedPoint, std::vector<AlignedPoint>>& pq, 
    int32_t count) const 
{
    Query(root_.get(), rect, pq, count);
}

void QuadTree::Insert(QuadTreeNode* node, const AlignedPoint& point) {
    if (node->is_leaf) {
        if (node->points.size() < max_capacity_) {
            node->points.push_back(point);
        }
        else {
            Subdivide(node);
            Insert(node, point);
        }
    }
    else {
        if (point.x <= node->bounds.lx + (node->bounds.hx - node->bounds.lx) / 2) {
            if (point.y <= node->bounds.ly + (node->bounds.hy - node->bounds.ly) / 2) {
                Insert(node->nw.get(), point);
            }
            else {
                Insert(node->sw.get(), point);
            }
        }
        else {
            if (point.y <= node->bounds.ly + (node->bounds.hy - node->bounds.ly) / 2) {
                Insert(node->ne.get(), point);
            }
            else {
                Insert(node->se.get(), point);
            }
        }
    }
}

void QuadTree::Subdivide(QuadTreeNode* node) {
    float midX = node->bounds.lx + (node->bounds.hx - node->bounds.lx) / 2;
    float midY = node->bounds.ly + (node->bounds.hy - node->bounds.ly) / 2;

    node->nw = std::make_unique<QuadTreeNode>(Rect{ node->bounds.lx, node->bounds.ly, midX, midY });
    node->ne = std::make_unique<QuadTreeNode>(Rect{ midX, node->bounds.ly, node->bounds.hx, midY });
    node->sw = std::make_unique<QuadTreeNode>(Rect{ node->bounds.lx, midY, midX, node->bounds.hy });
    node->se = std::make_unique<QuadTreeNode>(Rect{ midX, midY, node->bounds.hx, node->bounds.hy });

    for (const auto& point : node->points) {
        if (point.x <= midX) {
            if (point.y <= midY) {
                node->nw->points.push_back(point);
            }
            else {
                node->sw->points.push_back(point);
            }
        }
        else {
            if (point.y <= midY) {
                node->ne->points.push_back(point);
            }
            else {
                node->se->points.push_back(point);
            }
        }
    }
    node->points.clear();
    node->is_leaf = false;
}

void QuadTree::Query(
    const QuadTreeNode* node, 
    const Rect& rect, 
    std::priority_queue<AlignedPoint, std::vector<AlignedPoint>>& pq,
    int32_t count) const 
{
    if (!node) return;

    if (!Intersects(node->bounds, rect)) return;

    if (node->is_leaf) {
        for (const auto& point : node->points) {
            if (Contains(rect, point)) {
                pq.push(point);
                if (pq.size() > count) {
                    pq.pop();
                }
            }
        }
    }
    else {
        Query(node->nw.get(), rect, pq, count);
        Query(node->ne.get(), rect, pq, count);
        Query(node->sw.get(), rect, pq, count);
        Query(node->se.get(), rect, pq, count);
    }
}

bool QuadTree::Contains(const Rect& rect, const AlignedPoint& point) const {
    return point.x >= rect.lx && point.x <= rect.hx && point.y >= rect.ly && point.y <= rect.hy;
}

bool QuadTree::Intersects(const Rect& a, const Rect& b) const {
    return !(a.hx < b.lx || a.lx > b.hx || a.hy < b.ly || a.ly > b.hy);
}
