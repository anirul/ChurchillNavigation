#pragma once

#include <vector>

#include "storage_interface.h"

class VectorStorage : public StorageInterface {
public:
    VectorStorage(std::size_t capacity) {
        points_.reserve(capacity);
    }
    bool Insert(const Point& point) override;
    void Query(const Rect& range, PriorityList& found) const override;
    void Build() override;

private:
    std::vector<Point> points_;
};