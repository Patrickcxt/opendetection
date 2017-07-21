cmake_minimum_required(VERSION 2.8)
PROJECT(Test)

find_package(Caffe REQUIRED)
include_directories(${Caffe_INCLUDE_DIRS})
add_definitions(${Caffe_DEFINITIONS})    # ex. -DCPU_ONLY

add_executable(Test test.cpp ODDatasets.cpp PascalVOC.cpp ImageNet.cpp MSCoco.cpp)
target_link_libraries(Test ${Caffe_LIBRARIES} boost_system boost_filesystem)

