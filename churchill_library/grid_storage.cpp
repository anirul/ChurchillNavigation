#include "grid_storage.h"

#include <cmath>
#include <algorithm>
#include <iostream>
#include <omp.h>

#include "rect_util.h"

namespace {

    // Helper function to compute exponential boundaries for one half 
    // (positive or negative).
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
        auto it = std::upper_bound(
            boundaries.begin(), 
            boundaries.end(), 
            value);
        return std::max(0, 
            std::min(
                static_cast<int>(std::distance(boundaries.begin(), it)) - 1, 
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
        CalculateExponentialBoundaries(
            total_boundaries_.lx, 
            total_boundaries_.hx, 
            dx_);
    y_boundaries_ = 
        CalculateExponentialBoundaries(
            total_boundaries_.ly, 
            total_boundaries_.hy, 
            dy_);

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
            std::cerr 
                << "Point " << point.x << " " << point.y 
                << " is out of bounds!" << std::endl;
            assert(false);
        }
    }

    temp_points_.clear();
    temp_points_.shrink_to_fit();

    for (int i = 0; i < dx_; ++i) {
        for (int j = 0; j < dy_; ++j) {
            auto grid_node = &grid_[i * dy_ + j];
            std::sort(grid_node->points.begin(), grid_node->points.end(),
                [](const Point& a, const Point& b) {
                    return a.rank < b.rank;
                });
        }
    }
}

void GridStorage::Query(const Rect& range, PriorityList& found) const {
    int start_x = GetIndex(x_boundaries_, range.lx);
    int start_y = GetIndex(y_boundaries_, range.ly);
    int end_x = GetIndex(x_boundaries_, range.hx);
    int end_y = GetIndex(y_boundaries_, range.hy);

#ifdef _DEBUG
    std::cout << "size: " << (range.hx - range.lx) * (range.hy - range.ly) 
        << "\tblock: ";

    int count = (end_x - start_x) + (end_y - start_y);
    std::cout << count;

    std::chrono::high_resolution_clock::time_point start = 
        std::chrono::high_resolution_clock::now();
#endif

    std::vector<PriorityList> local_founds(
        omp_get_max_threads(), 
        PriorityList(found.Capacity()));

#pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
#pragma omp for collapse(2)
        for (int i = start_x; i <= end_x; ++i) {
            for (int j = start_y; j <= end_y; ++j) {
                const auto& grid_node = grid_[i * dy_ + j];
                if (grid_node.points.empty()) continue;
                local_founds[thread_id].FuseSortedRange(
                    grid_node.points, 
                    range);
            }
        }
    }

    for (const auto& local_found : local_founds) {
        found.FusePriority(local_found);
    }

#ifdef _DEBUG
    std::chrono::high_resolution_clock::time_point end =
        std::chrono::high_resolution_clock::now();
    std::cout << "\ttiming: " 
        << std::chrono::duration_cast<std::chrono::microseconds>(
            end - start).count()
        << "us" << std::endl;
#endif
}