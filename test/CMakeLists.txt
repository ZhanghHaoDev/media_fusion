include_directories(
    ${CMAKE_SOURCE_DIR}/qt_interface
    ${CMAKE_SOURCE_DIR}/file_analysis
)

add_executable(qt_interface_test
    qt_interface_test.cc
)

target_link_libraries(qt_interface_test PUBLIC
    qt_interface
)

################################################################################

add_executable(file_analysis_test
    file_analysis_test.cc
)

target_link_libraries(file_analysis_test PUBLIC
    file_analysis
)