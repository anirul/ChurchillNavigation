#include "point_search.h"

#include <vector>
#include <memory>
#include <map>
#include <iostream>

#include "quad_tree.h"
#include "priority_queue.h"
#include "vector_storage.h"
#include "hilbert_grid.h"
#include "kd_tree.h"
#include "grid_storage.h"

struct SearchContext {
    std::unique_ptr<StorageInterface> store;
};

extern "C" __declspec(dllexport) SearchContext* __stdcall 
create(const Point* points_begin, const Point* points_end) 
{    
    const float max = 1e11f;
    const float min = -1e11f;
    auto sc = new SearchContext{ 
        // std::make_unique<QuadTree>(Rect{ min, min, max, max })
        // std::make_unique<KDTree>()
        // std::make_unique<VectorStorage>(1024 * 1024)
        // std::make_unique<HilbertStorage>(1024)
        std::make_unique<GridStorage>(1024, 1024)
    };
    for (auto it = points_begin; it != points_end; ++it) {
        sc->store->Insert(*it);
    }
    sc->store->Build();
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
    sc->store->Query(rect, results);
    results.Sort();
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
