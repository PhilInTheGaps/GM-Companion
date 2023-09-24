cmake_minimum_required(VERSION 3.19)

set_source_files_properties(models/testtreeitem.cpp PROPERTIES
    SKIP_UNITY_BUILD_INCLUSION TRUE
)

set_source_files_properties(models/testcustomobjectlistmodel.cpp PROPERTIES
    SKIP_UNITY_BUILD_INCLUSION TRUE
)

qt_add_executable(test_common
    main.cpp
    models/testtreeitem.cpp
    models/testcustomobjectlistmodel.cpp
    updates/testupdatemanager.cpp
    utils/testutils.cpp
    utils/teststringutils.cpp
    utils/testfileutils.cpp
    utils/testnetworkutils.cpp
    utils/testmarkdownutils.cpp
    utils/testprocessinfo.cpp
)

target_link_libraries(test_common PRIVATE
    propertyhelper
    common
    ${MARKDOWN_LIBRARY}
    ${QTKEYCHAIN_LIBRARY}
    Qt6::Core
    Qt6::Gui
    Qt6::Network
    Qt6::Test
    gtest
)

target_precompile_headers(test_common REUSE_FROM common)

gtest_discover_tests(test_common)
