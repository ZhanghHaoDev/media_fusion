#include "aac_file.h"

#include "spdlog/spdlog.h"

aac_file::aac_file(const std::string file_path){
    spdlog::info("aac file path: {}", file_path);
}

aac_file::~aac_file(){

}