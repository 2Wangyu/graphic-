#version 330
out vec4 FragColor;


in vec2 TexCoords;
//in vec3 Normal;
in vec3 FragPos;


//漫反射贴图和高光贴图
struct Material {
    sampler2D texture_diffuse;
    sampler2D texture_specular;
    sampler2D texture_normal;
    sampler2D texture_height;
    float shininess;
};

struct DirectLigh {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct PointLight {
    vec3 position;
    //片段贡献、衰减
    float constant;
    float linear;
    float quadratic;


    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct FlashLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};



uniform Material material;
uniform DirectLigh dirLight;

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform FlashLight flashLight;
uniform bool FlashSwitch;

uniform vec3 viewPos;

vec3 CalculateDirectLight(DirectLigh light, vec3 normal, vec3 viewDir);
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalculateFlashLight(FlashLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

uniform bool self_luminous;

void main() {
    vec3 result;


    // 属性
    vec3 norm = normalize(vec3(texture(material.texture_normal, TexCoords)));
    vec3 viewDir = normalize(viewPos - FragPos);

    // 第一阶段：定向光照
    result = CalculateDirectLight(dirLight, norm, viewDir);
    if(self_luminous)
    result += 3.0*vec3(texture(material.texture_diffuse, TexCoords));

    // 第二阶段：点光源
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
    result += CalculatePointLight(pointLights[i], norm, FragPos, viewDir);
    // 第三阶段：聚光
    //result += CalcSpotLight(spotLight, norm, FragPos, viewDir);

    //第四阶段:手电筒
    if (FlashSwitch)
    result += CalculateFlashLight(flashLight, norm, FragPos, viewDir);
    //    result += vec3(0.1, 0.1, 0.1);

    FragColor = vec4(result, 1.0);

}


vec3 CalculateDirectLight(DirectLigh light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 合并结果
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular, TexCoords));
    return (ambient + diffuse + specular);
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {

    vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 衰减
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
    light.quadratic * (distance * distance));
    // 合并结果
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular, TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalculateFlashLight(FlashLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {

    vec3 ambient = light.ambient * texture(material.texture_diffuse, TexCoords).rgb;

    // diffuse
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.texture_diffuse, TexCoords).rgb;

    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.texture_specular, TexCoords).rgb;

    // spotlight (soft edges)
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse *= intensity;
    specular *= intensity;

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}