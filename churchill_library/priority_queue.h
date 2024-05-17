#pragma once

#include <list>
#include <vector>
#include <cassert>

#include "point_search.h"

class PriorityList {
public:
    PriorityList(size_t capacity) : capacity_(capacity) {
        data_.reserve(capacity);
    }

public:
    void Insert(const Point& point);
    void CopyTo(Point* out_points) const;
    void FusePriority(const PriorityList& other);
    void FuseSortedRange(const std::vector<Point>& other, Rect range);
    void Sort();

public:
    std::vector<Point>::const_iterator Begin() const { return data_.cbegin(); }
    std::vector<Point>::const_iterator End() const { return data_.cend(); }
    size_t Size() const { return data_.size(); }
    size_t Capacity() const { return capacity_; }

private:
    int32_t min_rank_ = 0;
    int32_t max_rank_ = 0;
    size_t capacity_;
    std::vector<Point> data_;

private:
    void UpdateMinMaxRanks();
};
