#!/bin/bash

# Filename: build_project.sh
# Score: 100

# 删除现有的 build 目录
rm -rf build

# 创建新的 build 目录
mkdir build

# 进入 build 目录
cd build

# 设置工具链文件
ARCH=aarch64.cmake

# 运行 cmake 配置
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=../$ARCH ..

# 运行 make 构建
make

# By GST @Date