![Linux](https://github.com/JarnoRalli/hourglass-c-api/actions/workflows/ubuntu_build.yml/badge.svg)   ![Windows](https://github.com/JarnoRalli/hourglass-c-api/actions/workflows/visual_studio_build.yml/badge.svg)   ![Code style](https://github.com/JarnoRalli/hourglass-c-api/actions/workflows/code_style.yml/badge.svg)

# Hourglass C-API

This repository showcases a pattern/technique called *Hourglass* for implementing a C-API for C++ libraries. Since C++ lacks an ABI (Application Binary Interface),
libraries written in C++, and compiled using one compiler, cannot necessarily be linked against when using another compiler. Also, if the ABI changes
between different versions of the same compiler, as sometimes happens between major versions, the same problem might arise. A workaround for this is to write
a C-API for the C++ library, since C has a more stable ABI. The downside is that the C-API sometimes lacks the expresiveness of the original C++ API. In this
design we implement a header only C++ API, that uses the C-API, in order to get around this inconvenience.

The original concept of the *Hourglass* C-API is from Stefanus DuToit. He discusses it in this [video](https://www.youtube.com/watch?v=PVYdHDm0q6Y) from the CppCon
from 2014.
