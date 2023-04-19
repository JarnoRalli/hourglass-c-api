include(FetchContent)

FetchContent_Declare(googletest
                     GIT_REPOSITORY https://github.com/google/googletest.git
                     GIT_TAG        b796f7d44681514f58a683a3a71ff17c94edb0c1 #v1.13.0
                     OVERRIDE_FIND_PACKAGE)

FetchContent_MakeAvailable(googletest)
