cmake_minimum_required(VERSION 3.19)

# Spotify Utils
qt_add_executable(test_spotifyutils testspotifyutils.cpp)
target_link_libraries(test_spotifyutils PRIVATE services Qt6::Test)
set_target_properties(test_spotifyutils PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIR})
add_test(NAME test_spotifyutils COMMAND test_spotifyutils)
