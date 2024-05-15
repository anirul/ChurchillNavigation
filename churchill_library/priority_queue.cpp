#include "priority_queue.h"

void PriorityList::CopyTo(Point* out_points) const {
    assert(data_.size() <= capacity_);
    for (const auto& p : data_) {
        *out_points++ = Point{ p.id, p.rank, p.x, p.y };
    }
}

void PriorityList::Insert(const Point& point) {
    // Find the appropriate position to insert the point
    auto it = std::find_if(
        data_.begin(), 
        data_.end(), 
        [&point](const Point& p) {
            return point.rank > p.rank;
        });

    // Insert the point while maintaining sorted order
    data_.insert(it, point);

    // If the size exceeds the capacity, remove the element with the lowest priority
    if (data_.size() > capacity_) {
        data_.pop_back();
    }
}