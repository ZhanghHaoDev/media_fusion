#include "aac_file.h"

#include <iostream>
#include <vector>
#include <string>
#include <sys/_types/_int32_t.h>

#include "gtest/gtest.h"

/*
* 测试文件路径 /Users/zhanghao/code/cpp/media_fusion/test/test_file/test.aac
* 文件大小：3633465字节
* 文件类型：AAC
* 音频时长：222.528s
* 采样率：48000
* 声道数：2
* 比特率：130625
* 比特深度：32
* 采样值 size : 21362688
* 帧长度：1024
*/
std::string file_path = "/Users/zhanghao/code/cpp/media_fusion/test/test_file/test.aac";

TEST(aac_file, file_open_test) {
    aac_file aac;
    std::string file_name = file_path;
    EXPECT_EQ(0, aac.file_open(file_name));
}

TEST(aac_file, get_file_type_test) {
    aac_file aac;
    std::string file_name = file_path;
    EXPECT_EQ(0, aac.file_open(file_name));
    EXPECT_EQ(file_base<int32_t>::file_type::AAC, aac.get_file_type());
}

TEST(aac_file, get_audio_file_info_test) {
    aac_file aac;
    std::string file_name = file_path;
    EXPECT_EQ(0, aac.file_open(file_name));
    auto info = aac.get_audio_file_info();
    EXPECT_EQ(file_path, info.file_path);
    EXPECT_EQ(file_base<int32_t>::file_type::AAC, info.type);
    EXPECT_EQ(3633465, info.file_size);
    EXPECT_EQ(222, info.duration);
    EXPECT_EQ(48000, info.sample_rate);
    EXPECT_EQ(2, info.channels);
    EXPECT_EQ(130625, info.bit_rate);
    EXPECT_EQ(32, info.bit_depth);
    EXPECT_EQ(21362688, info.sample_values.size());
    EXPECT_EQ(1024, info.frame_length);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}