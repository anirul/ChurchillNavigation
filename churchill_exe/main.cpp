#include <iostream>

#include "../churchill_library/point_search.h"

// I want to use min and max from the STL.
#define NOMINMAX

#include <Windows.h>
#include <vector>
#include <chrono>
#include <iostream>

#include <absl/flags/flag.h>
#include <absl/flags/parse.h>

ABSL_FLAG(int64_t, num_points, 100'000, "Number of points to generate.");
ABSL_FLAG(std::string, dll1, "", "The first DLL to be tested.");
ABSL_FLAG(std::string, dll2, "", "The second DLL to be tested.");
ABSL_FLAG(int32_t, seed, 0, "Random seed.");
ABSL_FLAG(int32_t, num_priority_list, 20, "Number of priority lists elements.");

float GetRandomFloat(float min = -10'000.0f, float max = 10'000.0f) {
    return min + (float)rand() / ((float)RAND_MAX / (max - min));
}

int GetRandomInt(int min, int max) {
    return min + rand() % (max - min + 1);
}

int test_dll(
    std::string dll_path, 
    const std::vector<Point> points, 
    int32_t priority_list_size) 
{
    std::cout << "--- Testing DLL: " << dll_path << " ---" << std::endl;

    // Load the DLL
    HMODULE hModule = LoadLibrary(dll_path.c_str());
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

    std::cout << "Creating SearchContext..." << std::endl;
    SearchContext* sc = create(points.data(), points.data() + points.size());

    // Define a search rectangle
    Rect rect = { -1.5f, -1.5f, 3.5e3f, 3.5e3f };

    // Prepare output buffer
    Point* out_points;
    out_points = new Point[priority_list_size];

    std::cout << "Searching for points..." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    int32_t found_count = search(sc, rect, priority_list_size, out_points);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end - start;
    std::cout << "timing: " << duration.count() << "s" << std::endl;

    assert(found_count <= priority_list_size);

    // Output the results
    for (int32_t i = 0; i < found_count; ++i) {
        std::cout << "ID: " << (int)out_points[i].id << ", Rank: " << out_points[i].rank
            << ", X: " << out_points[i].x << ", Y: " << out_points[i].y << std::endl;
    }

    delete[] out_points;

    // Destroy the SearchContext
    destroy(sc);

    // Unload the DLL
    FreeLibrary(hModule);

    return 0;
}

int main(int ac, char** av) {

    absl::ParseCommandLine(ac, av);

    srand(0);
    
    std::cout << "Creating points..." << std::endl;
    std::vector<Point> points;
    int32_t num_zeros = 0;
    for (int64_t i = 0; i < absl::GetFlag(FLAGS_num_points); ++i) {
        Point p;
        p.id = static_cast<int8_t>(GetRandomInt(0, 100));
        p.rank = GetRandomInt(0, 10'000);
        if (p.rank == 0) {
            ++num_zeros;
        }
        p.x = GetRandomFloat();
        p.y = GetRandomFloat();
        points.push_back(p);
    }

    std::cout << "Number of zeros: " << num_zeros << std::endl;

    std::cout << "Testing DLLs..." << std::endl;

    if (!absl::GetFlag(FLAGS_dll1).empty()) {
        int result = test_dll(
            absl::GetFlag(FLAGS_dll1), 
            points, 
            absl::GetFlag(FLAGS_num_priority_list));
        if (result != 0) {
            return result;
        }
    }
    else {
        std::cerr << "No dll1 specified!" << std::endl;
    }

    if (!absl::GetFlag(FLAGS_dll2).empty()) {
        int result = test_dll(
            absl::GetFlag(FLAGS_dll2), 
            points,
            absl::GetFlag(FLAGS_num_priority_list));
        if (result != 0) {
            return result;
        }
    }
    else {
        std::cerr << "No dll2 specified!" << std::endl;
    }

    return 0;
}
