#include "kd_tree.h"

#include <algorithm>

bool KDTree::Insert(const Point& point) {
    points_.push_back(point);
    return true;
}

void KDTree::Build() {
    nodes_.reserve(points_.size());
    BuildRecursive(0, points_.size(), 0);
    // Clear and shrink the points_ vector to free up memory
    points_.clear();
    points_.shrink_to_fit();
}

int64_t KDTree::BuildRecursive(int64_t start, int64_t end, int depth) {
    if (start >= end) return -1;

    int axis = depth % 2;
    int64_t mid = (start + end) / 2;

    std::nth_element(points_.begin() + start, points_.begin() + mid, points_.begin() + end,
        [axis](const Point& a, const Point& b) {
            return axis == 0 ? a.x < b.x : a.y < b.y;
        });

    nodes_.push_back({ points_[mid], -1, -1 });
    int64_t node_index = nodes_.size() - 1;

    nodes_[node_index].left = BuildRecursive(start, mid, depth + 1);
    nodes_[node_index].right = BuildRecursive(mid + 1, end, depth + 1);

    return node_index;
}

void KDTree::Query(const Rect& range, PriorityList& found) const {
    QueryRecursive(0, range, found, 0);
}

void KDTree::QueryRecursive(int64_t node_index, const Rect& range, PriorityList& found, int depth) const {
    if (node_index == -1) return;

    const KDNode& node = nodes_[node_index];
    const Point& point = node.point;

    if (point.x >= range.lx && point.x <= range.hx && point.y >= range.ly && point.y <= range.hy) {
        found.Insert(point);
    }

    int axis = depth % 2;
    if (axis == 0) {
        if (range.lx <= point.x) {
            QueryRecursive(node.left, range, found, depth + 1);
        }
        if (range.hx >= point.x) {
            QueryRecursive(node.right, range, found, depth + 1);
        }
    }
    else {
        if (range.ly <= point.y) {
            QueryRecursive(node.left, range, found, depth + 1);
        }
        if (range.hy >= point.y) {
            QueryRecursive(node.right, range, found, depth + 1);
        }
    }
}
