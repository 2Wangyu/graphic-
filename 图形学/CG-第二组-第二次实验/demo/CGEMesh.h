//
// Created by 分子轨道mo法 on 25-4-4.
//

#ifndef CG_E1_1_STATICMESH_H
#define CG_E1_1_STATICMESH_H

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <memory>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};
struct Texture {
    unsigned int id;
    std::string type;
};


class CGEMesh {
//允许多个Mesh引用同一个Mesh数据,允许只有opengl内存对象复用
    std::shared_ptr<std::vector<Vertex>> vertices;
    std::shared_ptr<std::vector<unsigned int>> indices;
    std::vector<Texture> textures;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    GLsizei count;
    GLenum VBOBufferUsage;
    GLenum EBOBufferUsage;

    bool VBOIsSelf;
    bool EBOIsSelf;


public:
    CGEMesh();

    ~CGEMesh();


    /**
     * 读取已有的VBO和EBO对象
     * @param VerticesVBO 已有的VBO对象
     * @param IndicesEBO 已有的EBO对象
     */
    void LoadMesh(const unsigned int &VerticesVBO, const unsigned int &IndicesEBO, const int EBOcount);

    /**
     * 读取顶点数据，并加入buffer
     * @param Vertices
     * @param Indices
     */
    void LoadMesh(const float *Vertices, const GLsizeiptr &sizeofVBO, const unsigned int *Indices,
                  const GLsizeiptr &sizeofEBO);

    /**
     * 解释缓冲数据，将会被绑定在当前VAO对象上
     * @param index 数据通道
     * @param size 数据个数
     * @param type 数据类型
     * @param normalized 是否归一化
     * @param stride 步进
     * @param pointer 首地址偏移量
     */
    void SetVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride,
                                const void *pointer);

    void DrawElement();

};


#endif //CG_E1_1_STATICMESH_H
