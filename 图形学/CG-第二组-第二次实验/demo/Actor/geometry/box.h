//
// Created by 分子轨道mo法 on 25-4-29.
//

#ifndef CG_E2_BOX_H
#define CG_E2_BOX_H


#include "glad/glad.h"

enum class BOX_FACE {
    INWARD,
    OUTWARD
};



    inline void BindBoxMesh(const unsigned int &vao, BOX_FACE type) {

        static bool BoxMeshHasInitInward = false;
        static bool BoxMeshHasInitOutward = false;
        static unsigned int VBO_In = 0, VBO_Out = 0, EBO_In = 0, EBO_Out = 0;

        switch (type) {
            case BOX_FACE::INWARD:
                if (!BoxMeshHasInitInward) {
                    float vertices[] = {
                            // 后面
                            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
                            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
                            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

                            // 正面
                            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
                            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
                            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
                            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
                            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
                            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
                            // 左面
                            -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                            -0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                            -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                            -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

                            // 右面
                            0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                            0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                            0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                            0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                            0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                            0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

                            // 底面
                            0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
                            0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
                            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                            -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                            0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
                            // 顶面
                            -0.5f, 0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                            0.5f, 0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
                            0.5f, 0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
                            0.5f, 0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
                            -0.5f, 0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
                            -0.5f, 0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f
                    };
                    unsigned int indices[36];
                    for (unsigned int i = 0; i < 36; i++) {
                        indices[i] = i;
                    }
                    glGenBuffers(1, &VBO_In);
                    glBindBuffer(GL_ARRAY_BUFFER, VBO_In);
                    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

                    glGenBuffers(1, &EBO_In);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_In);
                    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

                    BoxMeshHasInitInward = true;
                }


                glBindVertexArray(vao);
                glBindBuffer(GL_ARRAY_BUFFER, VBO_In);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_In);

                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) nullptr);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
                glEnableVertexAttribArray(2);

                glBindVertexArray(0);

                break;
            case BOX_FACE::OUTWARD:
                if (!BoxMeshHasInitOutward) {
                    float vertices[] = {
                            // 后面
                            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
                            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
                            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
                            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
                            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
                            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

                            // 正面
                            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
                            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
                            // 左面
                            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                            -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                            -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

                            // 右面
                            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                            0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

                            // 底面
                            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
                            0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
                            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                            -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
                            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
                            // 顶面
                            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                            0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
                            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
                            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
                            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f
                    };
                    unsigned int indices[36];
                    for (unsigned int i = 0; i < 12; i++) {
                        indices[3 * i] = 3 * i + 2;
                        indices[3 * i + 1] = 3 * i + 1;
                        indices[3 * i + 2] = 3 * i;
                    }
                    glGenBuffers(1, &VBO_Out);
                    glBindBuffer(GL_ARRAY_BUFFER, VBO_Out);
                    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

                    glGenBuffers(1, &EBO_Out);
                    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Out);
                    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

                    BoxMeshHasInitOutward = true;
                }


                glBindVertexArray(vao);
                glBindBuffer(GL_ARRAY_BUFFER, VBO_Out);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Out);

                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) nullptr);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
                glEnableVertexAttribArray(2);

                glBindVertexArray(0);


                break;
            default:
                throw std::runtime_error("没有指定面类型");
        }


    }


#endif //CG_E2_BOX_H
