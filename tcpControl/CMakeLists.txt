cmake_minimum_required(VERSION 2.8.3)
project(tcpControl)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  stdmsgs
)
## Generate messages in the 'msg' folder
# add_message_files(
#   FILES
#   Message1.msg
#   Message2.msg
# )

## Generate services in the 'srv' folder
# add_service_files(
#   FILES
#   Service1.srv
#   Service2.srv
# )

## Generate actions in the 'action' folder
# add_action_files(
#   FILES
#   Action1.action
#   Action2.action
# )

## Generate added messages and services with any dependencies listed here
# generate_messages(
#   DEPENDENCIES
#   std_msgs  # Or other packages containing msgs
# )

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES tcpControl
  CATKIN_DEPENDS roscpp stdmsgs
  DEPENDS system_lib
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(tcpControl_node src/tcpControl_node.cpp)
add_dependencies(tcpControl_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(tcpControl_node
   ${catkin_LIBRARIES}
)
