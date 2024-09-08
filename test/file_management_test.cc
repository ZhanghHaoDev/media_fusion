#include "aac_file.h"
#include <iostream>

int main() {
    aac_file file("/Users/zhanghao/code/cpp/media_fusion/tests_files/test.aac");

    if (file.is_aac_file()) {
        std::cout << "这个文件是一个AAC文件" << std::endl;
    } else {
        std::cout << "这个文件不是一个AAC文件" << std::endl;
    }

    aac_file::aac_file_info info = file.get_aac_file_info();
    std::cout << "文件路径: " << info.file_path << std::endl;
    std::cout << "是否是AAC文件: " << (info.is_aac ? "是" : "否") << std::endl;
    std::cout << "采样率: " << info.sample_rate << std::endl;
    std::cout << "声道数: " << info.channels << std::endl;
    std::cout << "比特率: " << info.bit_rate << std::endl;
    std::cout << "封装格式: " << (info.format == 0 ? "ADTS" : "ADIF") << std::endl;
    std::cout << "编码配置: " << info.profile << std::endl;
    std::cout << "帧长度: " << info.frame_length << std::endl;
    std::cout << "音频时长: " << info.duration << "秒" << std::endl;

    file.extract_audio("/Users/zhanghao/code/cpp/media_fusion/tests_files/believe.flv", "out.aac");
    file.play_aac_file();

    return 0;
}