cmake_minimum_required(VERSION 2.8.11)
set(CMAKE_CXX_STANDARD 14)

project(GM-Companion)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the Qt libraries
find_package(Qt5Widgets)
foreach(plugin ${Qt5Widgets_PLUGINS})
  get_target_property(_loc ${plugin} LOCATION)
  message("Plugin ${plugin} is at location ${_loc}")
endforeach()

find_package(Qt5Core)
foreach(plugin ${Qt5Core_PLUGINS})
  get_target_property(_loc ${plugin} LOCATION)
  message("Plugin ${plugin} is at location ${_loc}")
endforeach()

find_package(Qt5Network)
foreach(plugin ${Qt5Network_PLUGINS})
  get_target_property(_loc ${plugin} LOCATION)
  message("Plugin ${plugin} is at location ${_loc}")
endforeach()

find_package(Qt5Sql)
foreach(plugin ${Qt5Sql_PLUGINS})
  get_target_property(_loc ${plugin} LOCATION)
  message("Plugin ${plugin} is at location ${_loc}")
endforeach()

find_package(Qt5Gui)
foreach(plugin ${Qt5Gui_PLUGINS})
  get_target_property(_loc ${plugin} LOCATION)
  message("Plugin ${plugin} is at location ${_loc}")
endforeach()

# Tell CMake to create the gm-companion executable
add_executable(gm-companion WIN32 main.cpp)

# Use the modules from Qt 5.
target_link_libraries(gm-companion Qt5::Widgets)
target_link_libraries(gm-companion Qt5::Core)
target_link_libraries(gm-companion Qt5::Network)
target_link_libraries(gm-companion Qt5::Sql)
target_link_libraries(gm-companion Qt5::Gui)
