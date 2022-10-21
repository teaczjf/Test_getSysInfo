cmake_minimum_required(VERSION 3.14)

project(Test_getSysInfo LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)



#for easgylloging 全局宏定义
add_definitions (
                -DSQLITE_ENABLE_RTREE
                -DSQLITE_ENABLE_COLUMN_METADATA
                -DELPP_FEATURE_CRASH_LOG
                # easylog thread safe, it's very important!!!!!!!!
                -DELPP_THREAD_SAFE
                -DELPP_NO_DEFAULT_LOG_FILE
                -DELPP_FEATURE_ALL
#                -DELPP_OS_UNIX
                # -DSENDCMD_BY_MANNUE #开启后底层协议可以手动输入指令，新的通过数据库方式启动底层模块后，这方法就用不了了。
)


file(GLOB_RECURSE CPP_PATHS  *.cpp *.h *.cc *.hpp )
foreach(filepath ${CPP_PATHS})
    message("add cpp file:" ${filepath})
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" filename ${filepath}) #.+/(.+)\\\\..*" "\\\\1
    list(APPEND CPP_FILES ${filename})
endforeach(filepath)
list(REMOVE_DUPLICATES CPP_FILES)


qt_add_executable(Test_getSysInfo
    ${CPP_FILES}
)
#qt_add_executable(app-${CMAKE_PROJECT_NAME}
#    ${CPP_FILES}
#)


target_link_libraries(Test_getSysInfo PRIVATE ntdll)
