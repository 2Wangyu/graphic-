//
// Created by 分子轨道mo法 on 25-4-4.
//

#include "Light.h"
#include "geometry/box.h"


Light::Light() {

    glGenVertexArrays(1, &VAO);
    BindBoxMesh(VAO, BOX_FACE::OUTWARD);
}


void Light::Draw(mat4 ViewMat4, mat4 ProjectionMat4, Shader &shader) {
//    if(shader.isValid()) {
    shader.use();

    auto modelMatrix4f = this->GetModelMatrix4f();
    auto transMatrix = ProjectionMat4 * ViewMat4 * modelMatrix4f;
    shader.setMat4("transMatrix", transMatrix);
    shader.setMat4("model", modelMatrix4f);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

}




