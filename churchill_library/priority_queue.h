#pragma once

#include <list>
#include <cassert>

#include "point_search.h"

class PriorityList {
public:
    PriorityList(size_t capacity) : capacity_(capacity) {}

public:
    void Insert(const Point& point);
    void CopyTo(Point* out_points) const;

public:
    std::list<Point>::iterator Begin() { return data_.begin(); }
    std::list<Point>::iterator End() { return data_.end(); }
    size_t Size() const { return data_.size(); }
    size_t Capacity() const { return capacity_; }

private:
    size_t capacity_;
    std::list<Point> data_;
};