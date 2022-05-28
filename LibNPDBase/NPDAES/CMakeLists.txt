#cmake最低版本需求，不加入此行会受到警告信息
CMAKE_MINIMUM_REQUIRED(VERSION 3.13.0)

#项目名称
PROJECT(aescode)

################################################################################
# 程序版本
# V01 主版本	功能模块变动较大时修改,例如模块功能增加,修改架构等
# R01 子版本	某个模块的功能发生较大变化时修改
# C01 内部修订版本	可作为修订版本, 修复一个严重BUG可以进行修改
# B00 阶段版本	如下：
# 00: Alpha版, 此阶段的软件主要以实现功能为主,BUG较多,需进一步修复
# 01: Beta版, 相对Alpha有改进,消除一些严重BUG, 仍存在缺陷,需要进一步测试
# 02: RC版, 基本成熟, 基本不存在导致错误的BUG;
# 03: Release, 正式发布版

set(MAIN_VERSION  1)
set(SUB_VERSION   1)
set(INNER_VERSION 1)
set(STAGE_VERSION 0)

# 添加version.h
configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/version.h.in"
  "${CMAKE_CURRENT_SOURCE_DIR}/AES/version.h"
)

# 这样可以找到verdion.h头文件
#INCLUDE_DIRECTORIES("${CMAKE_CURRENT_BINARY_DIR}/AES")
################################################################################

#包含一个子目录AES
ADD_SUBDIRECTORY(AES)

#添加头文件路径
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/LibNPDErrorDeal)

#把当前目录下所有源代码文件和头文件加入变量DIR_SRCS
AUX_SOURCE_DIRECTORY(. DIR_SRCS)

#生成应用程序 AESCode (在windows下会自动生成AESCode)
#ADD_EXECUTABLE(AESCode ${DIR_SRCS})
ADD_LIBRARY(aescode STATIC ${DIR_SRCS})

#指明可执行文件AESCode需要连接一个名为AES的链接库
TARGET_LINK_LIBRARIES(aescode aes base)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/output/lib/AES)