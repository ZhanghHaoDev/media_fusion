include_directories(
    ${CMAKE_SOURCE_DIR}/file_info
)

enable_testing()

add_executable(media_fusion_test
    "main_test.cc"
    "./file_info_test.cc"
)

target_link_libraries(media_fusion_test PUBLIC  
    gtest_main
    file_info
)

include(GoogleTest)
gtest_discover_tests(media_fusion_test)