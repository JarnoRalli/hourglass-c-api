cmake_minimum_required(VERSION 3.24)

message(STATUS "--------------")
message(STATUS "CPP-EXECUTABLE")
message(STATUS "--------------")

project(cpp_executable VERSION 1.0 LANGUAGES CXX C)

# Organize project into folders. Improves readability in IDEs that support this
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# To distinguish between 'release' and 'debug' builds
set(CMAKE_DEBUG_POSTFIX "_d" CACHE STRING "File prefix for debug builds")

# Add build directory to the path where config files are searched for
if(NOT CMAKE_PREFIX_PATH)
    set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR} CACHE PATH "Search path for find_package() installation files")
else()
    list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})
endif()

# Set directories where binary artifacts are placed
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib CACHE PATH "Archive output directory" FORCE)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib CACHE PATH "Library output directory" FORCE)
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib CACHE PATH "Runtime output directory" FORCE)
endif()

# If Conan is being used, silence the output -> too chatty
set(CONAN_CMAKE_SILENT_OUTPUT TRUE CACHE BOOL "Conan verbosity")

find_package(LibStoryBoard REQUIRED CONFIG)

add_executable(cpp_executable src/cpp_executable.cpp)
target_compile_features(cpp_executable PRIVATE cxx_std_14)
target_link_libraries(cpp_executable LibStoryBoard::LibStoryBoard)
