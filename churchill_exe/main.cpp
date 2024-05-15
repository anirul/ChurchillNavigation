#include <iostream>

#include "../churchill_library/point_search.h"

#define NOMINMAX

#include <Windows.h>
#include <vector>
#include <random>
#include <chrono>

float GetRandomFloat() {
    // Create a random device and seed the random number generator
    static std::random_device rd;  // Non-deterministic random number generator
    static std::mt19937 gen(rd()); // Seed the Mersenne Twister RNG

    // Define the range for the random float value
    static const float lower_bound = -10'000.0f;
    static const float upper_bound = 10'000.0f;

    // Create a distribution to produce random float values within the specified range
    std::uniform_real_distribution<float> dis(lower_bound, upper_bound);

    // Generate a random float value
    return dis(gen);
}

int GetRandomInt() {
    // Create a random device and seed the random number generator
    static std::random_device rd;  // Non-deterministic random number generator
    static std::mt19937 gen(rd()); // Seed the Mersenne Twister RNG

    // Define the range for the random integer value
    static const int lower_bound = 0;
    static const int upper_bound = 10'000;

    // Create a distribution to produce random integer values within the specified range
    std::uniform_int_distribution<int> dis(lower_bound, upper_bound);

    // Generate a random integer value
    return dis(gen);
}

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

    std::cout << "Creating points..." << std::endl;
    std::vector<Point> points;
    for (auto i = 0; i < 10'000'000; ++i) {
        points.push_back(
            Point{
                static_cast<int8_t>(GetRandomInt()),
                GetRandomInt(),
                GetRandomFloat(),
                GetRandomFloat()
            });
    }

    std::cout << "Creating SearchContext..." << std::endl;
    SearchContext* sc = create(points.data(), points.data() + points.size());

    // Define a search rectangle
    Rect rect = { -1.5f, -1.5f, 3.5e3f, 3.5e3f };

    // Prepare output buffer
    Point out_points[20];

    std::cout << "Searching for points..." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    int32_t found_count = search(sc, rect, 20, out_points);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end - start;
    std::cout << "timing: " << duration.count() << "s" << std::endl;

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
