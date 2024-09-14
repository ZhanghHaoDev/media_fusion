#ifndef AAC_FILE_H
#define AAC_FILE_H

#include <iostream>
#include <string>
#include <sys/_types/_int32_t.h>

#include "file_base.h"

class aac_file : public file_base<int32_t>{
public:
    aac_file();
    ~aac_file();

    /**
     * @brief 打开文件
     * 
     * @param file_name 文件名
     * @return int 错误码
     */
    int file_open(const std::string &file_name) override;

    /**
     * @brief 判断文件类型
     * 
     * @return file_type 文件类型
     */
    enum file_type get_file_type() override;

    /**
     * @brief 获取aac文件信息
     * 
     * @param file_path 文件路径，如果为空则使用构造函数传入的文件路径
     * @return aac_file_info aac文件信息
     */
    file_base<int32_t>::audio_file_info get_audio_file_info() override;

private:
    std::string file_path;  // 文件路径
    audio_file_info info;     // 文件信息
};

#endif // AAC_FILE_H