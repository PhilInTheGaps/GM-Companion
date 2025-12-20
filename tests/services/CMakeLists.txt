cmake_minimum_required(VERSION 3.19)

set(SERVICES_TESTS
    main.cpp
    rest/testcallbackserver.cpp
    rest/testrestreply.cpp
    rest/testrestrequest.cpp
    spotify/testspotifyutils.cpp
)

if(WITH_YOUTUBE)
    list(APPEND SERVICES_TESTS
        youtube/testyoutube.cpp
        youtube/testpipedconnector.cpp
        youtube/testvideoid.cpp
        youtube/testplaylistid.cpp
    )
endif()

qt_add_executable(test_services ${SERVICES_TESTS})

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
