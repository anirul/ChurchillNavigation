#pragma once

#include <stdint.h>

struct AlignedPoint {
    float x;
    float y;
    int32_t rank;
    int8_t id;
    AlignedPoint(float px, float py, int32_t pr, int8_t pid) :
        x(px), y(py), rank(pr), id(pid) {}
    AlignedPoint() : x(0), y(0), rank(0), id(0) {}
};

inline bool operator<(const AlignedPoint& a, const AlignedPoint& b) {
    return a.rank < b.rank;
}
