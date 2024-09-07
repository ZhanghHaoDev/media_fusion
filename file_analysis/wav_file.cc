#include "wav_file.h"

#include "spdlog/spdlog.h"

wav_file::wav_file(const std::string file_path){
    spdlog::info("wav file path: {}", file_path);
}

wav_file::~wav_file(){

}