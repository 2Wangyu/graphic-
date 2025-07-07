/*
 * PBR Shader (Cook-Torrance) using Albedo, Roughness, Metalness, Normal maps
 * GLSL 330 core
 */
// Fragment Shader
#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoords;
in vec3 Normal;

struct Light {
    vec3 position;
    vec3 color;
};

#define NR_LIGHTS 4
uniform Light lights[NR_LIGHTS];
uniform vec3 camPos;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;

const float PI = 3.14159265359;

// Normal Distribution Function: GGX/Trowbridge-Reitz
float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

// Geometry: Schlick-GGX
float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;
    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

mat3 ComputeTBN(vec3 N) {
    // 以世界空间位置和 UV 的导数构建切线和副切线
    vec3 dp1 = dFdx(FragPos);
    vec3 dp2 = dFdy(FragPos);
    vec2 duv1 = dFdx(TexCoords);
    vec2 duv2 = dFdy(TexCoords);
    vec3 tangent = normalize(duv2.y * dp1 - duv1.y * dp2);
    vec3 bitangent = normalize(-duv2.x * dp1 + duv1.x * dp2);
    mat3 TBN = mat3(tangent, bitangent, N);
    return TBN;
}

void main() {
    vec3 albedo = pow(texture(albedoMap, TexCoords).rgb, vec3(2.2));
    float metallic  = texture(metallicMap, TexCoords).r;
    float roughness = texture(roughnessMap, TexCoords).r;
    float ao        = texture(aoMap, TexCoords).r;

    vec3 N = normalize(Normal);
    // 采样法线贴图并转换到世界空间
    vec3 normalTex = texture(normalMap, TexCoords).rgb;
    normalTex = normalTex * 2.0 - 1.0;
    mat3 TBN = ComputeTBN(N);
    N = normalize(TBN * normalTex);

    vec3 V = normalize(camPos - FragPos);
    vec3 F0 = mix(vec3(0.04), albedo, metallic);

    vec3 Lo = vec3(0.0);
    for(int i=0; i<4; ++i) {
        vec3 L = normalize(lights[i].position - FragPos);
        vec3 H = normalize(V + L);
        float dist = length(lights[i].position - FragPos);
        vec3 radiance = lights[i].color / (dist*dist);

        float NDF = DistributionGGX(N, H, roughness);
        float G   = GeometrySmith(N, V, L, roughness);
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
        vec3 spec = (NDF * G * F) / (4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001);

        vec3 kS = F;
        vec3 kD = (1.0 - kS) * (1.0 - metallic);
        Lo += (kD * albedo / PI + spec) * radiance * max(dot(N, L), 0.0);
    }

    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo;
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));

    FragColor = vec4(color, 1.0);
}

