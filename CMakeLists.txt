cmake_minimum_required(VERSION 3.20)
project(gpx_see_cmake)
# 指定c++标准的版本
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-g")
# 设置Qt5的cmake模块所在目录，如果不设置将使用系统提供的版本
# QT_DIR和QT_VERSION是指定了qt安装目录和版本的环境变量
# set(CMAKE_PREFIX_PATH $ENV{QT_DIR}/$ENV{QT_VERSION}/mingw53_32/lib/cmake)
# set(CMAKE_PREFIX_PATH "D:\\msys64\\mingw64\\lib\\cmake")

#设置工程包含当前目录，非必须
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#打开全局moc,设置自动生成moc文件，一定要设置
set(CMAKE_AUTOMOC ON)
#打开全局uic，非必须
set(CMAKE_AUTOUIC ON)
#打开全局rcc，非必须，如需打开，注意修改33行的qrc文件名
set(CMAKE_AUTORCC ON)

add_definitions(-DAPP_VERSION="13.19")

# Add compiler flags for building executables (-fPIE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

#设置运行时输出可执行文件目录(CMAKE源目录CMAKE_CURRENT_SOURCE_DIR，执行目录CMAKE_CURRENT_BINARY_DIR)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
#设置运行时输出共享库文件目录
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)

#查找需要的Qt库文件，最好每一个库都要写，Qt也会根据依赖关系自动添加
set(Projects_QT5_COMPONENTS
        Core
        Gui
        Widgets
        Network
        Sql
        Concurrent
        PrintSupport
        Positioning
        Svg
        SerialPort
        )

set(Projects_QT5_Includes
        ${Qt5Core_INCLUDE}
        ${Qt5Gui_INCLUDE}
        ${Qt5Widgets_INCLUDE}
        ${Qt5Network_INCLUDE}
        ${Qt5Sql_INCLUDE}
        ${Qt5Concurrent_INCLUDE}
        ${Qt5PrintSupport_INCLUDE}
        ${Qt5Positioning_INCLUDE}
        ${Qt5Svg_INCLUDE}
        ${Qt5SerialPort_INCLUDE}
        )

set(Projects_QT5_Libraries
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Network
        Qt5::Sql
        Qt5::Concurrent
        Qt5::PrintSupport
        Qt5::Positioning
        Qt5::Svg
        Qt5::SerialPort
        )

find_package(Qt5  COMPONENTS ${Projects_QT5_COMPONENTS} REQUIRED )
find_package (Qt5LinguistTools)

INCLUDE_DIRECTORIES(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${Projects_QT5_Includes}
)#设置项目包含目录

include_directories(
        src/
        src/common/
        src/GUI/
        src/data/
        src/map/
        src/map/ENC/
        src/map/IMG/
        src/map/proj/
)

file(GLOB_RECURSE header_SRCS
        src/common/*.h
        src/GUI/*.h
        src/data/*.h
        src/map/*.h
        src/map/ENC/*.h
        src/map/IMG/*.h
        src/map/proj/*.h
        )


#查找当前文件夹中的所有源代码文件，也可以通过Set命令将所有文件设置为一个变量
file(GLOB_RECURSE SOURCES
        src/*.cpp
        )
#查找设置当前文件夹中所有的头文件
FILE(GLOB HEAD_FILES "./*.h")
#查找设置当前文件夹中所有的ui文件
FILE(GLOB UI_FILES "./*.ui")

#复制data文件夹到build目录
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data/)


#通过Ui文件生成对应的头文件，一定要添加
# qt5_wrap_ui(WRAP_FILES ${UI_FILES})

#添加资源文件，非必须，一旦采用，注意修改相应的qrc文件名
set(QRC_FILES
        gpxsee.qrc
        theme-color.qrc
)

set(TS_FILES
        lang/gpxsee_en.ts
        lang/gpxsee_zh_CN.ts
        )

set_source_files_properties(${TS_FILES} PROPERTIES OUTPUT_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/translations)

get_source_file_property(output_location ${CMAKE_CURRENT_BINARY_DIR}/translations OUTPUT_LOCATION)
if(output_location)
    file(MAKE_DIRECTORY "${output_location}")
    set(qm "${output_location}/${qm}.qm")
else()
    set(qm "${CMAKE_CURRENT_BINARY_DIR}/${qm}.qm")
endif()

qt_add_translation(qmFiles ${TS_FILES})
#将ui文件和生成文件整理在一个文件夹中，非必须
# source_group("Ui" FILES ${UI_FILES} ${WRAP_FILES} )

add_library(Solution SHARED
        ${QRC_FILES}
)

#创建工程文件
# add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEAD_FILES} ${QRC_FILES} ${WRAP_FILES})
add_executable(${PROJECT_NAME} WIN32
        ${SOURCES}
        ${header_SRCS}
        ${QRC_FILES}
        ${qmFiles}
        )

#添加Qt5依赖项
target_link_libraries(${PROJECT_NAME}
        ${Projects_QT5_Libraries}
        Qt${QT_VERSION_MAJOR}::GuiPrivate
        )