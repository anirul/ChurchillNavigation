#pragma once

#include <list>
#include <vector>
#include <cassert>
#include <mutex>

#include "point_search.h"

class PriorityList {
public:
    PriorityList(size_t capacity) : capacity_(capacity) {
        data_.reserve(capacity);
    }

public:
    void Insert(const Point& point);
    // Copy to an external list !!!should be sorted before!!!
    void CopyTo(Point* out_points) const;
    void FusePriority(const PriorityList& other);
    void FuseSortedRange(const std::vector<Point>& other, Rect range);
    void Sort();
    size_t Size() const;
    bool Full() const;

public:
    std::vector<Point>::const_iterator Begin() const { return data_.cbegin(); }
    std::vector<Point>::const_iterator End() const { return data_.cend(); }
    size_t Capacity() const { return capacity_; }

private:
    // Contain the max rank in the list, this is the value at which you can
    // start updating the priority list.
    int32_t max_rank_ = std::numeric_limits<int32_t>::max();
    const size_t capacity_;
    std::vector<Point> data_;

private:
    void UpdateMaxRank();
};
