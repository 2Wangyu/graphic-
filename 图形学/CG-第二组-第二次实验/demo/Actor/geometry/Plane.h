//
// Created by 分子轨道mo法 on 25-5-5.
//

#ifndef CG_E2_PLANE_H
#define CG_E2_PLANE_H


#include "glad/glad.h"
#include "../Actor.h"


inline void BindPlaneMesh(const unsigned int &vao) {

    static bool PlaneMeshHasInit = false;
    static unsigned int VBO = 0, EBO = 0;

    if (!PlaneMeshHasInit) {
        float vertices[] = {
                // 底面
                -1.f, 0.f, -1.f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                1.f, 0.f, -1.f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
                1.f, 0.f, 1.f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
                -1.f, 0.f, 1.f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f

        };
        unsigned int indices[6] = {
                0, 1, 2,
                2, 3, 0


        };
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        PlaneMeshHasInit = true;
    }

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}


class Plane : public Actor {

    unsigned int TexDiffuse;
    unsigned int TexSpecular;
    unsigned int TexNormal;


    unsigned int VAO;
public:
    Plane();
    void setTexNormal(unsigned int texNormal);


    void setTexSpecular(unsigned int texSpecular);

    void setTexDiffuse(unsigned int texDiffuse);

    void Draw(const Shader& shader);

};

#endif //CG_E2_PLANE_H
