# media_fusion

自己写的一个播放器项目

## 1. 包含的功能

1. 分析音视频文件
2. 播放音视频文件，倍速播放
3. 音视频文件的转码，提取音频，提取视频
4. rtsp流的播放（未来）

## 2. 使用到的技术

1. Qt 版本6 用于界面的设计
2. FFmpeg 用于音视频文件的解析，转码，播放
3. SDL2 用于音视频文件的播放
4. glog 用于日志的输出
5. gflags 用于命令行参数的解析
6. gtest 用于单元测试

## 3. 测试

```shell
mkdir build
cd build
cmake ..
make
cd test
ctest
```