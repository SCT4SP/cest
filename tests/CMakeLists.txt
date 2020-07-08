cmake_minimum_required(VERSION 3.0)
project(runtime_tests)

# If working in the include directory use:
# make -C ../../tests/build/

set(CMAKE_CXX_COMPILER $ENV{MYGCC}/bin/g++)
message("MYGCC is $ENV{MYGCC}")
set(CMAKE_CXX_FLAGS "-std=c++2a")
#set(CMAKE_CXX_COMPILER $ENV{MYCLANG}/bin/clang++)
#set(CMAKE_CXX_FLAGS "-std=c++2a")
#set(CMAKE_CXX_FLAGS "-std=c++2a -stdlib=libc++")
set(CMAKE_BUILD_TYPE Debug)

set(name runtime_tests)
add_executable(${name} ${name}.cpp)
target_include_directories(${name} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../include)

# Indicates to test cest's constexpr (and runtime) support
target_compile_definitions(${name} PRIVATE CONSTEXPR_CEST=1)
# Indicates to test the C++ standard library's constexpr (and runtime) support
target_compile_definitions(${name} PRIVATE CONSTEXPR_STDLIB=0)
# Indicates that constexpr tests (cest+stdlib) should be performed
target_compile_definitions(${name} PRIVATE RUN_STATIC_TESTS=0)
