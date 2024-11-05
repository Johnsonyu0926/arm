# 删除旧的构建目录
rm -rf build

# 创建新的构建目录
mkdir build

# 进入构建目录
cd build

# 运行CMake，设置构建类型为Release
cmake -DCMAKE_BUILD_TYPE=Release ..

# 编译项目
make

# 如果目标目录不存在，则创建它
if [ ! -d "../../bin/lib_debug" ] ; then
    mkdir -p ../../bin/lib_debug
fi

# 复制生成的库文件到目标目录
cp ./libmetartccore7.a ../../bin/lib_debug/