#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTexUV;

//变换矩阵
uniform mat4 transMatrix;
//贴图平移，放缩
uniform float scaleValue;
uniform float offset_u;
uniform float offset_v;
uniform mat4 model;

out vec3 vertexColor;
out vec2 TexUV;
out vec3 Normal;
out vec3 FragPos;

void main()
{
    // 修正为 1.0
    gl_Position = transMatrix * vec4(aPos, 1.0);

    vertexColor = aColor;

    //片段在空间中的位置
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal =vec3(model * vec4(aNormal, 1.0));
    TexUV = vec2(offset_u, offset_v) + scaleValue * aTexUV;
}