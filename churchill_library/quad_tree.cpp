#include "quad_tree.h"

bool QuadTree::Insert(const Point& point) {
    if (!RectContains(boundary_, point.x, point.y)) {
        return false;
    }

    if (!divided_) {
        if (points_.size() < kCapacity) {
            points_.push_back(point);
            return true;
        }
        else {
            Subdivide();
        }
    }

    if (northeast_->Insert(point)) return true;
    if (northwest_->Insert(point)) return true;
    if (southeast_->Insert(point)) return true;
    if (southwest_->Insert(point)) return true;
    
    return false;
}

void QuadTree::Query(const Rect& range, PriorityList& found) const {
    if (!RectIntersects(boundary_, range)) {
        return;
    }

    for (const auto& p : points_) {
        if (RectContains(range, p.x, p.y)) {
            found.Insert(p);
        }
    }

    if (divided_) {
        northeast_->Query(range, found);
        northwest_->Query(range, found);
        southeast_->Query(range, found);
        southwest_->Query(range, found);
    }
}

void QuadTree::Subdivide() {
    float xMid = (boundary_.lx + boundary_.hx) / 2;
    float yMid = (boundary_.ly + boundary_.hy) / 2;

    Rect ne = MakeRect(xMid, yMid, boundary_.hx, boundary_.hy);
    northeast_ = std::make_unique<QuadTree>(ne);

    Rect nw = MakeRect(boundary_.lx, yMid, xMid, boundary_.hy);
    northwest_ = std::make_unique<QuadTree>(nw);

    Rect se = MakeRect(xMid, boundary_.ly, boundary_.hx, yMid);
    southeast_ = std::make_unique<QuadTree>(se);

    Rect sw = MakeRect(boundary_.lx, boundary_.ly, xMid, yMid);
    southwest_ = std::make_unique<QuadTree>(sw);

    divided_ = true;
    // Move existing points to the appropriate subtrees
    for (const auto& point : points_) {
        if (northeast_->Insert(point)) continue;
        if (northwest_->Insert(point)) continue;
        if (southeast_->Insert(point)) continue;
        if (southwest_->Insert(point)) continue;
        assert(false);
    }
    points_.clear();
}
