#pragma once

#include <list>
#include <cassert>

#include "aligned_point.h"
#include "point_search.h"

class PriorityList {
public:
    PriorityList(size_t capacity) : capacity_(capacity) {}
    void Insert(const AlignedPoint& point);
    void CopyTo(Point* out_points) const;
    std::list<AlignedPoint>::iterator Begin() { return data_.begin(); }
    std::list<AlignedPoint>::iterator End() { return data_.end(); }
    size_t Size() const { return data_.size(); }

private:
    size_t capacity_;
    std::list<AlignedPoint> data_;
};