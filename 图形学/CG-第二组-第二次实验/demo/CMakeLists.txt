cmake_minimum_required(VERSION 3.5)
project(CG_E2)

set(CMAKE_CXX_STANDARD 20)

# 静态链接设置（Windows专用）
if (WIN32)
    #    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -mwindows")

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
endif ()

# OpenGL相关路径配置
set(GL_API_DIR "${CMAKE_CURRENT_SOURCE_DIR}/public")
set(GL_libs_debug_x64 "${GL_API_DIR}/lib/debug_x64")
set(GL_libs_release_x64 "${GL_API_DIR}/lib/release_x64")
set(SHADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Shaders)
set(UTILITY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/utility)


# 设置可执行文件输出目录
set(EXECUTABLE_OUTPUT_DIR ${CMAKE_BINARY_DIR})

# 包含目录
include_directories(
        "${GL_API_DIR}/include"
        "${SHADER_DIR}"
        "${UTILITY_DIR}"
)

# 可执行目标
add_executable(CG_E2
        main.cpp
        "${GL_API_DIR}/src/glad.c"
        utility/Shader.cpp
        utility/Shader.h
        utility/stb_image.cpp
        utility/Camera.cpp
        utility/Camera.h
        CGEMesh.cpp
        CGEMesh.h
        Actor/Actor.cpp
        Actor/Actor.h
        Actor/Light.cpp
        Actor/Light.h
        utility/InputHandler.cpp
        utility/InputHandler.h
        utility/Timer.cpp
        utility/Timer.h
        Actor/Mesh.cpp
        Actor/Mesh.h
        Actor/Model.cpp
        Actor/Model.h
        Actor/SkyBox.cpp
        Actor/SkyBox.h
        Actor/geometry/box.h
        utility/ImageLoader.h
        Actor/geometry/Plane.cpp
        Actor/geometry/Plane.h
)

# 输出目录配置
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Debug配置
#    set(DEPENDENCY_IMPORT_LIBRARY "${GL_libs_debug_x64}/glfw3dll.lib")
    set(DEPENDENCY_IMPORT_LIBRARY "${GL_libs_debug_x64}/glfw3dll.lib" "${GL_libs_debug_x64}/libassimpd.dll.a")
    set(DEPENDENCY_DLL "${GL_libs_debug_x64}/glfw3.dll" "${GL_libs_debug_x64}/libassimp-5d.dll")
else ()
    # Release配置（默认）
#    set(DEPENDENCY_IMPORT_LIBRARY "${GL_libs_release_x64}/glfw3dll.lib")
    set(DEPENDENCY_IMPORT_LIBRARY "${GL_libs_release_x64}/glfw3dll.lib" "${GL_libs_release_x64}/libassimp.dll.a")
    set(DEPENDENCY_DLL "${GL_libs_release_x64}/glfw3.dll" "${GL_libs_release_x64}/libassimp-5.dll")
endif()

# 链接库
target_link_libraries(CG_E2
        ${DEPENDENCY_IMPORT_LIBRARY}
        opengl32.lib
)

## 将 DLL 依赖复制到目标文件夹
#add_custom_command(TARGET CG_E2 POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        "${DEPENDENCY_DLL}"
#        "${EXECUTABLE_OUTPUT_DIR}"
#        COMMENT "Copying ${DEPENDENCY_DLL} to ${EXECUTABLE_OUTPUT_DIR}"
#)
# 将 DLL 依赖复制到目标文件夹
foreach(DLL ${DEPENDENCY_DLL})
    add_custom_command(TARGET CG_E2 POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${DLL}
            "${EXECUTABLE_OUTPUT_DIR}"
            COMMENT "Copying ${DLL} to ${EXECUTABLE_OUTPUT_DIR}"
            VERBATIM
    )
endforeach()


## 复制Shader文件到输出目录
#file(GLOB SHADER_FILES "${SHADER_DIR}/*.glsl" "${SHADER_DIR}/*.frag")
#add_custom_command(TARGET CG_E2 POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E make_directory "${EXECUTABLE_OUTPUT_DIR}/Shaders"
#        COMMAND ${CMAKE_COMMAND} -E copy
#        ${SHADER_FILES}
#        "${EXECUTABLE_OUTPUT_DIR}/Shaders"
#        COMMENT "Copying shader files"
#)