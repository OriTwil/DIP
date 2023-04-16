cmake_minimum_required(VERSION 2.8.3)
project(probot_vision)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  cv_bridge
  image_transport
  moveit_core
  moveit_ros_planning
  moveit_ros_planning_interface
  moveit_visual_tools
  roscpp
  sensor_msgs
  std_msgs
  tf
)

## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)
find_package(OpenCV REQUIRED)

## Uncomment this if the package has a setup.py. This macro ensures
## modules and global scripts declared therein get installed
## See http://ros.org/doc/api/catkin/html/user_guide/setup_dot_py.html
# catkin_python_setup()

################################################
## Declare ROS messages, services and actions ##
################################################

## To declare and build messages, services or actions from within this
## package, follow these steps:
## * Let MSG_DEP_SET be the set of packages whose message types you use in
##   your messages/services/actions (e.g. std_msgs, actionlib_msgs, ...).
## * In the file package.xml:
##   * add a build_depend tag for "message_generation"
##   * add a build_depend and a exec_depend tag for each package in MSG_DEP_SET
##   * If MSG_DEP_SET isn't empty the following dependency has been pulled in
##     but can be declared for certainty nonetheless:
##     * add a exec_depend tag for "message_runtime"
## * In this file (CMakeLists.txt):
##   * add "message_generation" and every package in MSG_DEP_SET to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * add "message_runtime" and every package in MSG_DEP_SET to
##     catkin_package(CATKIN_DEPENDS ...)
##   * uncomment the add_*_files sections below as needed
##     and list every .msg/.srv/.action file to be processed
##   * uncomment the generate_messages entry below
##   * add every package in MSG_DEP_SET to generate_messages(DEPENDENCIES ...)

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

################################################
## Declare ROS dynamic reconfigure parameters ##
################################################

## To declare and build dynamic reconfigure parameters within this
## package, follow these steps:
## * In the file package.xml:
##   * add a build_depend and a exec_depend tag for "dynamic_reconfigure"
## * In this file (CMakeLists.txt):
##   * add "dynamic_reconfigure" to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * uncomment the "generate_dynamic_reconfigure_options" section below
##     and list every .cfg file to be processed

## Generate dynamic reconfigure parameters in the 'cfg' folder
# generate_dynamic_reconfigure_options(
#   cfg/DynReconf1.cfg
#   cfg/DynReconf2.cfg
# )

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if your package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES probot_vision
#  CATKIN_DEPENDS roscpp
#  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

## Declare a C++ library
# add_library(${PROJECT_NAME}
#   src/${PROJECT_NAME}/probot_vision.cpp
# )

## Add cmake target dependencies of the library
## as an example, code may need to be generated before libraries
## either from message generation or dynamic reconfigure
# add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Declare a C++ executable
## With catkin_make all packages are built within a single CMake context
## The recommended prefix ensures that target names across packages don't collide
# add_executable(${PROJECT_NAME}_node src/probot_vision_node.cpp)

## Rename C++ executable without prefix
## The above recommended prefix causes long target names, the following renames the
## target back to the shorter version for ease of user use
## e.g. "rosrun someones_pkg node" instead of "rosrun someones_pkg someones_pkg_node"
# set_target_properties(${PROJECT_NAME}_node PROPERTIES OUTPUT_NAME node PREFIX "")

## Add cmake target dependencies of the executable
## same as for the library above
# add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Specify libraries to link a library or executable target against
# target_link_libraries(${PROJECT_NAME}_node
#   ${catkin_LIBRARIES}
# )

