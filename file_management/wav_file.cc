#include "wav_file.h"

#include "glog/logging.h"

wav_file::wav_file(const std::string file_path){
    if (file_path.empty()){
        LOG(ERROR) << "输入的文件为空 : " << __FILE__ << ", line: " << __LINE__;
        return;
    }
}

wav_file::~wav_file(){

}