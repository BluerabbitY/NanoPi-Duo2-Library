# 交叉编译环境配置  要在PROJECT指令前,否则无效
# 告知当前使用的是交叉编译方式
SET(CMAKE_SYSTEM_NAME Linux)

# 指定处理器类型
SET(CMAKE_SYSTEM_PROCESSOR arm)

# 设置交叉编译环境
SET(ENV{TOOLCHAIN} /opt/csky-gcc/bin/csky-abiv2-linux-)

# 指定C交叉编译工具
SET(CMAKE_C_COMPILER "/opt/csky-gcc/bin/csky-abiv2-linux-gcc")

# 指定C++交叉编译工具
SET(CMAKE_CXX_COMPILER "/opt/csky-gcc/bin/csky-abiv2-linux-g++")

#指定交叉编译环境安装目录...可以设置也可以不设置
#SET(CMAKE_FIND_ROOT_PATH "...")

# 设置查找工具程序路径
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# 设置只在指定目录下查找库文件
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)

# 设置只在指定目录下查找头文件
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# 输出详细编译命令
set(CMAKEV_VERBOSE_MAKE_FILE ON)
# 设置C++标准
set(CMAKE_CXX_STANDARD 14)