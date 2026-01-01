project(piperncnn)

cmake_minimum_required(VERSION 3.10)

#set(ncnn_DIR ${CMAKE_SOURCE_DIR}/ncnn-20250503-android-vulkan/${ANDROID_ABI}/lib/cmake/ncnn)
if(DEFINED ncnn_DIR_FROM_GRADLE)
    message(STATUS "Using NCNN path from Gradle: ${ncnn_DIR}")
    #    set(ncnn_DIR ${ncnn_DIR_FROM_GRADLE}/${ANDROID_ABI}/lib/cmake/ncnn)
    set(ncnn_DIR ${CMAKE_SOURCE_DIR}/ncnn-android-vulkan/${ANDROID_ABI}/lib/cmake/ncnn)
else()
    message(WARNING "ncnn_DIR_FROM_GRADLE not set. Falling back to default path. Please sync project with Gradle.")
    set(ncnn_DIR ${CMAKE_SOURCE_DIR}/ncnn-android-vulkan/${ANDROID_ABI}/lib/cmake/ncnn)
endif()

find_package(ncnn REQUIRED)

add_library(piperncnn SHARED piperncnn.cpp piper.cpp simpleg2p.cpp audioplayer.cpp)

target_link_libraries(piperncnn ncnn OpenSLES)
