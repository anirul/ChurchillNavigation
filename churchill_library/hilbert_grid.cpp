#include "hilbert_grid.h"
#include <algorithm>
#include "rect_util.h"

// HilbertStorage constructor implementation
HilbertStorage::HilbertStorage(int grid_size) : grid_size_(grid_size) {}

// HilbertStorage Insert method implementation
bool HilbertStorage::Insert(const Point& point) {
    int x = static_cast<int>(point.x * grid_size_);
    int y = static_cast<int>(point.y * grid_size_);
    uint32_t index = HilbertIndex(x, y, grid_size_);
    hilbert_points_.insert({ index, point });
    return true;
}

// HilbertStorage Query method implementation
void HilbertStorage::Query(const Rect& range, PriorityList& found) const {
    int x_min = static_cast<int>(range.lx * grid_size_);
    int y_min = static_cast<int>(range.ly * grid_size_);
    int x_max = static_cast<int>(range.hx * grid_size_);
    int y_max = static_cast<int>(range.hy * grid_size_);

    uint32_t min_index = HilbertIndex(x_min, y_min, grid_size_);
    uint32_t max_index = HilbertIndex(x_max, y_max, grid_size_);

    auto lower = hilbert_points_.lower_bound(min_index);
    auto upper = hilbert_points_.upper_bound(max_index);

    for (auto it = lower; it != upper; ++it) {
        if (RectContains(range, it->second.x, it->second.y)) {
            found.Insert(it->second);
        }
    }
}

// HilbertStorage HilbertIndex method implementation
uint32_t HilbertStorage::HilbertIndex(int x, int y, int n) {
    uint32_t index = 0;
    for (int s = n / 2; s > 0; s /= 2) {
        int rx = (x & s) ? 1 : 0;
        int ry = (y & s) ? 1 : 0;
        index += s * s * ((3 * rx) ^ ry);
        if (!ry) {
            if (rx) {
                x = n - 1 - x;
                y = n - 1 - y;
            }
            std::swap(x, y);
        }
    }
    return index;
}
