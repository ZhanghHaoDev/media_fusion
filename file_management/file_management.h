#ifndef FILE_MANAGEMENT_H
#define FILE_MANAGEMENT_H

#include <iostream>
#include <string>

#define FILE_TYPE_UNKNOWN 0
#define FILE_TYPE_AAC 1
#define FILE_TYPE_FLV 2
#define FILE_TYPE_MP4 3

class file_management{
public:
    file_management();
    ~file_management();

    /**
     * @brief 判断文件类型
     * 
     * @param file_path 文件路径
     * @return int 小于0表示错误，0表示未知文件类型，大于0表示文件类型
     */
    int file_type(const std::string& file_path);

private:


};

#endif // FILE_MANAGEMENT_H