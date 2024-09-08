#include "file_management.h"

#include <spdlog/spdlog.h>

extern "C" {
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
    #include <libavutil/log.h>
    #include <libavutil/opt.h>
    #include <libswresample/swresample.h>
}

file_management::file_management(){

}
file_management::~file_management(){

}

int file_management::file_type(const std::string& file_path){
     if (file_path.empty()) {
        spdlog::error("输入的文件路径为空，文件名：{}，行号：{}", __FILE__, __LINE__);
        return -1;
    }

    AVFormatContext* fmt_ctx = avformat_alloc_context();
    if (!fmt_ctx) {
        spdlog::error("输入的文件路径为空，文件名：{}，行号：{}", __FILE__, __LINE__);
        return FILE_TYPE_UNKNOWN;
    }

    if (avformat_open_input(&fmt_ctx, file_path.c_str(), nullptr, nullptr) != 0) {
        spdlog::error("Could not open file: {}，文件名称：{}，行号：{}", file_path, __FILE__, __LINE__);
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