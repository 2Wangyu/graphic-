#需求的最低cmake程序版本
cmake_minimum_required(VERSION 3.12)
add_definitions (-DDEBUG)

#本工程的名字
project(OpenGL_Lecture)

#本工程支持的C++版本
set(CMAKE_CXX_STANDARD 17)


file(GLOB ASSETS "./assets" )

file(COPY ${ASSETS} DESTINATION ${CMAKE_BINARY_DIR})


include_directories(
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/include
)

link_directories(
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/thirdParty/lib
)

add_subdirectory(wrapper)
add_subdirectory(application)
add_subdirectory(glframework)

#本工程所有cpp文件编译链接，生成exe
#add_executable(openglStudy "glad.c" "main (2).cpp")
add_executable(openglStudy "main.cpp" "glad.c" )

target_link_libraries(openglStudy glfw3.lib wrapper app fw)