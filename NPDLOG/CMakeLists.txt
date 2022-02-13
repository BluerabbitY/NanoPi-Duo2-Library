if (${EBUG})
    message("Debug mode")
    add_definitions(-DLDEBUG)
else()
    message("Release mode")
endif()

#把当前目录下所有源代码文件和头文件加入变量DIR_SRCS
AUX_SOURCE_DIRECTORY(. DIR_SRCS)

#将该目录中的源文件编译为共享库
ADD_LIBRARY (log STATIC ${DIR_SRCS})

#设置共享库输出路径
SET(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)