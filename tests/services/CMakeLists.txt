cmake_minimum_required(VERSION 3.19)

qt_add_executable(test_services
    main.cpp
    rest/testcallbackserver.cpp
    spotify/testspotifyutils.cpp
    youtube/testyoutube.cpp
    youtube/testpipedconnector.cpp
    youtube/testvideoid.cpp
    youtube/testplaylistid.cpp
)

target_link_libraries(test_services PRIVATE
    propertyhelper
    common
    services
    testhelper
    Qt6::Core
    Qt6::Gui
    Qt6::Network
    Qt6::Test
    gtest
)

target_precompile_headers(test_services REUSE_FROM common)

gtest_discover_tests(test_services)
