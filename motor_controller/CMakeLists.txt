cmake_minimum_required(VERSION 2.8.3)
project(motor_controller)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
)
catkin_package(
	INCLUDE_DIRS include
	CATKIN_DEPENDS roscpp std_msgs
	DEPENDS system_lib
)

include_directories( ${catkin_INCLUDE_DIRS} )

add_executable(motor_controller_node src/motor_controller_node.cpp)
add_dependencies(motor_controller_node motor_controller_generate_messages_cpp)
target_link_libraries(motor_controller_node ${catkin_LIBRARIES})

