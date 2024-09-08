add_library(file_management STATIC
    wav_file.cc
    aac_file.cc
    file_management.cc
)

find_package(spdlog REQUIRED)

target_link_libraries(file_management PUBLIC 
    spdlog::spdlog
    "/opt/homebrew/Cellar/ffmpeg/7.0.2/lib/libavformat.dylib"
    "/opt/homebrew/Cellar/ffmpeg/7.0.2/lib/libavcodec.dylib"
    "/opt/homebrew/Cellar/ffmpeg/7.0.2/lib/libavutil.dylib"
    "/opt/homebrew/Cellar/ffmpeg/7.0.2/lib/libswresample.dylib"
)