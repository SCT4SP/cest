cmake_minimum_required(VERSION 3.0)
project(runtime_tests)

# If working in the include directory use:
# make -C ../../tests/build/

set(CMAKE_CXX_COMPILER $ENV{MYGCC}/bin/g++)
set(CMAKE_CXX_FLAGS "-std=c++2a")
#set(CMAKE_CXX_COMPILER $ENV{MYCLANG}/bin/clang++)
#set(CMAKE_CXX_FLAGS "-std=c++2a")
#set(CMAKE_CXX_FLAGS "-std=c++2a -stdlib=libc++")
set(CMAKE_BUILD_TYPE Debug)

set(name runtime_tests)
add_executable(${name} ${name}.cpp)
target_include_directories(${name} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../include)
