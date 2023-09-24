cmake_minimum_required(VERSION 3.19)

set_source_files_properties(testnextcloudaccess.cpp PROPERTIES
    SKIP_UNITY_BUILD_INCLUSION TRUE
)

set(SOURCES
    main.cpp
    abstractaccesstest.h
    abstractaccesstest.cpp
    mocknextcloud.h
    mocknextcloud.cpp
    oputils.h
    oputils.cpp
    testlocalaccess.cpp
    testfiledialog.cpp
    testnextcloudaccess.cpp
)

if (ENABLE_EXTRA_TESTS)
    set_source_files_properties(testgoogledriveaccess.cpp PROPERTIES
        SKIP_UNITY_BUILD_INCLUSION TRUE
    )

    list(APPEND SOURCES
        testgoogledriveaccess.cpp
    )
endif()

qt_add_executable(test_filesystem ${SOURCES})

target_link_libraries(test_filesystem PUBLIC
    testhelper
    filesystem
    filesystemplugin
    gm-companion-ui-filedialog
    gm-companion-ui-filedialogplugin
    Qt6::Core
    Qt6::Gui
    Qt6::Network
    Qt6::Test
    gtest
)

target_precompile_headers(test_filesystem REUSE_FROM common)

gtest_discover_tests(test_filesystem)
