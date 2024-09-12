include_directories(
    ${CMAKE_SOURCE_DIR}/qt_interface
    ${CMAKE_SOURCE_DIR}/file_management
)

add_executable(qt_interface_test
    qt_interface_test.cc
)

find_package(fmt REQUIRED)

target_link_libraries(qt_interface_test PUBLIC
    fmt::fmt
    qt_interface
)

################################################################################

add_executable(file_management_test
    file_management_test.cc
)

target_link_libraries(file_management_test PUBLIC
    file_management
    gtest
)

enable_testing()
add_test(NAME file_management_test COMMAND file_management_test)