include_directories(${PROJECT_NAME}/opencv-3.3.1-dev/opencv)
include_directories(${PROJECT_NAME}/opencv-3.3.1-dev/opencv2)
LINK_DIRECTORIES(${PROJECT_NAME}/opencvlib-all)
LINK_DIRECTORIES(${PROJECT_NAME}/opencvlib-all)
link_libraries(
opencv_calib3d
opencv_core
opencv_dnn
opencv_features2d
opencv_flann
opencv_highgui
opencv_imgcodecs
opencv_imgproc
opencv_ml
opencv_objdetect
opencv_photo
opencv_shape
opencv_stitching
opencv_superres
opencv_video
opencv_videoio
opencv_videostab
opencv_viz
opencv_aruco
opencv_bgsegm
opencv_bioinspired
opencv_ccalib
opencv_cvv
opencv_datasets
opencv_dpm
opencv_face
opencv_fuzzy
opencv_hdf
opencv_img_hash
opencv_line_descriptor
opencv_optflow
opencv_phase_unwrapping
opencv_plot
opencv_reg
opencv_rgbd
opencv_saliency
opencv_stereo
opencv_structured_light
opencv_surface_matching
opencv_text
opencv_tracking
opencv_xfeatures2d
opencv_ximgproc
opencv_xobjdetect
opencv_xphoto


)   
LINK_DIRECTORIES("/opt/ros/kinetic/lib")
#LINK_DIRECTORIES("/home/feibot/dashgo_ws/devel/lib")
LINK_DIRECTORIES("/usr/lib/x86_64-linux-gnu")
LINK_DIRECTORIES("/usr/lib/x86_64-linux-gnu"  "/usr/lib" "/opt/ros/kinetic/lib/x86_64-linux-gnu/")
link_libraries(
cv_bridge
opencv_core3
opencv_imgproc3
opencv_imgcodecs3
moveit_common_planning_interface_objects
moveit_planning_scene_interface
moveit_move_group_interface
moveit_warehouse
warehouse_ros
moveit_pick_place_planner
moveit_move_group_capabilities_base
moveit_visual_tools
rviz_visual_tools
rviz_visual_tools_gui
rviz_visual_tools_remote_control
rviz_visual_tools_imarker_simple
#tf_conversions
#kdl_conversions
moveit_rdf_loader
moveit_kinematics_plugin_loader
moveit_robot_model_loader
moveit_constraint_sampler_manager_loader
moveit_planning_pipeline
moveit_trajectory_execution_manager
moveit_plan_execution
moveit_planning_scene_monitor
moveit_collision_plugin_loader
chomp_motion_planner
moveit_lazy_free_space_updater
moveit_point_containment_filter
moveit_occupancy_map_monitor
moveit_pointcloud_octomap_updater_core
moveit_semantic_world
image_transport
tinyxml2
class_loader
PocoFoundation
dl
roslib
rospack
python2.7
boost_program_options
moveit_exceptions
moveit_background_processing
moveit_kinematics_base
moveit_robot_model
moveit_transforms
moveit_robot_state
moveit_robot_trajectory
moveit_planning_interface
moveit_collision_detection
moveit_collision_detection_fcl
moveit_kinematic_constraints
moveit_planning_scene
moveit_constraint_samplers
moveit_planning_request_adapter
moveit_profiler
moveit_profiler
moveit_trajectory_processing
moveit_distance_field
moveit_collision_distance_field
moveit_kinematics_metrics
moveit_dynamics_solver
moveit_utils
boost_iostreams
fcl
eigen_conversions
geometric_shapes
octomap
octomath
kdl_parser
orocos-kdl
urdf
urdfdom_sensor
urdfdom_model_state
urdfdom_model
urdfdom_world
tinyxml
rosconsole_bridge
random_numbers
srdfdom
tf
tf2_ros
actionlib
message_filters
roscpp
boost_filesystem
boost_signals
xmlrpcpp
tf2
roscpp_serialization
rosconsole
rosconsole_log4cxx
rosconsole_backend_interface
log4cxx
boost_regex
rostime
cpp_common
boost_system
boost_thread
boost_chrono
boost_date_time
boost_atomic
pthread
console_bridge
)


add_executable(image_converter src/image_converter.cpp)
#target_link_libraries(image_converter ${catkin_LIBRARIES} ${OpenCV_LIBS})


add_executable(visonManager src/vision_manager.cpp)
#target_link_libraries(visonManager ${catkin_LIBRARIES} ${OpenCV_LIBS})

add_executable(image_correct src/image_correct.cpp)
#target_link_libraries(image_correct ${catkin_LIBRARIES} ${OpenCV_LIBS})

#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
# install(PROGRAMS
#   scripts/my_python_script
#   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark executables and/or libraries for installation
# install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}_node
#   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark cpp header files for installation
# install(DIRECTORY include/${PROJECT_NAME}/
#   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
#   FILES_MATCHING PATTERN "*.h"
#   PATTERN ".svn" EXCLUDE
# )

## Mark other files for installation (e.g. launch and bag files, etc.)
# install(FILES
#   # myfile1
#   # myfile2
#   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
# )

#############
## Testing ##
#############

## Add gtest based cpp test target and link libraries
# catkin_add_gtest(${PROJECT_NAME}-test test/test_probot_vision.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)
