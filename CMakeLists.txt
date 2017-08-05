cmake_minimum_required(VERSION 2.8.11)

project(GM-Companion)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the Qt libraries
find_package(Qt5Widgets)
find_package(Qt5Core)
find_package(Qt5Network)
find_package(Qt5Sql)
find_package(Qt5Gui)

# Tell CMake to create the gm-companion executable
add_executable(gm-companion WIN32 main.cpp)

# Use the modules from Qt 5.
target_link_libraries(gm-companion Qt5::Widgets)
target_link_libraries(gm-companion Qt5::Core)
target_link_libraries(gm-companion Qt5::Network)
target_link_libraries(gm-companion Qt5::Sql)
target_link_libraries(gm-companion Qt5::Gui)
