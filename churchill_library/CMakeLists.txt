
add_library(churchill_library SHARED 
    quad_tree.cpp
    quad_tree.h
    point_search.cpp 
    point_search.h
    priority_queue.cpp
    priority_queue.h
)

target_include_directories(churchill_library 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}
)
