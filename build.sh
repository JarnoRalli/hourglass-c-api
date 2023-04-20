#! /bin/bash

#This script builds the library and the C and C++ executables, and executes these.
#You need to have a C and C++ compilers, and cmake installed.

root_dir=$(pwd)

# Build, test and install the library
cd libstoryboard
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=./install
cmake --build .
ctest -C Release
cmake --install .

# Build the C executable and execute it
cd $root_dir
cd c_executable
mkdir build
cd build
cmake .. -D CMAKE_BUILD_TYPE=Release -D LibStoryBoard_DIR=$root_dir/libstoryboard/build/install/lib/cmake/LibStoryBoard
cmake --build .
./c_executable

# Build the C++ executable and execute it
cd $root_dir
cd cpp_executable
mkdir build
cd build
cmake .. -D CMAKE_BUILD_TYPE=Release -D LibStoryBoard_DIR=$root_dir/libstoryboard/build/install/lib/cmake/LibStoryBoard
cmake --build .
./cpp_executable
