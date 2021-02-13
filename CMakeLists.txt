cmake_minimum_required(VERSION 2.8)
cmake_policy(VERSION 2.8)

project(OpenWaterL)

# Build Debug Work Or Release Work
# TODO: Use Python To Change This Param
set(_OPENWATERL_PARAM_DEBUG 1)
# set(_OPENWATERL_PARAM_DEBUG 0)

if (${_OPENWATERL_PARAM_DEBUG})
	message("#### _OPENWATERL_PARAM_DEBUG    $$$ General Debug Work")
else (${_OPENWATERL_PARAM_DEBUG})
	message("#### _OPENWATERL_PARAM_RELEASE  $$$ General Release Work")
endif (${_OPENWATERL_PARAM_DEBUG})

message("#### CMake Source Dir")
message("${CMAKE_SOURCE_DIR}")
# use 64 or 32
if (${CMAKE_CL_64})
	message("#### Use CL 64 bit")
else (${CMAKE_CL_64})
	message("#### Use CL 32 bit")
endif (${CMAKE_CL_64})

# for warning D9002
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

# debug/release 环境设置
set(CMAKE_DEBUG_POSTFIX "_d" CACHE STRING "add a postfix, usually d on windows")  
set(CMAKE_RELEASE_POSTFIX "" CACHE STRING "add a postfix, usually empty on windows")  

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/libs)

# set biuld path with debug or release
if (${_OPENWATERL_PARAM_DEBUG})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin)
else (${_OPENWATERL_PARAM_DEBUG})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin)
endif (${_OPENWATERL_PARAM_DEBUG})

link_directories(${LIBRARY_OUTPUT_PATH})
message("#### OpenWaterL Build Libs path:")
message("${LIBRARY_OUTPUT_PATH}")

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

set(OPENGL_LIBS ${GLFW3} ${GLAD} opengl32)

message("#### All OpenGL Libs:")
message("${OPENGL_LIBS}")
message("#### glad source:")
message("${GLAD_SOURCE}")

# add spdlog
set(SPDLOG_SOURCE "${CMAKE_SOURCE_DIR}/external/spdlog/")
include_directories(${SPDLOG_SOURCE})

# add stb
set(STB_SOURCE "${CMAKE_SOURCE_DIR}/external/stb/")
include_directories(${STB_SOURCE})

# add glm
set(STB_SOURCE "${CMAKE_SOURCE_DIR}/external/glm-0.9.9.7/glm/")
include_directories(${STB_SOURCE})

# add fbxsdk
set(FBXSDK_SOURCE "${CMAKE_SOURCE_DIR}/external/fbx/include")
include_directories(${FBXSDK_SOURCE})
# fbx libs path and dll
# copy fbx dll to ${PROJECT_SOURCE_DIR}/bin
# use file(COPY) and not use configur_file
# configure_file(<input> <output> COPYONLY)  .vs. file(COPY <input> DESTINATION <output>)
# If the input file is modified the build system will re-run CMake to re-configure the file and generate the build system again.
# but libfbxsdk.dll doesn't change
if (${CMAKE_CL_64}) # if -A x64
	if (${_OPENWATERL_PARAM_DEBUG})
		set(FBX_LIBS_PATH "${CMAKE_SOURCE_DIR}/external/fbx/lib/x64/debug")
		file(COPY ${CMAKE_SOURCE_DIR}/external/fbx/lib/x64/debug/libfbxsdk.dll DESTINATION ${PROJECT_SOURCE_DIR}/bin/)
	else (${_OPENWATERL_PARAM_DEBUG})
		set(FBX_LIBS_PATH "${CMAKE_SOURCE_DIR}/external/fbx/lib/x64/release")
		file(COPY ${CMAKE_SOURCE_DIR}/external/fbx/lib/x64/release/libfbxsdk.dll DESTINATION ${PROJECT_SOURCE_DIR}/bin/)
	endif (${_OPENWATERL_PARAM_DEBUG})
else (${CMAKE_CL_64}) # if -A x86
	if (${_OPENWATERL_PARAM_DEBUG})
		set(FBX_LIBS_PATH "${CMAKE_SOURCE_DIR}/external/fbx/lib/x86/debug")
		file(COPY ${CMAKE_SOURCE_DIR}/external/fbx/lib/x86/debug/libfbxsdk.dll DESTINATION ${PROJECT_SOURCE_DIR}/bin/)
	else (${_OPENWATERL_PARAM_DEBUG})
		set(FBX_LIBS_PATH "${CMAKE_SOURCE_DIR}/external/fbx/lib/x86/release")
		file(COPY ${CMAKE_SOURCE_DIR}/external/fbx/lib/x86/release/libfbxsdk.dll DESTINATION ${PROJECT_SOURCE_DIR}/bin/)
	endif (${_OPENWATERL_PARAM_DEBUG})
endif (${CMAKE_CL_64})
message("#### fbx libs path:")
message("${FBX_LIBS_PATH}")
link_directories(${FBX_LIBS_PATH})
# fbx libs
# No Use libfbxsdk-md and libfbxsdk-mt if need find in  fbx sdk vs2015 version
# set(FBX_LIBS "libfbxsdk" "libfbxsdk-md" "libfbxsdk-mt")
set(FBX_LIBS "libfbxsdk")
message("#### fbx libs:")
message("${FBX_LIBS}")

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

# 接入 md 文件
file(GLOB_RECURSE NOTE_MARKDOWNFILE
	"note/*.md"
)

# 接入 shader 文件
# vs 顶点着色器文件
# fs 片段着色器文件
# gs 几何着色器文件
file(GLOB_RECURSE SHADER_FILE
	"workspace/shader_code/*.[vfg]s"
)

message("#### all CPP files:")
foreach(F ${CPPFILE})
     message(${F})
endforeach(F)
message("#### all H files:")
foreach(F ${HEADFILE})
     message(${F})
endforeach(F)
add_executable(OpenWaterL ${CPPFILE} ${HEADFILE} ${MARKDOWNFILE} ${NOTE_MARKDOWNFILE} ${SHADER_FILE})

# Make WorkDirectory and Set it
set_target_properties(OpenWaterL PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/workspace")

set(all_files ${HEADFILE} ${CPPFILE})

# 添加 定义
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# 一个根据文件夹来进行分类的函数, 在windows vs 下有用
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

message("#### all md files:")
foreach(F ${MARKDOWNFILE})
     message(${F})
endforeach(F)
#设置 vs sln filter的接口
source_group("_markdownDoc" FILES ${MARKDOWNFILE})

message("#### all note - md files:")
foreach(F ${NOTE_MARKDOWNFILE})
     message(${F})
endforeach(F)
source_group("_markdownNote" FILES ${NOTE_MARKDOWNFILE})

source_group("_shaders" FILES ${SHADER_FILE})

# A "debug", "optimized", or "general" keyword indicates that the library immediately following 
# it is to be used only for the corresponding build configuration.
# Libraries specified as "debug" are appended to the the LINK_INTERFACE_LIBRARIES_DEBUG property 
# Libraries specified as "optimized" are appended to the the LINK_INTERFACE_LIBRARIES property
# Libraries specified as "general" (or without any keyword) are treated as if specified for both "debug" and "optimized"
target_link_libraries(OpenWaterL ${OPENGL_LIBS})
target_link_libraries(OpenWaterL ${FBX_LIBS})

add_dependencies(OpenWaterL glfw)
# add_dependencies(OpenWaterL glad) # 依赖默认已有
