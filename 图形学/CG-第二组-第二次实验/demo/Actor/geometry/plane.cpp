//
// Created by 分子轨道mo法 on 25-5-5.
//

#include "Plane.h"

void Plane::Draw(const Shader& shader) {


    shader.use();
    glActiveTexture(GL_TEXTURE0);
    shader.setInt("Materia.texture_diffuse",0);
    glBindTexture(GL_TEXTURE_2D, TexDiffuse);

    glActiveTexture(GL_TEXTURE1);
    shader.setInt("Materia.texture_specular",1);
    glBindTexture(GL_TEXTURE_2D, TexSpecular);

    glActiveTexture(GL_TEXTURE2);
    shader.setInt("Materia.texture_normal",2);
    glBindTexture(GL_TEXTURE_2D, TexNormal);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

//    GLenum err = glGetError();
//    if (err != GL_NO_ERROR) {
//        std::cerr << "OpenGL Error in Plane draw: " << err << std::endl;
//    }
}

Plane::Plane() {
    glGenVertexArrays(1, &VAO);
    BindPlaneMesh(VAO);
}

void Plane::setTexNormal(unsigned int texNormal) {
    TexNormal = texNormal;
}

void Plane::setTexSpecular(unsigned int texSpecular) {
    TexSpecular = texSpecular;
}

void Plane::setTexDiffuse(unsigned int texDiffuse) {
    TexDiffuse = texDiffuse;
}
