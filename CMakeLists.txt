cmake_minimum_required(VERSION 2.8)
cmake_policy(VERSION 2.8)

project(OpenWaterL)

list(APPEND CMAKE_CXX_FLAGS "-std=c++11")

# debug/release 环境设置
set(CMAKE_DEBUG_POSTFIX "_d" CACHE STRING "add a postfix, usually d on windows")  
set(CMAKE_RELEASE_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows")  

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/libs)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)  

link_directories(${LIBRARY_OUTPUT_PATH})

# first create relevant static libraries requried for other projects
# add glfw lib
set(GLFW_SOURCE "${CMAKE_SOURCE_DIR}/external/glfw/")
add_subdirectory(${GLFW_SOURCE} glfw3)
include_directories(${GLFW_SOURCE}/include)
# link different lib according to different build type
set(GLFW3 optimized glfw3 debug "glfw3${CMAKE_DEBUG_POSTFIX}")

# add glad lib
set(GLAD_SOURCE "${CMAKE_SOURCE_DIR}/external/glad/")
add_subdirectory(${GLAD_SOURCE} glad)
include_directories(${GLAD_SOURCE}/include)
set(GLAD optimized glad debug "glad${CMAKE_DEBUG_POSTFIX}")

set(LIBS ${GLFW3} ${GLAD} opengl32)
message(STATUS, ${LIBS}, ${GLAD_SOURCE})

file(GLOB_RECURSE CPPFILE
	"src/code/*.cpp"
)

file(GLOB_RECURSE HEADFILE
	"src/code/*.h"
)

# 接入 md 文件
file(GLOB_RECURSE MARKDOWNFILE
	"doc/*.md"
)

message(STATUS, ${CPPFILE}, " 所有 CPP 文件")
message(STATUS, ${HEADFILE}, " 所有 H 文件")
add_executable(OpenWaterL ${CPPFILE} ${HEADFILE} ${MARKDOWNFILE})

set(all_files ${HEADFILE} ${CPPFILE})

# 一个根据文件夹来进行分类的函数
macro(source_group_by_dir source_files)
    if(MSVC)
        set(sgbd_cur_dir ${CMAKE_CURRENT_SOURCE_DIR})
        foreach(sgbd_file ${${source_files}})
            string(REGEX REPLACE ${sgbd_cur_dir}/\(.*\) \\1 sgbd_fpath ${sgbd_file})
            string(REGEX REPLACE "\(.*\)/.*" \\1 sgbd_group_name ${sgbd_fpath})
            string(COMPARE EQUAL ${sgbd_fpath} ${sgbd_group_name} sgbd_nogroup)
            string(REPLACE "/" "\\" sgbd_group_name ${sgbd_group_name})
            if(sgbd_nogroup)
                set(sgbd_group_name "\\")
            endif(sgbd_nogroup)
            source_group(${sgbd_group_name} FILES ${sgbd_file})
        endforeach(sgbd_file)
    endif(MSVC)
endmacro(source_group_by_dir)
source_group_by_dir(all_files)

message(STATUS, ${MARKDOWNFILE}, " 所有 md 文件")
#设置 vs sln filter的接口
source_group("_markdownDoc" FILES ${MARKDOWNFILE})

target_link_libraries(OpenWaterL ${LIBS})
add_dependencies(OpenWaterL glfw)
# add_dependencies(OpenWaterL glad)
