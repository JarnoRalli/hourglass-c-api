![Linux](https://github.com/JarnoRalli/hourglass-c-api/actions/workflows/ubuntu_build.yml/badge.svg)   ![Windows](https://github.com/JarnoRalli/hourglass-c-api/actions/workflows/visual_studio_build.yml/badge.svg)   ![Code style](https://github.com/JarnoRalli/hourglass-c-api/actions/workflows/code_style.yml/badge.svg)

# 1 Hourglass C API for C++ Library

This repository showcases a pattern/technique called *Hourglass* for implementing C APIs, and header only C++ APIs, for libraries implemented in C++.
The problem that this pattern solves is the lack of standard ABI in C++. What this means is that each compiler, and even different versions of the same compiler,
can have non-compatible ABIs. What this means in practise is that if you compile a C++ library, that exposes functionality of the library using a C++ API, using one compiler, and then
you compile and link the application code using another compiler, you most likely will end up having linking problems. Maintainers of commercial C++ libraries, 
where only binaries are distributed, solve this problem by implementing C APIs for their libraries.

<figure align="center">
    <img src="./images/shutterstock_384007933.jpg" width="600">
    <figcaption><b>Hourglass technique for avoiding ABI problems.</b></figcaption>
</figure>

This is inconvenient, since C lacks the expressive power of C++, meaning that you force the user of the C++ library to interact 
with it using C. The *Hourglass* technique solves this problem by implementing a header only C++ API, that is based on the C API, allowing users 
of the library to interact with it using C++. This is also where the pattern gets its name from. *Hourglass* technique first narrows C++ to C and 
then widens (expressiveness) back to C++. The original concept of the *Hourglass* C API technique is from Stefanus DuToit. 
He discusses it in this [video](https://www.youtube.com/watch?v=PVYdHDm0q6Y) from the 2014 CppCon.

# 2 Implementation

This repository implements a `LibStoryBoard` library that implements a storyboard object that contains note objects. Each note exposes the following functions:

* `std::string getTitle()`
* `std::string getText()`
* `std::vector<std::string> getTags()`

, whereas a storyboard exposes the following functions:

* `void addNote(const Note& newNote)`
* `void addNote(Note&& newNote)`
* `int deleteNote(const Note& deleteMe)`
* `int searchByTitle(const std::string& title, note_cont_t& container)`
* `int searchByText(const std::string& text, note_cont_t& container)`
* `int searchByTag(const tag_cont_t& tags, note_cont_t& container)`

The library, after having been built and installed using cmake, exposes both C- and C++-APIs. The C++ API is a header only, based on the C API. Original C++
objects are not exposed directly.

## 2.1 Hourglass C++ API and STL-Containers

One problem that arises when you go from C++ to C, and back to C++, is related to containers and the STL-library. The STL-library implements
many different containers, like the std::vector. The problem is how to deal with API calls that return arrays. The *Hourglass* pattern 
deals with this in a quite innovative way: it calls a callback function for each element in the container and let's the application code decide what to do.
One of the parameters given for the callback function is *client data*. This can be, for example, a pointer to a std::vector that then calls *push_back* for each element.

Let's take a look at how this is implemented in the *Hourglass* pattern. `LibStoryBoard` implements note- and storyboard classes in C++. The library
has both C API and C++ API (header only, based on the C API). Following is a simplification of the `Note` object from the library, implemented in C++.
Actual implementation can be found in [storyboard.cpp](./libstoryboard/src/storyboard.cpp).


```cpp
class Note
{
public:
    std::vector<std::string> getTags();
private:
    std::vector<std::string> tags_container;
}
```

The C API, available at [storyboardCppAPI.hpp](./libstoryboard/include/storyboard/storyboardCppAPI.hpp),  defines a `note_query_handler` 
function pointer that takes a `void*` pointer and a `const char*` pointer. `note_get_tags` is the function, defined in the C API, that is 
used for querying the tags of a given `note_in`, where the `note_in` is an opaque pointer.

```cpp
typedef void (*note_query_handler)(void* client_data, const char* tag);
int32_t note_get_tags(const note_t note_in, note_query_handler handler, void* client_data, error_t_* out_error);
```

Implementation of `note_get_tags` calls the `handler` function for each element of the `std::vector`
returned by the `getTags()` function.

```cpp
int32_t note_get_tags(const note_t note_in, note_query_handler handler, void* client_data, error_t_* out_error)
{
    if (!note_in)
    {
        *out_error = new error{"note_in not initialized"};
        return 0;
    }

    translateExceptions(out_error, [&] {
        std::vector<std::string> query_result(note_in->actual.getTags());

        for (auto&& elem : query_result)
        {
            handler(client_data, elem.c_str());
        }
    });

    return note_in->actual.getTags().size();
}
```

It is left to the user of the C API to decide what the `handler` function should do in the application code. If, for example, he/she wants it to
print the tags, the `handler` function could be as follows:

```cpp
void query_tags(void* client_data, const char* tag)
{
    printf("%s ", tag);
}
```

Example of application code that uses the C API is available from [c_executable.c](./c_executable/src/c_executable.c).


Now, using this very same C API, we have implemented a header only C++ API, available at [storyboardCppAPI.hpp](/libstoryboard/include/storyboard/storyboardCppAPI.hpp), 
that uses this same approach for querying note's tags as follows:

```cpp
class Note
{
public:
    std::vector<std::string> getTags() const
    {
        std::vector<std::string> tags_container;

        auto callback = [](void* client_data, const char* tag) {
            ((std::vector<std::string>*)client_data)->push_back(std::string(tag));
        };

        note_get_tags(m_opaque, callback, &tags_container, ThrowOnError{});

        return tags_container;
    }
}
```

# 3 Directories

This repository contains the following directories.

* [LibStoryBoard](libstoryboard/README.md)
  * *LibStoryBoard* library is written in C++, but it also has a C API, implemented using the hourglass approach.
  * Uses modern CMake as a build system
* [C executable](c_executable/README.md)
  * A C executable that uses *LibStoryBoard* via the C API.
  * Uses modern CMake as a build system
* [C++ executable](cpp_executable/README.md)
  * A C++ executable that uses *LibStoryBoard* via the hourglass C++ API.
  * Uses modern CMake as a build system

You first need to build and install the *LibStoryBoard* library and then build either one of the executables for testing. The library
and the executables use modern CMake as the build system.

# 4 Scripts

* [build.sh](build.sh)
  * Builds everything and executes the C and C++ executables. You need to install C and C++ compilers, and cmake>=3.24 before running the script.
* [format_code.sh](format_code.sh)
  * Runs `clang-format` on all the c, cpp, h and hpp files. Intended to be used before committing code. GitHub actions uses Ubuntu 20.04 to test the code in the PRs.
