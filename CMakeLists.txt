cmake_minimum_required(VERSION 2.8.11)
project(GM-Companion)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Widgets finds its own dependencies.
find_package(Qt5 REQUIRED
    Core
    Gui
    Widgets
    Multimedia
    MultimediaWidgets
    Sql
    Network
)

add_executable(gm-companion main.cpp mainwindow.cpp)

qt5_use_modules(gm-companion widgets multimedia multimediawidgets sql network)

# Tell CMake to create the gm-companion executable
add_executable(gm-companion main.cpp)
