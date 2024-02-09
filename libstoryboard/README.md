# 1 LibStoryBoard

`LibStoryBoard` implements a simple `Storyboard` class that can be used for adding, deleting and querying
of notes posted in the `Storyboard`. The library exposes one target, called `LibStoryBoard::LibStoryBoard`,
that can be linked against in another project in order to get access to the `Storyboard` functionality.

# 2 Building and Installing

The library needs to be built and installed before it can be used in another project. Following commands build, test
and installs the library. The library is installed to `build/install` directory.

```bash
cd libstoryboard
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=./install
cmake --build .
ctest -C Release
cmake --install .
```

