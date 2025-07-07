#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 projection;
uniform float sky_scale;

out vec3 FragPos;

void main()
{
    FragPos = aPos;
    // 去掉视图矩阵的平移分量，只用旋转，这样天空盒不会跟随相机移动
    mat4 rotView = mat4(mat3(view));
    gl_Position = projection * rotView * vec4(aPos, 1.0) * sky_scale;
}
