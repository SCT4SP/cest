cmake_minimum_required(VERSION 3.0)
set(name cest_tests)
project(${name})

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Wall)

message("CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
message("CMAKE_CXX_STANDARD: ${CMAKE_CXX_STANDARD}")
message("CMAKE_BUILD_TYPE:   ${CMAKE_BUILD_TYPE}")

add_executable(${name} ${name}.cpp)
add_test(all_${name} ${name})
target_include_directories(${name} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../include)

# Indicates to test cest's constexpr (and runtime) support
target_compile_definitions(${name} PRIVATE CONSTEXPR_CEST=1)

enable_testing()
