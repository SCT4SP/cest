cmake_minimum_required(VERSION 3.0)
project(runtime_tests)

#set(CMAKE_CXX_COMPILER $ENV{MYGCC}/bin/g++)
set(CMAKE_CXX_COMPILER clang++)
#set(CMAKE_CXX_COMPILER $ENV{MYCLANG}/bin/clang++)
message("CMAKE_CXX_COMPILER is ${CMAKE_CXX_COMPILER}")
set(CMAKE_CXX_FLAGS "-std=c++2a")
#set(CMAKE_CXX_FLAGS "-std=c++2a -stdlib=libc++")
set(CMAKE_BUILD_TYPE Debug)

set(name runtime_tests)
add_executable(${name} ${name}.cpp)
target_include_directories(${name} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../include)

# Indicates to test cest's constexpr (and runtime) support
target_compile_definitions(${name} PRIVATE CONSTEXPR_CEST=1)
