cmake_minimum_required(VERSION 3.25)

project(iris LANGUAGES CXX)

add_executable(iris
  src/bounds3.cpp
  src/camera.cpp
  src/image.cpp
  src/intersection.cpp
  src/main.cpp
  src/material.cpp
  src/mesh.cpp
  src/plane.cpp
  src/random.cpp
  src/ray.cpp
  src/primitive.cpp
  src/scene.cpp
  src/sphere.cpp
  src/triangle.cpp
  src/vec3.cpp
  )
  
target_include_directories(iris PRIVATE src dependencies)

target_compile_features(iris PUBLIC cxx_std_20)
set_target_properties(iris PROPERTIES CXX_EXTENSIONS OFF)

add_library(commandLine dependencies/commandLine.cpp)
target_link_libraries(iris commandLine)

# LINK EXTERNAL LIBRARIES
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
set(ASSIMP_BUILD_TESTS OFF)
target_link_libraries(iris assimp)

find_package(glog 0.6.0 REQUIRED)
target_link_libraries(iris glog::glog)

find_package(OpenEXR REQUIRED)
target_link_libraries(iris OpenEXR::OpenEXR)

find_package(OpenMP)
target_link_libraries(iris OpenMP::OpenMP_CXX)

# INCLUDE GOOGLE TEST
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# TESTS
enable_testing()

add_executable(test_vec3 tests/test_vec3.cpp src/vec3.cpp)
target_link_libraries(test_vec3 GTest::gtest_main)
target_include_directories(test_vec3 PRIVATE src dependencies)

add_executable(test_bounds3 tests/test_bounds3.cpp src/bounds3.cpp src/vec3.cpp)
target_link_libraries(test_bounds3 GTest::gtest_main)
target_include_directories(test_bounds3 PRIVATE src dependencies)

include(GoogleTest)
gtest_discover_tests(test_vec3 test_bounds3)

