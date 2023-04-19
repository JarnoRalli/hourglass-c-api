# ------------------
# Storyboard tests
# ------------------

find_package(googletest REQUIRED CONFIG)
include(GoogleTest)

message(STATUS "-----")
message(STATUS "TESTS")
message(STATUS "-----")

# Test 1
message(STATUS "\tAdding test testCAPI")
add_executable(testCAPI src/testCAPI.cpp)
target_link_libraries(testCAPI PRIVATE LibStoryBoard GTest::gtest)
add_test(NAME testCAPI COMMAND testCAPI WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set_property(TARGET testCAPI PROPERTY FOLDER "Tests")

# Test 2
message(STATUS "\tAdding test testCppAPI")
add_executable(testCppAPI src/testCppAPI.cpp)
target_link_libraries(testCppAPI PRIVATE LibStoryBoard GTest::gtest)
add_test(NAME testCppAPI COMMAND testCppAPI WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set_property(TARGET testCppAPI PROPERTY FOLDER "Tests")
