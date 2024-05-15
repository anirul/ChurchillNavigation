# test for churchill
enable_testing()

find_package(GTest CONFIG REQUIRED)

add_executable(churchill_test 
    ../churchill_library/quad_tree.cpp
    ../churchill_library/point_search.cpp
    ../churchill_library/priority_queue.cpp
    ../churchill_library/rect_util.cpp
    ../churchill_library/vector_storage.cpp
    ../churchill_library/hilbert_grid.cpp
    ../churchill_library/kd_tree.cpp
    ../churchill_library/grid_storage.cpp
    point_search_test.cpp
    point_search_test.h
    priority_queue_test.cpp
    priority_queue_test.h
    grid_storage_test.cpp
    grid_storage_test.h
    quad_tree_test.cpp
    quad_tree_test.h
    main.cpp
)

target_link_libraries(churchill_test
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        GTest::gtest 
        GTest::gtest_main 
        GTest::gmock 
        GTest::gmock_main
)

add_test(ALLTestInMain churchill_test)
