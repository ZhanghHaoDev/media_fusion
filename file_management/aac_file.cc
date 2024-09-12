#include "aac_file.h"

#include "glog/logging.h"

#include <SDL2/SDL.h>

extern "C" {
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
    #include <libavutil/log.h>
    #include <libavutil/opt.h>
    #include <libswresample/swresample.h>
}

aac_file::aac_file(const std::string& file_path){
    if (file_path.empty()){
        LOG(ERROR) << "无法打开文件，文件为空: " << __FILE__ << ", line: " << __LINE__;
        return;
    }
    
    this->file_path = file_path;

    av_log_set_level(AV_LOG_QUIET);   // 禁用 FFmpeg 日志输出
}

aac_file::aac_file(){
    av_log_set_level(AV_LOG_QUIET);   // 禁用 FFmpeg 日志输出
}

aac_file::~aac_file(){}

bool aac_file::is_aac_file(const std::string& file_path){
    std::string path_to_use = file_path.empty() ? this->file_path : file_path;

    AVFormatContext* format_context = nullptr;
    if (avformat_open_input(&format_context, path_to_use.c_str(), nullptr, nullptr) != 0) {
        LOG(ERROR) << "无法打开文件: " << path_to_use << ", file: " << __FILE__ << ", line: " << __LINE__;
        return false;
    }

    if (avformat_find_stream_info(format_context, nullptr) < 0) {
        LOG(ERROR) << "无法找到流信息: " << path_to_use << ", file: " << __FILE__ << ", line: " << __LINE__;
        avformat_close_input(&format_context);
        return false;
    }

    bool is_aac = false;
    for (unsigned int i = 0; i < format_context->nb_streams; i++) {
        AVStream* stream = format_context->streams[i];
        if (stream->codecpar->codec_id == AV_CODEC_ID_AAC) {
            is_aac = true;
            break;
        }
    }

    avformat_close_input(&format_context);
    return is_aac;
}

aac_file::aac_file_info aac_file::get_aac_file_info(const std::string& file_path){
    aac_file_info info;
    std::string path_to_use = file_path.empty() ? this->file_path : file_path;

    AVFormatContext* format_context = nullptr;
    if (avformat_open_input(&format_context, path_to_use.c_str(), nullptr, nullptr) != 0) {
        LOG(ERROR) << "无法打开文件: " << path_to_use << ", file: " << __FILE__ << ", line: " << __LINE__;
        throw std::runtime_error("无法打开文件");
    }

    if (avformat_find_stream_info(format_context, nullptr) < 0) {
        LOG(ERROR) << "无法找到流信息: " << path_to_use << ", file: " << __FILE__ << ", line: " << __LINE__;
        avformat_close_input(&format_context);
        throw std::runtime_error("无法找到流信息");
    }

    info.file_path = path_to_use;
    info.is_aac = aac_file::is_aac_file();

    for (unsigned int i = 0; i < format_context->nb_streams; i++) {
        AVStream* stream = format_context->streams[i];
        if (stream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            info.sample_rate = stream->codecpar->sample_rate;
            info.bit_rate = stream->codecpar->bit_rate;
            info.channels = stream->codecpar->ch_layout.nb_channels;
            info.format = stream->codecpar->format;
            info.profile = stream->codecpar->profile;
            info.frame_length = stream->codecpar->frame_size;
            info.duration = static_cast<double>(stream->duration) * av_q2d(stream->time_base);
            
            break;
        }
    }

    avformat_close_input(&format_context);
    return info;
}

