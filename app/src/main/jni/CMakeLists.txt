project(piperncnn)

cmake_minimum_required(VERSION 3.10)

set(ncnn_DIR ${CMAKE_SOURCE_DIR}/ncnn-20250503-android-vulkan/${ANDROID_ABI}/lib/cmake/ncnn)
find_package(ncnn REQUIRED)

add_library(piperncnn SHARED piperncnn.cpp piper.cpp simpleg2p.cpp audioplayer.cpp)

target_link_libraries(piperncnn ncnn OpenSLES)
