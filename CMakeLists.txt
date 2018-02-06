#设置Cmake编译版本
cmake_minimum_required (VERSION 3.0)

include_directories(${CMAKE_SOURCE_DIR}/include)
if("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "armv7-a")
include_directories(${CMAKE_SOURCE_DIR}/third_party/ncnn-android-lib/include)
link_directories(${CMAKE_SOURCE_DIR}/third_party/ncnn-android-lib/armeabi-v7a/)
elseif("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "aarch64")
include_directories(${CMAKE_SOURCE_DIR}/third_party/ncnn-android-lib/include)
link_directories(${CMAKE_SOURCE_DIR}/third_party/ncnn-android-lib/arm64-v8a/)
endif()
set(OpenCV_DIR ${CMAKE_SOURCE_DIR}/third_party/OpenCV-android-sdk/sdk/native/jni)
find_package(OpenCV REQUIRED)
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -std=c++11")
aux_source_directory(${CMAKE_SOURCE_DIR}/src/ ssd_src)

find_package(OpenMP)
if(OpenMP_CXX_FOUND OR OPENMP_FOUND)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

add_library(ssdmobilenet SHARED ${ssd_src})
target_link_libraries(ssdmobilenet ncnn ${OpenCV_LIBS} )


