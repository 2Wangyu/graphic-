#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D noiseTexture;  // 火焰噪声纹理
uniform float time;               // 时间参数
vec3 flameCoreColor = vec3(0.8, 0.48, 0.08); // 火焰核心颜色
vec3 flameEdgeColor = vec3(0.8, 0.1, 0.0); // 火焰边缘颜色
vec3 flameMidColor = vec3(0.9, 0.38, 0.09);    // 新增中间色


void main()
{
    // 多重噪声参数
    vec2 uv1 = TexCoords * vec2(2.0, 3.0);
    vec2 uv2 = TexCoords * vec2(1.5, 2.5);
    uv1.y -= time * 1.8;
    uv2.y -= time * 1.2;

    // 采样多层噪声（使用不同通道）
    float noise1 = texture(noiseTexture, uv1).r;
    float noise2 = texture(noiseTexture, uv2).g;
    float combinedNoise = (noise1 * 0.7 + noise2 * 0.3) * 1.2;

    // 动态扰动
    vec2 distortedUV = TexCoords + vec2(combinedNoise * 0.1, 0.0);
    float baseShape = 1.0 - smoothstep(0.3, 1.0, distortedUV.y);

    // 火焰形态计算
    float verticalGradient = pow(1.0 - TexCoords.y, 2.0); // 二次曲线衰减
    float flameShape = smoothstep(0.15, 0.65, verticalGradient * combinedNoise);

    // 颜色分层混合
    vec3 color = mix(flameEdgeColor, flameMidColor, smoothstep(0.3, 0.6, flameShape));
    color = mix(color, flameCoreColor, smoothstep(0.6, 0.9, flameShape));

    // 核心辉光效果
    float glow = smoothstep(0.8, 1.0, flameShape) * 0.6;
    color += vec3(1.0, 0.9, 0.5) * glow;

    // 动态透明度
    float alpha = smoothstep(0.3, 0.8, flameShape);
    alpha *= 0.8 - TexCoords.y * 0.4; // 顶部更透明

    // 高级闪烁效果（使用噪声控制）
    float flickerNoise = texture(noiseTexture, vec2(time * 0.2, 0.5)).b;
    float flicker = mix(0.8, 1.2, flickerNoise);
    color *= flicker;

    // 边缘扰动
    float edgeDistortion = sin(time * 8.0 + TexCoords.x * 10.0) * 0.02;
    color.r += edgeDistortion * (1.0 - flameShape);

    // 最终输出（添加Gamma校正）
    color = pow(color, vec3(1.0 / 2.2));
    FragColor = vec4(color, alpha * 0.9);
}