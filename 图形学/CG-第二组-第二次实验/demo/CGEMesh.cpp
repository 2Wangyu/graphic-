#include <iostream>
#include "CGEMesh.h"

CGEMesh::CGEMesh() {

    glGenVertexArrays(1, &VAO);
    if (VAO == 0) {
        std::cerr << "Error: Failed to generate VAO." << std::endl;
    }

    VBO = 0;
    EBO = 0;
    count = 0;

    VBOBufferUsage = GL_STATIC_DRAW;
    EBOBufferUsage = GL_STATIC_DRAW;
    VBOIsSelf = false;
    EBOIsSelf = false;

    glBindVertexArray(VAO);
    glBindVertexArray(0);
}

void CGEMesh::SetVertexAttribPointer(GLuint index, GLint size, GLenum type,
                                     GLboolean normalized, GLsizei stride,
                                     const void *pointer) {
    if (!glIsVertexArray(VAO)) {
        std::cerr << "Error: VAO is not valid." << std::endl;
        return;
    }

    if (!glIsBuffer(VBO)) {
        std::cerr << "Error: VBO is not valid." << std::endl;
        glBindVertexArray(0);
        return;
    }

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
    glEnableVertexAttribArray(index);

    glBindVertexArray(0);
}

void CGEMesh::LoadMesh(const unsigned int &VerticesVBO, const unsigned int &IndicesEBO,
                       const GLsizei EBOcount) {
    if (!glIsVertexArray(VAO)) {
        std::cerr << "Error: VAO is not valid." << std::endl;
        return;
    }

    if (!glIsBuffer(VerticesVBO)) {
        std::cerr << "Error: Invalid Vertices VBO." << std::endl;
        return;
    }

    if (!glIsBuffer(IndicesEBO)) {
        std::cerr << "Error: Invalid Indices EBO." << std::endl;
        return;
    }

//    glBindVertexArray(VAO);
//    glBindBuffer(GL_ARRAY_BUFFER, VerticesVBO);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndicesEBO);

    VBO = VerticesVBO;
    EBO = IndicesEBO;
    VBOIsSelf = false;
    EBOIsSelf = false;
    count = EBOcount;
//
//    glBindVertexArray(0);
}

void CGEMesh::LoadMesh(const float *Vertices, const GLsizeiptr &sizeofVBO,
                       const unsigned int *Indices, const GLsizeiptr &sizeofEBO) {
    if (!glIsVertexArray(VAO)) {
        std::cerr << "Error: VAO is not valid." << std::endl;
        return;
    }

    // Create or reuse VBO
    if (!glIsBuffer(VBO) || !VBOIsSelf) {
        glGenBuffers(1, &VBO);
        if (VBO == 0) {
            std::cerr << "Error: Failed to generate VBO." << std::endl;
            return;
        }
        VBOIsSelf = true;
    }

    // Create or reuse EBO
    if (!glIsBuffer(EBO) || !EBOIsSelf) {
        glGenBuffers(1, &EBO);
        if (EBO == 0) {
            std::cerr << "Error: Failed to generate EBO." << std::endl;
            return;
        }
        EBOIsSelf = true;
    }

    glBindVertexArray(VAO);

    // Upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeofVBO, Vertices, VBOBufferUsage);

    // Upload index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeofEBO, Indices, EBOBufferUsage);

    count = static_cast<GLsizei>(sizeofEBO / sizeof(unsigned int));

    glBindVertexArray(0);
}

CGEMesh::~CGEMesh() {
    if (glIsVertexArray(VAO)) {
        glDeleteVertexArrays(1, &VAO);
    }

    if (VBOIsSelf && glIsBuffer(VBO)) {
        glDeleteBuffers(1, &VBO);
    }

    if (EBOIsSelf && glIsBuffer(EBO)) {
        glDeleteBuffers(1, &EBO);
    }
}

void CGEMesh::DrawElement() {
    if (!glIsVertexArray(VAO)) {
        std::cerr << "Error: VAO is not valid for drawing." << std::endl;
        return;
    }

    if (count <= 0) {
        std::cerr << "Warning: Attempt to draw with zero count." << std::endl;
        return;
    }

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}