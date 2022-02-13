#cmake最低版本需求，不加入此行会受到警告信息
CMAKE_MINIMUM_REQUIRED(VERSION 3.13.0)

#把当前目录下所有源代码文件和头文件加入变量DIR_SRCS
AUX_SOURCE_DIRECTORY(. DIR_SRCS)

#将该目录中的源文件编译为共享库
ADD_LIBRARY(database STATIC ${DIR_SRCS})

#设置共享库输出路径
SET(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)

#添加编译选项
ADD_COMPILE_OPTIONS(-pthread -ldl)