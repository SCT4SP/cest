#!/bin/bash

# bash build.sh build $PWD/../../tests 1 Debug clang++

CMAKE_BUILD_DIR=$1
CMAKE_SRC_DIR=$2
USE_CEST_VECTOR_ETC=$3
CMAKE_BUILD_TYPE=$4
CMAKE_CXX_COMPILER=$5

echo !!!!!!!!!!!!!!!!!!!!! $1 $2 $3 $4 $5
exit

cmake -E make_directory ${CMAKE_BUILD_DIR}
cd ${CMAKE_BUILD_DIR}
cmake $CMAKE_SRC_DIR -DUSE_CEST_VECTOR_ETC=${USE_CEST_VECTOR_ETC} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
cmake --build . --config ${CMAKE_BUILD_TYPE}
ctest -C ${CMAKE_BUILD_TYPE}
