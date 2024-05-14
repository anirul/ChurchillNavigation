#include <iostream>

#include "../churchill_library/point_search.h"

#include <Windows.h>

int main(int ac, char** av) {
    // Load the DLL
    HMODULE hModule = LoadLibrary(av[1]);
    if (!hModule) {
        std::cerr << "Could not load the DLL!" << std::endl;
        return 1;
    }

    // Get function pointers
    auto create = (T_create)GetProcAddress(hModule, "create");
    auto search = (T_search)GetProcAddress(hModule, "search");
    auto destroy = (T_destroy)GetProcAddress(hModule, "destroy");

    if (!create || !search || !destroy) {
        std::cerr << "Could not find one or more functions!" << std::endl;
        FreeLibrary(hModule);
        return 1;
    }

    // Create some points
    Point points[] = {
        { 1, 10, 1.0f, 1.0f },
        { 2, 20, 2.0f, 2.0f },
        { 3, 30, 3.0f, 3.0f },
        { 4, 40, 4.0f, 4.0f }
    };

    // Create SearchContext
    SearchContext* sc = create(points, points + 4);

    // Define a search rectangle
    Rect rect = { 1.5f, 1.5f, 3.5f, 3.5f };

    // Prepare output buffer
    Point out_points[2];

    // Search for points
    int32_t found_count = search(sc, rect, 2, out_points);

    // Output the results
    for (int32_t i = 0; i < found_count; ++i) {
        std::cout << "ID: " << (int)out_points[i].id << ", Rank: " << out_points[i].rank
                  << ", X: " << out_points[i].x << ", Y: " << out_points[i].y << std::endl;
    }

    // Destroy the SearchContext
    destroy(sc);

    // Unload the DLL
    FreeLibrary(hModule);

    return 0;
}
