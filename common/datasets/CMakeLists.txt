cmake_minimum_required(VERSION 2.8)
PROJECT(Test)

set(Caffe_DIR /home/amax/cxt/open_detection/packages/ssd/build)
find_package(Caffe REQUIRED)
include_directories(${Caffe_INCLUDE_DIRS})
add_definitions(${Caffe_DEFINITIONS})    # ex. -DCPU_ONLY

add_executable(Test test_lmdb.cpp ODDatasets.cpp PascalVOC.cpp)
#add_executable(Test test_lmdb.cpp ODDatasets.cpp PascalVOC.cpp ImageNet.cpp MSCoco.cpp)
target_link_libraries(Test ${Caffe_LIBRARIES} boost_system boost_filesystem)

