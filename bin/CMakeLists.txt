include_directories(
    ${CMAKE_SOURCE_DIR}/qt_interface
    ${CMAKE_SOURCE_DIR}/file_management
)

add_executable(media_fusion_app
    main.cc
)

target_link_libraries(media_fusion_app PUBLIC
    qt_interface
    file_management
)