#pragma once

#include "point_search.h"
#include "priority_queue.h"

struct StorageInterface {
    virtual ~StorageInterface() = default;
    virtual bool Insert(const Point& point) = 0;
    virtual void Query(const Rect& range, PriorityList& found) const = 0;
    virtual void Build() {}
};
