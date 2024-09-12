#include "gtest/gtest.h"
#include "aac_file.h"

// 测试 aac_file 类的 is_aac_file 方法
TEST(AacFileTest, IsAacFile) {
    aac_file file("/Users/zhanghao/code/cpp/media_fusion/tests_files/test.aac");
    EXPECT_TRUE(file.is_aac_file());
}

// 测试 aac_file 类的 get_aac_file_info 方法
TEST(AacFileTest, GetAacFileInfo) {
    aac_file file("/Users/zhanghao/code/cpp/media_fusion/tests_files/test.aac");
    aac_file::aac_file_info info = file.get_aac_file_info();
    EXPECT_EQ(info.file_path, "/Users/zhanghao/code/cpp/media_fusion/tests_files/test.aac");
    EXPECT_TRUE(info.is_aac);
    EXPECT_GT(info.sample_rate, 0);
    EXPECT_GT(info.channels, 0);
    EXPECT_GT(info.bit_rate, 0);
    EXPECT_GE(info.format, 0);
    EXPECT_GT(info.frame_length, 0);
    EXPECT_GT(info.duration, 0);
}

// 测试 aac_file 类的 extract_audio 方法
TEST(AacFileTest, ExtractAudio) {
    aac_file file("/Users/zhanghao/code/cpp/media_fusion/tests_files/test.aac");
    EXPECT_NO_THROW(file.extract_audio("/Users/zhanghao/code/cpp/media_fusion/tests_files/believe.flv", "out.aac"));
}

// 测试 aac_file 类的 play_aac_file 方法
TEST(AacFileTest, PlayAacFile) {
    aac_file file("/Users/zhanghao/code/cpp/media_fusion/tests_files/test.aac");
    EXPECT_NO_THROW(file.play_aac_file());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}