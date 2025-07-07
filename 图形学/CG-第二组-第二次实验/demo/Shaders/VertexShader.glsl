#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

//变换矩阵
uniform mat4 transMatrix;
uniform mat4 model;


out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;



void main()
{
    // 修正为 1.0
    gl_Position = transMatrix * vec4(aPos, 1.0);

//    vertexColor = aColor;

    //片段在空间中的位置

    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords= aTexCoords;
    //    TexUV = vec2(offset_u, offset_v) + scaleValue * aTexCoords;
}
