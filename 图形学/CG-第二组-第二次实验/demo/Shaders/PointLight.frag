#version 330 core
out vec4 FragColor;


//in vec3 vertexColor;

in vec2 TexUV;
in vec3 Normal;
in vec3 FragPos;

//漫反射贴图和高光贴图
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 viewPos;

//uniform float mixValue;
//uniform sampler2D Texture;
//uniform sampler2D Texture2;

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

    //    vec3 result = ambient + diffuse + specular + texture(material.emission, TexUV).rgb;
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}