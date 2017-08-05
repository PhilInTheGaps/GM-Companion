cmake_minimum_required(VERSION 3.0.2)
project(GM-Companion)

find_package(Qt5Widgets)
find_package(Qt5Multimedia)
find_package(Qt5MultimediaWidgets)
find_package(Qt5Network)
find_package(Qt5Sql)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(mainwindow mainwindow.cpp)
target_link_libraries (mainwindow Qt5::Widgets)

add_executable(gm-companion main.cpp)
target_link_libraries (gm-companion mainwindow)

qt5_use_modules(gm-companion core gui widgets multimedia multimediawidgets sql network)
