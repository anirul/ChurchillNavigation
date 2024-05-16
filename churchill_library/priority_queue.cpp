#include "priority_queue.h"

#include <algorithm>

void PriorityList::CopyTo(Point* out_points) const {
    assert(data_.size() <= capacity_);
    for (const auto& p : data_) {
        *out_points++ = Point{ p.id, p.rank, p.x, p.y };
    }
}

void PriorityList::Insert(const Point& point) {
    if (data_.empty()) {
        min_rank_ = point.rank;
        max_rank_ = point.rank;
        data_.push_back(point);
    }
    else if (data_.size() < capacity_) {
        data_.push_back(point);
        min_rank_ = std::min(min_rank_, point.rank);
        max_rank_ = std::max(max_rank_, point.rank);
    }
    else if (point.rank > min_rank_) {
        for (auto it = data_.begin(); it != data_.end(); ++it) {
            if (it->rank == min_rank_) {
                *it = point;
                break;
            }
        }
        UpdateMinMaxRanks();
    }
}

void PriorityList::Fuse(const PriorityList& other) {
    for (auto it = other.Begin(); it != other.End(); ++it) {
        Insert(*it);
    }
}

void PriorityList::Sort() {
    // Sort the list
    std::sort(data_.begin(), data_.end(),
        [](const Point& a, const Point& b) {
            return a.rank < b.rank;
        });
}

void PriorityList::UpdateMinMaxRanks() {
    min_rank_ = std::numeric_limits<int32_t>::max();
    max_rank_ = std::numeric_limits<int32_t>::min();
    for (const auto& point : data_) {
        min_rank_ = std::min(min_rank_, point.rank);
        max_rank_ = std::max(max_rank_, point.rank);
    }
}
