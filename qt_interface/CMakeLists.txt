set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_library(qt_interface STATIC
    main_window.cc
)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

target_link_libraries(qt_interface PUBLIC 
    Qt6::Widgets)