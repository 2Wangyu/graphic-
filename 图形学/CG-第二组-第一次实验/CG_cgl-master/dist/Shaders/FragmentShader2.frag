#version 330 core
out vec4 FragColor;
in vec3 ThisPosition;

uniform float ThisTime;
const float M_PI = 3.14159265358979323846;

void main() {
    float position =ThisTime+ sqrt(pow(ThisPosition.x, 2) + pow(ThisPosition.y, 2) + pow(ThisPosition.z, 2));
    FragColor = vec4(0.5f + 0.5f * sin(position),
    0.5f + 0.5f * sin(position + 2.0f * M_PI / 3.0f),
    0.5f + 0.5f * sin(position + 4.0f * M_PI / 3.0f),1.0f);

}