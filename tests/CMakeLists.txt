cmake_minimum_required(VERSION 3.19)

project(tests)

include(GoogleTest)
add_definitions(-DGTEST_LANGUAGE_CXX14)

find_package(Threads REQUIRED)

add_subdirectory(testhelper)
add_subdirectory(common)
add_subdirectory(filesystem)
add_subdirectory(services)

set(AUDIO_THUMBNAILS
    tools/audio/thumbnails/testaudiothumbnail.cpp
    tools/audio/thumbnails/loaders/testtagimageloader.cpp
    tools/audio/thumbnails/loaders/testfileimageloader.cpp
)

# web loader
# somehow this always fails using github actions (windows) even though the compiled executable works on other machines
# so we disable this test for now
if(NOT (IS_RUNNING_IN_CI AND WIN32))
    list(APPEND AUDIO_THUMBNAILS tools/audio/thumbnails/loaders/testwebimageloader.cpp)
endif()

set_source_files_properties(${AUDIO_THUMBNAILS} PROPERTIES UNITY_GROUP "audio_thumbnails")

set(SOURCES
    main.cpp
    addons/testaddonmanager.cpp
    addons/testaddonreader.cpp
    tools/audio/testaudioproject.cpp
    tools/audio/testaudioprojectupgrader.cpp
    tools/audio/testaudiosaveload.cpp
    tools/audio/testaudioaddons.cpp
    tools/audio/testaudioplaylist.cpp
    tools/converter/testconverterprojectupgrader.cpp
    tools/converter/testconvertertool.cpp
    tools/converter/testconvertereditor.cpp
    tools/shop/testshopproject.cpp
    tools/shop/testshoptool.cpp
    tools/shop/testshopeditor.cpp
    tools/notes/testhtmlgenerator.cpp
    tools/notes/testnotebook.cpp
    tools/notes/testnotebookchapter.cpp
    tools/notes/testnotebookpage.cpp
    tools/notes/testnotestool.cpp
    tools/testcombattracker.cpp
    tools/testnamegenerator.cpp
    ${AUDIO_THUMBNAILS}
)

qt_add_executable(test_src ${SOURCES})

set_target_properties(test_src PROPERTIES
    UNITY_BUILD_MODE GROUP)

target_link_libraries(test_src PUBLIC
    gm-companion-lib
    testhelper
    Qt6::Core
    Qt6::Gui
    Qt6::Network
    Qt6::Test
    gtest
)

target_precompile_headers(test_src REUSE_FROM common)

gtest_discover_tests(test_src)