void aac_file::extract_audio(const std::string& input_video, const std::string& output_aac) {
    if (input_video.empty() || output_aac.empty()) {
        LOG(ERROR) << "输入文件路径或输出文件路径为空,文件名：" << __FILE__ << ", 行号：" << __LINE__;
        return;
    }

    AVFormatContext* format_context = nullptr;
    if (avformat_open_input(&format_context, input_video.c_str(), nullptr, nullptr) != 0) {
        LOG(ERROR) << "无法打开文件: " << input_video << ", 文件名：" << __FILE__ << ", 行号：" << __LINE__;
        return;
    }

    if (avformat_find_stream_info(format_context, nullptr) < 0) {
        LOG(ERROR) << "无法找到流信息: " << input_video << ", 文件名：" << __FILE__ << ", 行号：" << __LINE__;
        avformat_close_input(&format_context);
        return;
    }

    AVStream* audio_stream = nullptr;
    for (unsigned int i = 0; i < format_context->nb_streams; i++) {
        if (format_context->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audio_stream = format_context->streams[i];
            break;
        }
    }

    if (!audio_stream) {
        LOG(ERROR) << "没有找到音频流: " << input_video << ", 文件名：" << __FILE__ << ", 行号：" << __LINE__;
        avformat_close_input(&format_context);
        return;
    }

    AVFormatContext* output_format_context = nullptr;
    avformat_alloc_output_context2(&output_format_context, nullptr, nullptr, output_aac.c_str());
    if (!output_format_context) {
        LOG(ERROR) << "无法创建输出上下文: " << output_aac << ", 文件名：" << __FILE__ << ", 行号：" << __LINE__;
        avformat_close_input(&format_context);
        return;
    }

    AVStream* out_stream = avformat_new_stream(output_format_context, nullptr);
    if (!out_stream) {
        LOG(ERROR) << "无法创建输出流: " << output_aac << ", 文件名：" << __FILE__ << ", 行号：" << __LINE__;
        avformat_close_input(&format_context);
        avformat_free_context(output_format_context);
        return;
    }

    if (avcodec_parameters_copy(out_stream->codecpar, audio_stream->codecpar) < 0) {
        LOG(ERROR) << "无法复制编码参数: " << output_aac << ", 文件名：" << __FILE__ << ", 行号：" << __LINE__;
        avformat_close_input(&format_context);
        avformat_free_context(output_format_context);
        return;
    }

    out_stream->codecpar->codec_tag = 0;

    if (!(output_format_context->oformat->flags & AVFMT_NOFILE)) {
        if (avio_open(&output_format_context->pb, output_aac.c_str(), AVIO_FLAG_WRITE) < 0) {
            LOG(ERROR) << "无法打开输出文件: " << output_aac << ", 文件名：" << __FILE__ << ", 行号：" << __LINE__;
            avformat_close_input(&format_context);
            avformat_free_context(output_format_context);
            return;
        }
    }

    if (avformat_write_header(output_format_context, nullptr) < 0) {
        LOG(ERROR) << "无法写入文件头: " << output_aac << ", 文件名：" << __FILE__ << ", 行号：" << __LINE__;
        avformat_close_input(&format_context);
        avformat_free_context(output_format_context);
        return;
    }

    AVPacket packet;
    while (av_read_frame(format_context, &packet) >= 0) {
        if (packet.stream_index == audio_stream->index) {
            packet.stream_index = out_stream->index;
            av_interleaved_write_frame(output_format_context, &packet);
        }
        av_packet_unref(&packet);
    }

    av_write_trailer(output_format_context);

    avformat_close_input(&format_context);
    if (!(output_format_context->oformat->flags & AVFMT_NOFILE)) {
        avio_closep(&output_format_context->pb);
    }
    avformat_free_context(output_format_context);

    LOG(INFO) << "音频提取完成: " << output_aac;
}

void aac_file::play_aac_file(const std::string& file_path){
    std::string path_to_use = file_path.empty() ? this->file_path : file_path;

    AVFormatContext* format_context = nullptr;
    if (avformat_open_input(&format_context, path_to_use.c_str(), nullptr, nullptr) != 0) {
        LOG(ERROR) << "无法打开文件: " << path_to_use << ", 文件名：" << __FILE__ << ", 行号：" << __LINE__;
        return;
    }

    if (avformat_find_stream_info(format_context, nullptr) < 0) {
        LOG(ERROR) << "无法找到流信息: " << path_to_use << ", 文件名：" << __FILE__ << ", 行号：" << __LINE__;
        avformat_close_input(&format_context);
        return;
    }
}


void aac_file::input_aac_file(const std::string& file_path){
    if (file_path.empty()) {
        LOG(ERROR) << "file path is empty, file: " << __FILE__ << ", line: " << __LINE__;
        return;
    }

    this->file_path = file_path;
}