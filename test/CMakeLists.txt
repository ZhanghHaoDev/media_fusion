include_directories(
    ${CMAKE_SOURCE_DIR}/file_management
)

enable_testing()

add_executable(media_fusion_test
    "./file_management_test/aac_file_test.cc"
)

target_link_libraries(media_fusion_test PUBLIC
    gtest_main
    file_management
)

include(GoogleTest)
gtest_discover_tests(media_fusion_test)