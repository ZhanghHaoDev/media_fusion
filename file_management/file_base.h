#ifndef FILE_BASE_H
#define FILE_BASE_H

#include <string>
#include <vector>

// 文件的基类
template <typename T>
class file_base{
public:
    enum file_type{
        UNKNOWN = 0,
        AAC = 1,
        MP3 = 2,
    };

    struct audio_file_info{
        std::string file_path; // 文件路径
        int file_size; // 文件大小, 单位：字节
        enum file_type type; // 文件类型
        int duration;   // 音频时长 单位：秒
        int sample_rate;   // 采样率
        int channels;      // 声道数
        int bit_rate;      // 比特率
        int bit_depth;  // 比特深度
        std::vector<T> sample_values; // 采样值
        int frame_length;   // 帧长度
    };

    struct video_file_info{
        std::string file_path; // 文件路径
        enum file_type type; // 文件类型
    };

public:
    /**
     * @brief 打开文件
     * 
     * @param file_name 文件名
     * @return int 错误码
     */
    virtual int file_open(const std::string &file_name) = 0;

    /**
     * @brief 获取文件类型
     * 
     * @return true 是对应的文件
     */
    virtual enum file_type get_file_type() = 0;

    /**
     * @brief 获取音频文件信息,提供默认实现
     * 
     * @return audio_file_info 音频文件信息 
     */
    virtual audio_file_info get_audio_file_info(){
        return audio_file_info();
    }

    /**
     * @brief 获取视频文件信息,提供默认实现
     * 
     * @return video_file_info 视频文件信息
     */
    virtual video_file_info get_video_file_info(){
        return video_file_info();
    }
};

#endif // FILE_BASE_H