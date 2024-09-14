#include "aac_file.h"

#include <fstream>
#include <string>
#include <vector>

#include <sys/_types/_int16_t.h>

#include "glog/logging.h"
#include <SDL2/SDL.h>
extern "C" {
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
    #include <libavutil/log.h>
    #include <libavutil/opt.h>
    #include <libswresample/swresample.h>
}

aac_file::aac_file(){
    av_log_set_level(AV_LOG_QUIET);   // 禁用 FFmpeg 日志输出
}

aac_file::~aac_file() = default;

int aac_file::file_open(const std::string &file_name){
    if (file_name.empty()) {
        LOG(ERROR) << "file name is empty, file: " << __FILE__ << ", line: " << __LINE__;
        return -1;
    }
    std::ifstream file(file_name);
    if (!file.is_open()) {
        LOG(ERROR) << "无法打开文件: " << file_name << ", file: " << __FILE__ << ", line: " << __LINE__;
        return -1;
    }

    this->file_path = file_name;
    return 0;
}

file_base<int32_t>::file_type aac_file::get_file_type(){
    std::string path_to_use = this->file_path;

    AVFormatContext* format_context = nullptr;
    if (avformat_open_input(&format_context, path_to_use.c_str(), nullptr, nullptr) != 0) {
        LOG(ERROR) << "无法打开文件: " << path_to_use << ", file: " << __FILE__ << ", line: " << __LINE__;
        return file_base<int32_t>::UNKNOWN;
    }

    if (avformat_find_stream_info(format_context, nullptr) < 0) {
        LOG(ERROR) << "无法找到流信息: " << path_to_use << ", file: " << __FILE__ << ", line: " << __LINE__;
        avformat_close_input(&format_context);
        return file_base<int32_t>::UNKNOWN;
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
    return file_base<int32_t>::AAC;
}

// file_base<int32_t>::audio_file_info aac_file::get_audio_file_info(){
//     std::string path_to_use = this->file_path;

//     AVFormatContext* format_context = nullptr;
//     if (avformat_open_input(&format_context, path_to_use.c_str(), nullptr, nullptr) != 0) {
//         LOG(ERROR) << "无法打开文件: " << path_to_use << ", file: " << __FILE__ << ", line: " << __LINE__;
//         throw std::runtime_error("无法打开文件");
//     }

//     if (avformat_find_stream_info(format_context, nullptr) < 0) {
//         LOG(ERROR) << "无法找到流信息: " << path_to_use << ", file: " << __FILE__ << ", line: " << __LINE__;
//         avformat_close_input(&format_context);
//         throw std::runtime_error("无法找到流信息");
//     }

//     info.file_path = path_to_use;
//     info.type = file_base<int32_t>::AAC;

//     for (unsigned int i = 0; i < format_context->nb_streams; i++) {
//         AVStream* stream = format_context->streams[i];
//         if (stream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
//             info.duration = static_cast<double>(stream->duration) * av_q2d(stream->time_base);
//             info.file_size = avio_size(format_context->pb);
//             info.sample_rate = stream->codecpar->sample_rate;
//             info.channels = stream->codecpar->ch_layout.nb_channels;
//             info.bit_rate = stream->codecpar->bit_rate;
//             info.bit_depth = av_get_bytes_per_sample(static_cast<AVSampleFormat>(stream->codecpar->format)) * 8;
//             info.frame_length = stream->codecpar->frame_size;
//             break;
//         }
//     }

//     avformat_close_input(&format_context);
//     return info;
// }

file_base<int32_t>::audio_file_info aac_file::get_audio_file_info() {
    file_base<int32_t>::audio_file_info info;
    std::string path_to_use = this->file_path;

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
    info.type = file_base<int32_t>::AAC;

    AVCodecContext* codec_context = nullptr;
    const AVCodec* codec = nullptr;
    AVPacket* packet = av_packet_alloc();
    AVFrame* frame = av_frame_alloc();

    for (unsigned int i = 0; i < format_context->nb_streams; i++) {
        AVStream* stream = format_context->streams[i];
        if (stream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            codec = avcodec_find_decoder(stream->codecpar->codec_id);
            if (!codec) {
                LOG(ERROR) << "无法找到解码器: " << path_to_use << ", file: " << __FILE__ << ", line: " << __LINE__;
                avformat_close_input(&format_context);
                throw std::runtime_error("无法找到解码器");
            }

            codec_context = avcodec_alloc_context3(codec);
            if (!codec_context) {
                LOG(ERROR) << "无法分配解码器上下文: " << path_to_use << ", file: " << __FILE__ << ", line: " << __LINE__;
                avformat_close_input(&format_context);
                throw std::runtime_error("无法分配解码器上下文");
            }

            if (avcodec_parameters_to_context(codec_context, stream->codecpar) < 0) {
                LOG(ERROR) << "无法初始化解码器上下文: " << path_to_use << ", file: " << __FILE__ << ", line: " << __LINE__;
                avcodec_free_context(&codec_context);
                avformat_close_input(&format_context);
                throw std::runtime_error("无法初始化解码器上下文");
            }

            if (avcodec_open2(codec_context, codec, nullptr) < 0) {
                LOG(ERROR) << "无法打开解码器: " << path_to_use << ", file: " << __FILE__ << ", line: " << __LINE__;
                avcodec_free_context(&codec_context);
                avformat_close_input(&format_context);
                throw std::runtime_error("无法打开解码器");
            }

            info.duration = static_cast<double>(stream->duration) * av_q2d(stream->time_base);
            info.file_size = avio_size(format_context->pb);
            info.sample_rate = stream->codecpar->sample_rate;
            info.channels = stream->codecpar->ch_layout.nb_channels;
            info.bit_rate = stream->codecpar->bit_rate;
            info.bit_depth = av_get_bytes_per_sample(static_cast<AVSampleFormat>(stream->codecpar->format)) * 8;
            info.frame_length = stream->codecpar->frame_size;

            while (av_read_frame(format_context, packet) >= 0) {
                if (packet->stream_index == stream->index) {
                    if (avcodec_send_packet(codec_context, packet) == 0) {
                        while (avcodec_receive_frame(codec_context, frame) == 0) {
                            for (int i = 0; i < frame->nb_samples; i++) {
                                for (int ch = 0; ch < codec_context->ch_layout.nb_channels; ch++) {
                                    int32_t* sample = reinterpret_cast<int32_t*>(frame->data[ch] + i * av_get_bytes_per_sample(codec_context->sample_fmt));
                                    info.sample_values.push_back(*sample);
                                }
                            }
                        }
                    }
                }
                av_packet_unref(packet);
            }

            break;
        }
    }

    av_frame_free(&frame);
    av_packet_free(&packet);
    avcodec_free_context(&codec_context);
    avformat_close_input(&format_context);

    return info;
}
