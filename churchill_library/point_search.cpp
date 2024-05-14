#include "point_search.h"

#include <vector>
#include <algorithm>
#include <memory>
#include <map>
#include <iostream>

#include "quad_tree.h"
#include "priority_queue.h"

struct SearchContext {
    std::vector<AlignedPoint> points;
};

std::ostream& operator<<(std::ostream& os, const Rect& r) {
    os  << "Rect: [" << r.lx 
        << ", " << r.ly 
        << ", " << r.hx 
        << ", " 
        << r.hy << "]";
    return os;
}

extern "C" __declspec(dllexport) SearchContext* __stdcall 
create(const Point* points_begin, const Point* points_end) 
{    
    auto sc = new SearchContext{};
    bool first = true;
    Rect boundaries{};
    for (auto it = points_begin; it != points_end; ++it) {
        sc->points.push_back(AlignedPoint(it->x, it->y, it->rank, it->id));
        if (first) {
            boundaries.lx = boundaries.hx = it->x;
            boundaries.ly = boundaries.hy = it->y;
            first = false;
        }
        else {
            boundaries.lx = std::min(boundaries.lx, it->x);
            boundaries.hx = std::max(boundaries.hx, it->x);
            boundaries.ly = std::min(boundaries.ly, it->y);
            boundaries.hy = std::max(boundaries.hy, it->y);
        }
    }
    std::cout << "boundaries: " << boundaries << std::endl;
    return sc;
}

extern "C" __declspec(dllexport) int32_t __stdcall 
search(
    SearchContext* sc, 
    const Rect rect, 
    const int32_t count, 
    Point* out_points) 
{
    if (!sc) return 0;
    int32_t num_copied = 0;
    PriorityList results(static_cast<size_t>(count));
    for (int i = 0; i < sc->points.size(); ++i) {
        if (rect.lx <= sc->points[i].x && sc->points[i].x <= rect.hx &&
            rect.ly <= sc->points[i].y && sc->points[i].y <= rect.hy) {
            results.Insert(sc->points[i]);
        }
    }
    for (auto it = results.Begin(); it != results.End(); ++it) {
        out_points[num_copied].id = it->id;
        out_points[num_copied].rank = it->rank;
        out_points[num_copied].x = it->x;
        out_points[num_copied].y = it->y;
        ++num_copied;
        if (num_copied == count) break;
    }
#ifdef _DEBUG
    std::cout << ".";
#endif // _DEBUG
    return num_copied;
}

extern "C" __declspec(dllexport) SearchContext* __stdcall 
destroy(SearchContext* sc) 
{
    delete sc;
    return nullptr;
}
