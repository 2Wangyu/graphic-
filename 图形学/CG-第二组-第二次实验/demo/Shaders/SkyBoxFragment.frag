#version 330

in vec3 FragPos;

out vec4 FragColor;

uniform samplerCube skybox;

void main() {

    FragColor = texture(skybox, normalize(FragPos));

}

