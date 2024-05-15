#pragma once

#include <vector>

#include "storage_interface.h"

class KDTree : public StorageInterface {
public:
    bool Insert(const Point& point) override;
    void Query(const Rect& range, PriorityList& found) const override;
    void Build() override;

private:

    struct KDNode {
        Point point;
        int64_t left;
        int64_t right;
    };

    std::vector<Point> points_;
    std::vector<KDNode> nodes_;

    int64_t BuildRecursive(int64_t start, int64_t end, int depth);
    void QueryRecursive(int64_t node_index, const Rect& range, PriorityList& found, int depth) const;
};
