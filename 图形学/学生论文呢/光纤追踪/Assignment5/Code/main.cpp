#include <filesystem>
#include "Scene.hpp"
#include "Sphere.hpp"
#include "Triangle.hpp"
#include "Light.hpp"
#include "Renderer.hpp"
// main.cpp
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
int main() {
    std::cout << "当前工作目录: " << std::filesystem::current_path() << std::endl;
    Scene scene(2000, 2000);
    scene.backgroundColor = Vector3f(0.1f, 0.12f, 0.15f); // 深蓝灰

    // 设置背景图片（添加这行代码）
    scene.SetBackgroundImage("../top.jpg"); // 替换为你的图片路径
    // 添加多个小球
    auto sph1 = std::make_unique<Sphere>(Vector3f(-1, 0, -12), 2);
    sph1->materialType = DIFFUSE_AND_GLOSSY;
    sph1->diffuseColor = Vector3f(1.0f, 0.2f, 0.2f);
    scene.Add(std::move(sph1));

//    auto sph3 = std::make_unique<Sphere>(Vector3f(3.0f, 0.0f, -10.0f), 1.0f);
//    sph3->materialType = DIFFUSE_AND_GLOSSY;
//    sph3->diffuseColor = Vector3f(1.0f, 0.2f, 0.2f);
//    scene.Add(std::move(sph3));

    auto sph2 = std::make_unique<Sphere>(Vector3f(0.5f, -0.5f, -8), 1.5f);
    sph2->ior = 1.5f;
    sph2->materialType = REFLECTION_AND_REFRACTION;
    scene.Add(std::move(sph2));



    // 添加三角形地板和光源（原有代码）
    Vector3f verts[4] = {{-5,-3,-6}, {5,-3,-6}, {5,-3,-16}, {-5,-3,-16}};
    uint32_t vertIndex[6] = {0, 1, 3, 1, 2, 3};
    Vector2f st[4] = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    auto mesh = std::make_unique<MeshTriangle>(verts, vertIndex, 2, st);
    mesh->materialType = DIFFUSE_AND_GLOSSY;
    scene.Add(std::move(mesh));
// 添加主光源 - 强光从左上照射
    scene.Add(std::make_unique<Light>(Vector3f(-30, 100, 30), Vector3f(1.0f, 1.0f, 0.9f)));

// 添加辅助光源 - 从右侧照射的冷色调光
    scene.Add(std::make_unique<Light>(Vector3f(40, 60, -15), Vector3f(0.6f, 0.7f, 1.0f)));

// 添加背光 - 从后方照射的弱光
    scene.Add(std::make_unique<Light>(Vector3f(0, 50, -30), Vector3f(0.3f, 0.3f, 0.4f)));

// 可选：添加环境光 (通过Scene类的ambientLight属性设置)
    scene.ambientLight = Vector3f(0.1f, 0.1f, 0.15f);  // 微弱的蓝色环境光
    Renderer r;
    r.Render(scene);

    return 0;
}