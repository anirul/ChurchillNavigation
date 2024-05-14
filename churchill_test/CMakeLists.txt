
add_executable(churchill_test 
    churchill_test.cpp 
    ../churchill_library/point_search.h
)

target_link_libraries(churchill_test
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)