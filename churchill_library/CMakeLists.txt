
add_library(churchill_library SHARED 
    grid_storage.cpp
    grid_storage.h
    quad_tree.cpp
    quad_tree.h
    kd_tree.cpp
    kd_tree.h
    vector_storage.cpp
    vector_storage.h
    hilbert_grid.cpp
    hilbert_grid.h
    point_search.cpp 
    point_search.h
    priority_queue.cpp
    priority_queue.h
    rect_util.cpp
    rect_util.h
    storage_interface.h
)

target_include_directories(churchill_library 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}
)
