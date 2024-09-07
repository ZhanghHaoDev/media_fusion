add_library(file_analysis STATIC
    wav_file.cc
    aac_file.cc
)

find_package(spdlog REQUIRED)

target_link_libraries(file_analysis PUBLIC 
    spdlog::spdlog
)