#pragma once

#include <vector>
#include <memory>
#include <string>
#include "Vector.hpp"
#include "Object.hpp"
#include "Light.hpp"

#include "stb_image.h"

class Scene
{
public:
    // 屏幕大小
    int width = 1280;
    int height = 960;

    // 可视角度
    double fov = 90;
    // 背景颜色 RGB格式
    Vector3f backgroundColor = Vector3f(0.235294, 0.67451, 0.843137);
    // 最大递归深度
    int maxDepth = 20;
    // 这个偏移量的目的是避免 反射光 或者 折射光 与命中点所在的表面相交
    float epsilon = 0.00001;

    // 添加环境光
    Vector3f ambientLight = Vector3f(0.1f, 0.1f, 0.1f);

    // 添加雾效参数
    float fogDensity = 0.02f;
    Vector3f fogColor = Vector3f(0.7f, 0.8f, 1.0f);
    // 背景图片相关
    unsigned char* backgroundImage = nullptr;
    int bgWidth = 0;
    int bgHeight = 0;
    int bgChannels = 0;
    std::string bgImagePath = "";

    // 构造函数
    Scene(int w, int h) : width(w), height(h) {}
    ~Scene() {
        if (backgroundImage) {
            stbi_image_free(backgroundImage);
        }
    }

    // 设置背景图片
    void SetBackgroundImage(const std::string& path) {
        // 如果已有背景图片，先释放
        if (backgroundImage) {
            stbi_image_free(backgroundImage);
            backgroundImage = nullptr;
        }

        // 加载新图片
        backgroundImage = stbi_load(path.c_str(), &bgWidth, &bgHeight, &bgChannels, 0);
        if (backgroundImage) {
            bgImagePath = path;
        } else {
            std::cerr << "Failed to load background image: " << path << std::endl;
        }
    }

    // 获取背景颜色（考虑背景图片）
    Vector3f GetBackgroundColor(float u, float v) const {
        if (!backgroundImage) {
            return backgroundColor;
        }

        // 使用自定义的clamp函数
        u = clamp(0.0f, 1.0f, u);
        v = clamp(0.0f, 1.0f, v);

        // 计算图片坐标
        int x = static_cast<int>(u * (bgWidth - 1));
        int y = static_cast<int>((1 - v) * (bgHeight - 1)); // 图片坐标系y轴向下

        int index = (y * bgWidth + x) * bgChannels;

        // 根据通道数返回颜色
        if (bgChannels >= 3) {
            return Vector3f(
                    backgroundImage[index] / 255.0f,
                    backgroundImage[index + 1] / 255.0f,
                    backgroundImage[index + 2] / 255.0f
            );
        } else if (bgChannels == 1) {
            float val = backgroundImage[index] / 255.0f;
            return Vector3f(val, val, val);
        }

        return backgroundColor;
    }

    // 将物体或者光源添加到场景中来
    void Add(std::unique_ptr<Object> object) { objects.push_back(std::move(object)); }
    void Add(std::unique_ptr<Light> light) { lights.push_back(std::move(light)); }

    [[nodiscard]] const std::vector<std::unique_ptr<Object> >& get_objects() const { return objects; }
    [[nodiscard]] const std::vector<std::unique_ptr<Light> >&  get_lights() const { return lights; }

private:
    // creating the scene (adding objects and lights)
    std::vector<std::unique_ptr<Object> > objects;
    std::vector<std::unique_ptr<Light> > lights;
};