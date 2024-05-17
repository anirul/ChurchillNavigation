#pragma once

#include <vector>

#include "storage_interface.h"

class GridStorage : public StorageInterface {
public:
    GridStorage(int dx, int dy) : dx_(dx), dy_(dy) {
        grid_.resize(dx * dy);
    }
    bool Insert(const Point& point) override;
    void Query(const Rect& range, PriorityList& found) const override;
    void Build() override;

private:
    struct GridNode {
        Rect boundaries{};
        std::vector<Point> points;
    };
    Rect total_boundaries_;
    int dx_;
    int dy_;
    std::vector<Point> temp_points_;
    std::vector<GridNode> grid_;
    std::vector<double> x_boundaries_;
    std::vector<double> y_boundaries_;
};
