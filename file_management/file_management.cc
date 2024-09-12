#include "file_management.h"

#include "glog/logging.h"

extern "C" {
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
    #include <libavutil/log.h>
    #include <libavutil/opt.h>
    #include <libswresample/swresample.h>
}

#include <glog/logging.h>
#include <filesystem>

file_management::file_management() {
}

file_management::~file_management(){
}

int file_management::file_type(const std::string& file_path){
     if (file_path.empty()) {
        LOG(ERROR) << "输入文件为空 : " << __FILE__ << ", line: " << __LINE__;
        return -1;
    }

    AVFormatContext* fmt_ctx = avformat_alloc_context();
    if (!fmt_ctx) {
        LOG(ERROR) << "输入的文件路径为空 : " << __FILE__ << ", line: " << __LINE__;
        return FILE_TYPE_UNKNOWN;
    }

    if (avformat_open_input(&fmt_ctx, file_path.c_str(), nullptr, nullptr) != 0) {
        LOG(ERROR) << "无法打开文件: " << file_path << ", 文件名：" << __FILE__ << ", 行号：" << __LINE__;
        avformat_free_context(fmt_ctx);
        return FILE_TYPE_UNKNOWN;
    }

    int type = FILE_TYPE_UNKNOWN;
    if (fmt_ctx->iformat->name) {
        std::string format_name(fmt_ctx->iformat->name);
        if (format_name.find("aac") != std::string::npos) {
            type = FILE_TYPE_AAC;
        } else if (format_name.find("flv") != std::string::npos) {
            type = FILE_TYPE_FLV;
        } else if (format_name.find("mp4") != std::string::npos) {
            type = FILE_TYPE_MP4;
        }
    }

    avformat_close_input(&fmt_ctx);
    avformat_free_context(fmt_ctx);
    return type;
}