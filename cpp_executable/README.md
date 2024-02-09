# 1 C++ Executable

A simple C++ executable that uses the storyboard library via the C++ API that is based on the C API. The C++ API
is a header only implementation, and since it interacts with the *LibStoryBoard* library using the C API, you
can build *LibStoryBoard* and the C++ executable using different compilers that have different C++ ABI:s.

# 2 Compiling

Before compiling this executable, you first need to build and install the storyboard library. When configuring the C-Executable project
using CMake, you have to set the `LibStoryBoard_DIR` CMake-variable, so that it points to the directory where you installed the library.
The complete path is as follows `<INSTALL_PATH>/lib/cmake/LibStoryBoard`. Inside that path you will find a file called `LibStoryBoardConfig.cmake`.
That is the file that is used by `find_package(LibStoryBoard REQUIRED CONFIG)` in order to discover the library, and the corresponding targets.

