cmake_minimum_required(VERSION 3.2)

project(cest LANGUAGES CXX)

option(CEST_BUILD_TEST "Build tests for cest" ON)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/cest DESTINATION include)

if(CEST_BUILD_TEST)
  add_subdirectory(tests)
endif()
