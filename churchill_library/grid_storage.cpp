#include "grid_storage.h"

#include <cmath>
#include <algorithm>

#include "rect_util.h"

namespace {


    // Helper function to compute exponential boundaries for one half (positive or negative).
    std::vector<double> CalculateHalfBoundaries(
        double min, 
        double max, 
        int half_divisions) 
    {
        std::vector<double> boundaries(half_divisions + 1);
        double log_min = std::log(min);
        double log_max = std::log(max);

        for (int i = 0; i <= half_divisions; ++i) {
            double t = static_cast<double>(i) / half_divisions;
            boundaries[i] = std::exp(log_min + t * (log_max - log_min));
        }

        return boundaries;
    }

    // Helper function to combine positive and negative boundaries.
    std::vector<double> CalculateExponentialBoundaries(
        double min, 
        double max, 
        int divisions) 
    {
        int half_divisions = divisions / 2;
        std::vector<double> pos_boundaries = 
            CalculateHalfBoundaries(1, max, half_divisions);
        std::vector<double> neg_boundaries = 
            CalculateHalfBoundaries(1, -min, half_divisions);

        std::vector<double> boundaries(divisions + 1);

        for (int i = 0; i <= half_divisions; ++i) {
            boundaries[i] = -neg_boundaries[half_divisions - i] + 1.0;
        }

        for (int i = 1; i <= half_divisions; ++i) {
            boundaries[half_divisions + i] = pos_boundaries[i] - 1.0;
        }

        boundaries[0] = min;
        boundaries[half_divisions] = 0;
        boundaries[divisions] = max;

        return boundaries;
    }

    // Helper function to get the index based on boundaries.
    int GetIndex(const std::vector<double>& boundaries, double value) {
        auto it = std::upper_bound(boundaries.begin(), boundaries.end(), value);
        return std::max(0, 
            std::min(static_cast<int>(std::distance(boundaries.begin(), it)) - 1, 
                static_cast<int>(boundaries.size() - 2)));
    }

}  // namespace

bool GridStorage::Insert(const Point& point) {
    temp_points_.push_back(point);
    return true;
}

void GridStorage::Build() {
    total_boundaries_ = { -1e10, -1e10, 1e10, 1e10 };

    x_boundaries_ = 
        CalculateExponentialBoundaries(total_boundaries_.lx, total_boundaries_.hx, dx_);
    y_boundaries_ = 
        CalculateExponentialBoundaries(total_boundaries_.ly, total_boundaries_.hy, dy_);

    for (int i = 0; i < dx_; ++i) {
        for (int j = 0; j < dy_; ++j) {
            Rect boundaries;
            boundaries.lx = static_cast<float>(x_boundaries_[i]);
            boundaries.ly = static_cast<float>(y_boundaries_[j]);
            boundaries.hx = static_cast<float>(x_boundaries_[i + 1]);
            boundaries.hy = static_cast<float>(y_boundaries_[j + 1]);
            grid_[i * dy_ + j].boundaries = boundaries;
        }
    }

    for (const auto& point : temp_points_) {
        int i = GetIndex(x_boundaries_, point.x);
        int j = GetIndex(y_boundaries_, point.y);
        if (RectContains(grid_[i * dy_ + j].boundaries, point.x, point.y)) {
            grid_[i * dy_ + j].points.push_back(point);
        } else {
            assert(false);
        }
    }

    temp_points_.clear();
    temp_points_.shrink_to_fit();

    for (int i = 0; i < dx_; ++i) {
        for (int j = 0; j < dy_; ++j) {
            auto grid_node = &grid_[i * dy_ + j];
            for (const auto& point : grid_node->points) {
                grid_node->priority_list.Insert(point);
            }
        }
    }
}

void GridStorage::Query(const Rect& range, PriorityList& found) const {
    int start_x = GetIndex(x_boundaries_, range.lx);
    int start_y = GetIndex(y_boundaries_, range.ly);
    int end_x = GetIndex(x_boundaries_, range.hx);
    int end_y = GetIndex(y_boundaries_, range.hy);

    for (int i = start_x; i <= end_x; ++i) {
        for (int j = start_y; j <= end_y; ++j) {
            const auto& grid_node = grid_[i * dy_ + j];
            if (RectIntersects(grid_node.boundaries, range)) {
                if (found.Capacity() == grid_node.priority_list.Capacity() && 
                    RectCompletelyContains(range, grid_node.boundaries)) {
                    found.Fuse(grid_node.priority_list);
                } else {
                    for (const auto& point : grid_node.points) {
                        if (point.x >= range.lx && point.x <= range.hx &&
                            point.y >= range.ly && point.y <= range.hy) {
                            found.Insert(point);
                        }
                    }
                }
            }
        }
    }
}