//#version 330 core
//out vec4 FragColor;
//in vec3 ThisPosition;
//
//uniform float ThisTime;
//const float M_PI = 3.14159265358979323846;
//
//void main() {
//    float position =ThisTime+ sqrt(pow(ThisPosition.x, 2) + pow(ThisPosition.y, 2) + pow(ThisPosition.z, 2));
//    FragColor = vec4(0.5f + 0.5f * sin(position),
//    0.5f + 0.5f * sin(position + 2.0f * M_PI / 3.0f),
//    0.5f + 0.5f * sin(position + 4.0f * M_PI / 3.0f),1.0f);
//
//}

#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexUV;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    // ambient
    vec3 ambient = light.ambient * texture(material.diffuse, TexUV).rgb;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexUV).rgb;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexUV).rgb;

    // attenuation
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient  *= attenuation;
    diffuse   *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}