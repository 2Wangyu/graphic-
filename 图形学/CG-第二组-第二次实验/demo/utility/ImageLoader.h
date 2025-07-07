//
// Created by 分子轨道mo法 on 25-4-29.
//

#ifndef CG_E2_IMAGELOADER_H
#define CG_E2_IMAGELOADER_H


#include <iostream>
#include "glad/glad.h"
#include "stb_image.h"

inline unsigned int Load_Tex(const char *filename) {
    //纹理生成
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //tex读取
    int width, height, nrChannels;
    //分别获取图片高度宽度以及颜色通道数
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (data) {
        /**
     *texture内存解释:
     *target:意味着此函数影响当前绑定的2D纹理对象,而1D和3D不受影响;
     *level:生成mipmap级别
     *internalformat:gl存储纹理类型
     *width,height:不解释
     *border:gl的屎山遗留物，不用管
     *format:原图像格式
     *type:数据类型,此处为char(byte)数组
     *图像数据
     */
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        //生成mipmap(如果不需要则可省略);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(data);

        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            std::cerr << "OpenGL Texture Error: " << err << std::endl;
        }

        return texture;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    std::cout << "Failed to load texture" << std::endl;
    return 0;
}

inline unsigned int Load_Tex4f(const char *filename) {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    //tex读取
    int width, height, nrChannels;
    //分别获取图片高度宽度以及颜色通道数
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (data) {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);

        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            std::cerr << "OpenGL Texture Error: " << err << std::endl;
        }
        return texture;
    }

    std::cout << "Failed to load texture" << std::endl;
    return 0;
}


inline unsigned int Load_CubeTex(const std::vector<const char *>& faces) {
    if (faces.size() != 6) {
        std::cerr << "Error: LoadCubemap requires exactly 6 face file paths." << std::endl;
        return 0;
    }

    unsigned int texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); ++i) {
        unsigned char* data = stbi_load(faces[i], &width, &height, &nrChannels, 0);
        if (data) {
            GLenum format = (nrChannels == 4 ? GL_RGBA : GL_RGB);
            glTexImage2D(
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0,                          // mipmap level
                    format,                     // internal format
                    width, height,
                    0,                          // border
                    format,                     // source format
                    GL_UNSIGNED_BYTE,
                    data                        // pixel data
            );
            stbi_image_free(data);
        } else {
            std::cerr << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
            glDeleteTextures(1, &texID);
            return 0;
        }
    }

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // Generate mipmaps
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    // Unbind
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    return texID;
}

#endif //CG_E2_IMAGELOADER_H
