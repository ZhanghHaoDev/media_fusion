#include "gtest/gtest.h"
#include "glog/logging.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    google::InitGoogleLogging(argv[0]);
    google::SetStderrLogging(google::INFO); // 确保 INFO 级别日志输出到标准错误
    google::InstallFailureSignalHandler();  // 捕捉 SIGSEGV 等信号

    int result = RUN_ALL_TESTS();

    google::ShutdownGoogleLogging();
    return result;
}