//
// Created by 分子轨道mo法 on 25-4-29.
//

#include "SkyBox.h"
#include "geometry/box.h"
#include "ImageLoader.h"

SkyBox::SkyBox() {
    glGenVertexArrays(1, &VAO);
    BindBoxMesh(VAO, BOX_FACE::INWARD);

}

void SkyBox::Draw(Shader& shader) {


    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL Error in SkyBox draw: " << err << std::endl;
    }

}



