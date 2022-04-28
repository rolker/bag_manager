cmake_minimum_required(VERSION 2.8.3)
project(bag_manager)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rosbag
  tf2_msgs
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES 
  CATKIN_DEPENDS roscpp rosbag tf2_msgs
#  DEPENDS system_lib
)

include_directories(
  include
  bag_merge/include
  ${catkin_INCLUDE_DIRS}
)

add_executable(merge_bags bag_merge/src/merge_bags.cpp)

add_dependencies(merge_bags ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

target_link_libraries(merge_bags
    ${catkin_LIBRARIES}
)

add_executable(bag_dedupe_tf bag_dedupe_tf/src/bag_dedupe_tf.cpp)
add_dependencies(bag_dedupe_tf ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

target_link_libraries(bag_dedupe_tf
    ${catkin_LIBRARIES}
)


install(TARGETS merge_bags bag_dedupe_tf
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

## Mark cpp header files for installation
install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
  FILES_MATCHING PATTERN "*.h"
  PATTERN ".svn" EXCLUDE
)

## Mark other files for installation (e.g. launch and bag files, etc.)
# install(FILES
#   # myfile1
#   # myfile2
#   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
# )
