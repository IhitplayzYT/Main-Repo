cmake_minimum_required(VERSION 3.8)
project(rover)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()


find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)

add_executable(gps src/gps.cpp)
ament_target_dependencies(gps rclcpp sensor_msgs geometry_msgs tf2_geometry_msgs)

add_executable(gps2 src/gps2.cpp)
ament_target_dependencies(gps2 rclcpp sensor_msgs geometry_msgs tf2_geometry_msgs)

install(
  DIRECTORY launch model rviz src
  DESTINATION share/${PROJECT_NAME}
)

install(TARGETS
  gps
  DESTINATION lib/${PROJECT_NAME})

  install(TARGETS
  gps2
  DESTINATION lib/${PROJECT_NAME})



if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
 
  set(ament_cmake_copyright_FOUND TRUE)
  
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
