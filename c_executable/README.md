# 1 C Executable

A simple C executable that uses the storyboard library via the C-API. Since C has (relatively) stable ABI, there shouldn't exist any
problems compiling the `LibStoryBoard` with, say *GCC*, and then compiling the C executable with a different compiler like *clang*.

# 2 Compiling

Before compiling this executable, you first need to build and install the storyboard library. When configuring the C-Executable project
using CMake, you have to set the `LibStoryBoard_DIR` CMake-variable, so that it points to the directory where you installed the library.
The complete path is as follows `<INSTALL_PATH>/lib/cmake/LibStoryBoard`. Inside that path you will find a file called `LibStoryBoardConfig.cmake`.
That is the file that is used by `find_package(LibStoryBoard REQUIRED CONFIG)` in order to discover the library, and the corresponding targets.
