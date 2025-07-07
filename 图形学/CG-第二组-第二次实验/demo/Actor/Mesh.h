//
// Created by 分子轨道mo法 on 25-4-16.
//

#ifndef CG_E2_MESH_H
#define CG_E2_MESH_H

#include <string>
#include <memory>
#include "glm/glm.hpp"
#include "Shader.h"

#define MAX_BONE_INFLUENCE 4

struct MinVertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
};

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};
struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};


class Mesh {

    //允许多个Mesh引用同一个Mesh数据,允许只有opengl内存对象复用
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    std::vector<Texture> Textures;
    unsigned int VAO;


public:
    Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices,
         const std::vector<Texture> &textures);

    void Draw(Shader &shader);


private:
    unsigned int VBO, EBO;

    void SetUp();

};


#endif //CG_E2_MESH_H
