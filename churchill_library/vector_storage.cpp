#include "vector_storage.h"

#include <iostream>

#include "rect_util.h"

bool VectorStorage::Insert(const Point& point) {
    points_.push_back(point);
    return true;
}

void VectorStorage::Query(const Rect& range, PriorityList& found) const {
    for (const auto& point : points_) {
        if (RectContains(range, point.x, point.y)) {
            found.Insert(point);
        }
    }
}

void VectorStorage::Build() {
    float x_min = 0;
    float x_max = 0;
    float y_min = 0;
    float y_max = 0;
    for (const Point& point : points_) {
        x_min = std::min(x_min, point.x);
        x_max = std::max(x_max, point.x);
        y_min = std::min(y_min, point.y);
        y_max = std::max(y_max, point.y);
    }
    std::cout 
        << "Bounding box: (" << x_min << ", " << y_min 
        << ") - (" << x_max << ", " << y_max << ")\n";
}