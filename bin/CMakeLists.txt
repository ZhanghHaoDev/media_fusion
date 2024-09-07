include_directories(
    ${CMAKE_SOURCE_DIR}/qt_interface
)

add_executable(media_fusion_app
    main.cc
)

target_link_libraries(media_fusion_app PUBLIC
    qt_interface
)