#ifndef AAC_FILE_H
#define AAC_FILE_H

#include <iostream>
#include <string>

class aac_file{
public:
struct aac_file_info{
    std::string file_path; // 文件路径
    bool is_aac;       // 是否是aac文件
    int duration;   // 音频时长 单位：秒
    int sample_rate;   // 采样率
    int channels;      // 声道数
    int bit_rate;      // 比特率
    int format; // 封装格式 0: ADTS 1: ADIF
    int profile;    // 编码配置 0: AAC LC 1: HE-AAC (v1) 2: HE-AAC (v2) 3: AAC LD 4: AAC ELD
    int frame_length;   // 帧长度
};

public:

    /**
     * @brief 构造函数，传入文件路径
     * 
     * @param file_path 文件路径
     */
    aac_file(const std::string& file_path);
    aac_file();
    ~aac_file();

    /**
     * @brief 判断是否是aac文件
     * 
     * @param file_path 文件路径，如果为空则使用构造函数传入的文件路径
     * @return true 是aac文件
     * @return false 不是aac文件
     */
    bool is_aac_file(const std::string& file_path = "");

    /**
     * @brief 获取aac文件信息
     * 
     * @param file_path 文件路径，如果为空则使用构造函数传入的文件路径
     * @return aac_file_info aac文件信息
     */
    aac_file_info get_aac_file_info(const std::string& file_path = "");

    /**
     * @brief 提取音频
     * 
     * @param output_file_path 输出文件路径
     */
    void extract_audio(const std::string& input_video, const std::string& output_aac);

    /**
     * @brief 播放aac文件
     * 
     * @param file_path 文件路径，如果为空则使用构造函数传入的文件路径
     */
    void play_aac_file(const std::string& file_path = "");

    /**
     * @brief 输入aac文件
     * 
     * @param file_path 文件路径
     */
    void input_aac_file(const std::string& file_path);

private:
    std::string file_path;  // 文件路径
};

#endif // AAC_FILE_H