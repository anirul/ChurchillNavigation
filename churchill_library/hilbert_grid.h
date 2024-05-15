#pragma once

#include <vector>
#include <map>
#include <cstdint>
#include "storage_interface.h"

// HilbertStorage class definition
class HilbertStorage : public StorageInterface {
public:
    explicit HilbertStorage(int grid_size);

    bool Insert(const Point& point) override;
    void Query(const Rect& range, PriorityList& found) const override;

private:
    int grid_size_;
    std::multimap<uint32_t, Point> hilbert_points_;  // Use std::multimap for storing points

    static uint32_t HilbertIndex(int x, int y, int n);
};
