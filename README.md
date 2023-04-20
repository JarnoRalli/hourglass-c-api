![Linux](https://github.com/JarnoRalli/hourglass-c-api/actions/workflows/ubuntu_build.yml/badge.svg)   ![Windows](https://github.com/JarnoRalli/hourglass-c-api/actions/workflows/visual_studio_build.yml/badge.svg)   ![Code style](https://github.com/JarnoRalli/hourglass-c-api/actions/workflows/code_style.yml/badge.svg)

# Hourglass C API for C++ Library

This repository showcases a pattern/technique called *Hourglass* for implementing C APIs for C++ libraries. The problem that this pattern solves is
the lack of standard ABI in C++. What this means is that each compiler, and even different compiler versions, can have non-compatible ABIs.
What this means in practise, is that if you compile a library, that exposes functionality of the library using a C++ API, using a different compiler 
than what you use to compile the final executable with, you most likely will end up having linking problems. Maintainers of commercial C++
libraries, where only binaries are distributed, solve this problem by implementing a C API for their libraries.

<figure align="center">
    <img src="./images/shutterstock_384007933.jpg" width="600">
    <figcaption><b>Hourglass technique for avoiding ABI problems.</b></figcaption>
</figure>

This introduces a slight inconvenience, since C lacks the expressive power of C++, meaning that you force the user of the C++ library to interact 
with it using C. The *Hourglass* technique solves this problem by implementing a header only C++ API, that is based on the C API, allowing users 
of the library to interact with it using C++. This is also where the pattern gets its name from. *Hourglass* technique first narrows C++ to C and 
then back to C++. The original concept of the *Hourglass* C-API technique is from Stefanus DuToit. He discusses it in this [video](https://www.youtube.com/watch?v=PVYdHDm0q6Y) from the 2014 CppCon.

## Hourglass C++ API and STL-Containers

One problem that arises when you go from C++ to C and back to C++ is related to containers and the STL-library. The STL-library implements
many different containers, like the std::vector. The problem is how to deal with API calls that return arrays. The *Hourglass* technique 
deals with this in a quite innovative way: it calls a callback function for each element in the container. One of the parameters given for the callback
function is *client data*. This can be, for example, a pointer to a std::vector that then calls *push_back* for each element.

## Directories

This repository contains the following directories.

* [LibStoryBoard](libstoryboard/README.md)
  * A C++ library that has both C and C++ APIs, implemented using the Hourglass technique. Uses modern CMake.
* [C executable](c_executable/README.md)
  * A C executable that uses *LibStoryBoard* via the C API. Uses modern CMake.
* [C++ executable](cpp_executable/README.md)
  * A C++ executable that uses *LibStoryBoard* via the C++ API. Usees modern CMake.

You first need to build and install the *LibStoryBoard* library and then build either one of the executables for testing. The library
and the executables use modern CMake as the build system.

## Scripts

* [build.sh](build.sh)
  * Builds everything and executes the C and C++ executables. You need to install C and C++ compilers, and cmake>=3.24 before running the script.
* [format_code.sh](format_code.sh)
  * Runs `clang-format` on all the c, cpp, h and hpp files. Intended to be used before committing code. GitHub actions uses Ubuntu 20.04 to test the code in PRs.
