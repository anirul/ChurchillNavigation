# exe for churchill
find_package(absl CONFIG REQUIRED)

add_executable(churchill_exe main.cpp)

target_link_libraries(churchill_exe
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        absl::base
        absl::flags
        absl::flags_parse
)