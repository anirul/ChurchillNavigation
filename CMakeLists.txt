cmake_minimum_required(VERSION 3.14)

project(ChurchillNavigation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(churchill_library)
add_subdirectory(churchill_test)
add_subdirectory(churchill_exe)
