find_package(PCL REQUIRED)
find_package(OpenCV HINTS /usr/local/opencv3/ /usr/local/opencv3/ REQUIRED)
#find_package(OpenCV REQUIRED)
find_package(VTK REQUIRED)
find_package(Eigen REQUIRED)
find_package(Boost 1.40 COMPONENTS program_options REQUIRED )

set(Caffe_DIR /home/amax/cxt/open_detection/packages/ssd/build)
find_package(Caffe REQUIRED)
include_directories(${Caffe_INCLUDE_DIRS})
add_definitions(${Caffe_DEFINITIONS})    # ex. -DCPU_ONLY
#target_link_libraries(${Caffe_LIBRARIES})

include_directories("${OD_SOURCE_DIR}" ${EIGEN_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS} ${PCL_INCLUDE_DIRS} ${OD_SOURCE_DIR}/3rdparty/SiftGPU/src/SiftGPU)
