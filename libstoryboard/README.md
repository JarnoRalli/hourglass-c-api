# LibStoryBoard

`LibStoryBoard` implements a simple `Storyboard` class that can be used for adding, deleting and querying
of notes posted in the `Storyboard`. The library exposes one target, called `LibStoryBoard::LibStoryBoard`,
that can be linked against in another project in order to get access to the `Storyboard` functionality.

# Building and Installing

The library needs to be built and installed before it can be used in another project. Following commands build- and
install the library, and execute the tests.

```bash
cd libstorybuild
mkdir build
cd build
conan install ../conan -s build_type=Release --build=missing
mkdir install
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=./install
make -j4
make install
make test
```

