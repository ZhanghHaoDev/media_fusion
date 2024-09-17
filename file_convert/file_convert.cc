#include "file_convert.h"

#include <iostream>
#include <fstream>
#include <string>

#include "glog/logging.h"

extern "C" {
    #include "libavformat/avformat.h"
    #include "libavcodec/avcodec.h"
    #include "libavutil/log.h"
    #include "libavutil/opt.h"
    #include "libswresample/swresample.h"
    #include "libavutil/channel_layout.h"
    #include "libavutil/pixfmt.h"
}

file_convert::file_convert(){
    av_log_set_level(AV_LOG_QUIET);   // 禁用 FFmpeg 日志输出
}

file_convert::~file_convert() = default;

int file_convert::file_open(std::string input_file){
    if (input_file.empty() || !std::ifstream(input_file).is_open()) {
        LOG(ERROR) << "无法打开文件: " << input_file << ", file: " << __FILE__ << ", line: " << __LINE__;
        return -1;
    }

    AVFormatContext* formatContext = avformat_alloc_context();
    if (avformat_open_input(&formatContext, input_file.c_str(), nullptr, nullptr) != 0) {
        LOG(ERROR) << "无法打开文件: " << input_file << " 使用 FFmpeg, file: " << __FILE__ << ", line: " << __LINE__;
        return -1;
    }

    if (avformat_find_stream_info(formatContext, nullptr) < 0) {
        LOG(ERROR) << "无法获取流信息: " << input_file << ", file: " << __FILE__ << ", line: " << __LINE__;
        avformat_close_input(&formatContext);
        return -1;
    }

    for (unsigned int i = 0; i < formatContext->nb_streams; i++) {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            this->has_audio_stream = true;
        }
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            this->has_video_stream = true;
        }
    }

    avformat_close_input(&formatContext);

    if (!has_audio_stream && !has_video_stream) {
        LOG(ERROR) << "文件中不存在音频流和视频流: " << file_path << ", file: " << __FILE__ << ", line: " << __LINE__;
        return -1;
    }

    this->file_path = input_file;
    return 0;
}

int file_convert::video_to_audio(std::string& output_audio_path) {

    LOG(INFO) << "开始转换视频文件: " << file_path << " 为音频文件: " << output_audio_path;

    return 0;
}

int file_convert::audio_convert(std::string& output_audio_path) {
    
    return 0;
}