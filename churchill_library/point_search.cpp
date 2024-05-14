#include "point_search.h"

#include <vector>
#include <algorithm>
#include <memory>
#include <map>
#include <iostream>

#include "quad_tree.h"
#include "priority_queue.h"

struct SearchContext {
    QuadTree tree;
};

extern "C" __declspec(dllexport) SearchContext* __stdcall 
create(const Point* points_begin, const Point* points_end) 
{    
    float max = std::numeric_limits<float>::max();
    float min = std::numeric_limits<float>::min();
    auto sc = new SearchContext{ QuadTree(Rect{ min, min, max, max }) };
    for (auto it = points_begin; it != points_end; ++it) {
        sc->tree.Insert(*it);
    }
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
    sc->tree.Query(rect, results);
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
