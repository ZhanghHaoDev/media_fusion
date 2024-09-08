set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include_directories(
    ${CMAKE_SOURCE_DIR}/file_management
)

add_library(qt_interface STATIC
    main_window.cc
)

find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(spdlog REQUIRED)

target_link_libraries(qt_interface PUBLIC 
    spdlog::spdlog
    Qt6::Widgets
    file_management
)