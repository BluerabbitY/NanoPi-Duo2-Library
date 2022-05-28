# CMAKE最低版本需求
CMAKE_MINIMUM_REQUIRED(VERSION 3.13.0)

# 设置交叉编译选项
# INCLUDE(${CMAKE_SOURCE_DIR}/NanoPiDuo.cmake)


# 设置指定的C++编译器版本是必须的,如果不设置或者为OFF,则指定版本不可用时,会使用上一版本。
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

# 指定为C++17版本
SET(CMAKE_CXX_STANDARD 17)

################################################################################
# 程序版本
# V01 主版本	     功能模块变动较大时修改,例如模块功能增加,修改架构等
# R01 子版本	     某个模块的功能发生较大变化时修改
# C01 内部修订版本	可作为修订版本, 修复一个严重BUG可以进行修改
# B00 阶段版本
# 如下：
# 00: Alpha版       此阶段的软件主要以实现功能为主,BUG较多,需进一步修复
# 01: Beta版        相对Alpha有改进,消除一些严重BUG, 仍存在缺陷,需要进一步测试
# 02: RC版          基本成熟, 基本不存在导致错误的BUG
# 03: Release       正式发布版

SET(MAJOR_VERSION 1)
SET(SPAWN_VERSION 1)
SET(INNER_VERSION 1)
SET(STAGE_VERSION 1)

# 设置编译时间
STRING(TIMESTAMP COMPILE_TIME "%Y/%m/%d %H:%M:%S")

SET(BUILD_TIME ${COMPILE_TIME})

PROJECT(nanopiduo)

# config.h头文件位置
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_BINARY_DIR}/NPDinclude")

# 添加config.h
CONFIGURE_FILE(
  "${CMAKE_SOURCE_DIR}/NPDinclude/NPDconfig.h.in"
  "${CMAKE_SOURCE_DIR}/NPDinclude/NPDconfig.h"
)
################################################################################
add_definitions(-DPUGIXML_HEADER_ONLY)
add_definitions(-DASIO_STANDALONE)

#OPTION (DEBUG_LOG "print debug log information" ON)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/NPDThirdparty)

# INCLUDE_DIRECTORIES(/usr/local/${PLATFORM}/include)
# include_directories(${CMAKE_SOURCE_DIR}/NPDThirdparty/include/rest_rpc)

# RPC库头文件路径
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/NPDThirdparty/msgpack)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/NPDThirdparty/asio)

# 第三方库头文件
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/NPDThirdparty/CSV)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/NPDThirdparty/JSON)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/NPDThirdparty/XML)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/NPDThirdparty/SQLite3)

# 包含的文件夹
ADD_SUBDIRECTORY(LibNPDBase)
ADD_SUBDIRECTORY(LibNPDDataBase)
ADD_SUBDIRECTORY(LibNPDDevDriver)
ADD_SUBDIRECTORY(LibNPDErrorDeal)
ADD_SUBDIRECTORY(LibNPDFileDeal)
ADD_SUBDIRECTORY(LibNPDNet)
ADD_SUBDIRECTORY(LibNPDOLED)
ADD_SUBDIRECTORY(LibNPDProtocol)
ADD_SUBDIRECTORY(NanoPi)
################################################################################