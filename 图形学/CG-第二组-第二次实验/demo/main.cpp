#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include <stb_image.h>
#include <random>
#include <glm/glm.hpp>
#include <unordered_map>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"
#include "Shader.h"


#include "Actor/Actor.h"
#include "Actor/Light.h"
#include "ImageLoader.h"
#include "Actor/SkyBox.h"
#include "Actor/geometry/Plane.h"

using namespace glm;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

void CalculateColor(const float &t, float &red, float &green, float &blue);


// settings
int WIDTH = 800;
int HEIGHT = 600;
float SkyBoxScale = .01;
float angle_X = 0;
float angle_Y = 0;
float angle_Z = 0;
float offset_u = 0, offset_v = 0;
float scaleValue = 1;
float mixValue = 0.2f;
bool FlashSwitch = false;
std::unordered_map<int, bool> keyPressTable;
//std::function<mat4(float, float, float, float)> GetProjectionMatrix = GetPerspectiveProjectionMatrix;

Camera camera = Camera(vec3(0.f, 0.f, 3.f), vec3(0.f, 1.0f, 0.f), vec3(0.f, 0.f, -1.f));

vec3 position = {0, 0, 0};

const char *VertexShader_Path = "../Shaders/VertexShader.glsl";
const char *SkyBoxVertexShader_Path = "../Shaders/SkyBoxVertexShader.glsl";

//片段着色器
const char *PointLightFrag_Path = "../Shaders/PointLight.frag";
const char *DirectionLightFrag_Path = "../Shaders/DirectionLight.frag";
const char *FlashLightFrag_Path = "../Shaders/FlashLight.frag";
const char *MultiLightFrag_Path = "../Shaders/MultiLight.frag";
const char *PBRFrag_Path = "../Shaders/PBRFragment.frag";

const char *LampFrag_Path = "../Shaders/LightFragment.frag";
//const char *VertexShader_Path = "Shaders/VertexShader.glsl";
//const char *PointLightFrag_Path = "Shaders/PointLight.frag";
//const char *FragmentSharder_Path2 = "Shaders/FragmentShader2.frag";

//
const char *container_path = "../tex/container.jpg";
const char *container2_path = "../tex/container2.png";
const char *container2_specular_path = "../tex/container2_specular.png";
const char *Tex1_Path = "../tex/tex1.jpg";
const char *Tex2_Path = "../tex/tex2.jpg";
const char *EmissionMap_Path = "../tex/EmissionMap.jpg";
const char *SmileFace_Path = "../tex/awesomeface.png";


enum class ShaderMode {
    PointLight,
    DirectLight,
    FlashLight,
    MultiLight
};

int main() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "ComputerGraphics_E1", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        //终止glfw,否则glfw在初次初始化后将不再接收任何初始化参数
        glfwTerminate();
        return -1;
    }
    //window被设置为当前的上下文主线程对象
    glfwMakeContextCurrent(window);

    //当窗口大小被改变后将会调用该回调函数,调整为新的的glViewpoint
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 隐藏鼠标并让其自由移动
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, [](GLFWwindow *window, double xpos, double ypos) {
        static double CursorX = xpos, CursorY = ypos;
        camera.TurnX(xpos - CursorX);
        camera.TurnY(ypos - CursorY);
        CursorX = xpos;
        CursorY = ypos;
    });


    //开启垂直同步
    glfwSwapInterval(1);


    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback([](GLenum source, GLenum type, GLuint id,
                              GLenum severity, GLsizei length,
                              const GLchar *message, const void *userParam) {
        std::cerr << "[GL DEBUG] id=" << id
                  << " severity=" << severity
                  << " message: " << message << std::endl;
    }, nullptr);


//    //准备微程序
//    Shader shader_PointLight = Shader(VertexShader_Path, PointLightFrag_Path);
//    Shader shader_DirectLight = Shader(VertexShader_Path, DirectionLightFrag_Path);
//    Shader shader_FlashLight = Shader(VertexShader_Path, FlashLightFrag_Path);
    Shader shader_MultiLight = Shader(VertexShader_Path, MultiLightFrag_Path);
//    Shader shader_PBR = Shader(VertexShader_Path, PBRFrag_Path);



    Shader Lampshader = Shader(VertexShader_Path, LampFrag_Path);
    Lampshader.use();
    Lampshader.setInt("noiseTexture", 0);
    unsigned int flameTex = Load_Tex("../model/flame2.jpg");
    unsigned int flameTex_green = Load_Tex("../model/flame_green.jpg");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, flameTex);


    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL Light Init Error: " << err << std::endl;
    }


    Shader SkyBoxShader = Shader(SkyBoxVertexShader_Path, "../Shaders/SkyBoxFragment.frag");
    unsigned int SkyBoxCubeMap;
    std::vector<const char *> BoxFaces;
    BoxFaces.emplace_back("../model/skybox/galaxy/galaxy+X(1).png");
    BoxFaces.emplace_back("../model/skybox/galaxy/galaxy-X(1).png");
    BoxFaces.emplace_back("../model/skybox/galaxy/galaxy+Y(1).png");
    BoxFaces.emplace_back("../model/skybox/galaxy/galaxy-Y(1).png");
    BoxFaces.emplace_back("../model/skybox/galaxy/galaxy+Z(1).png");
    BoxFaces.emplace_back("../model/skybox/galaxy/galaxy-Z(1).png");

    SkyBoxCubeMap = Load_CubeTex(BoxFaces);
    SkyBoxShader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, SkyBoxCubeMap);
    SkyBoxShader.setInt("skybox", 0);
    SkyBoxShader.setFloat("sky_scale", SkyBoxScale);
    err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL SkyBoxShader Init Error: " << err << std::endl;
    }

    //渲染目标信息准备
    //------------
    //纹理生成
//    stbi_set_flip_vertically_on_load(true);

//    unsigned int Tex_box2 = Load_Tex4f(container2_path);
//    unsigned int SpecularTex_box2 = Load_Tex4f(container2_specular_path);


//古代遗迹风格背景
    shared_ptr<Model> model = std::make_shared<Model>(
            Model("../model/Ancient-Corinth/Ancient-Corinth.fbx"));
    Actor Ancient(model);
    Ancient.SetWorldLocation(1.f, 1.f, 1.f);
    //Tree.SetWorldScale(2.f, 2.f, 2.f);
    //Y,X,Z
    Ancient.SetWorldRotation(-90.f, 0.f, 0.f);


//两把剑
    shared_ptr<Model> model2 = std::make_shared<Model>(
            Model("../model/sword/sword.obj"));
    Actor item2(model2);
    item2.SetWorldLocation(1.f, -5.5f, 7.f);
    item2.SetWorldScale(10.f, 10.f, 10.f);
    //Y,X,Z
    item2.SetWorldRotation(0.f, 0.f, 0.f);
    Actor item3(model2);
    item3.SetWorldLocation(-1.5f, -5.5f, 8.f);
    item3.SetWorldScale(10.f, 10.f, 10.f);
    //Y,X,Z



    //古墓
    shared_ptr<Model> model1 = std::make_shared<Model>(
            Model("../model/gumu/gumu.obj"));
    Actor item1(model1);
    item1.SetWorldLocation(-3.f, -6.8f, 1.f);
    item1.SetWorldScale(0.1f, 0.1f, 0.1f);
    //Tree.SetWorldScale(2.f, 2.f, 2.f);
    //Y,X,Z
    item1.SetWorldRotation(-1.f, -68.f, 6.5f);


    //雕像
    shared_ptr<Model> model4 = std::make_shared<Model>(
            Model("../model/violinist/statue.obj"));
    Actor item4(model4);
    item4.SetWorldLocation(-3.f, -5.5f, 5.3f);
    item4.SetWorldScale(0.2f, 0.2f, 0.2f);
    //Tree.SetWorldScale(2.f, 2.f, 2.f);
    //Y,X,Z
    item4.SetWorldRotation(7.f, -68.f, 6.5f);
    Actor item5(model4);
    item5.SetWorldLocation(0.f, -5.5f, 4.f);
    item5.SetWorldScale(0.2f, 0.2f, 0.2f);
    //Y,X,Z
    item5.SetWorldRotation(7.f, -68.f, 6.5f);

    //金属盆
    shared_ptr<Model> model9 = std::make_shared<Model>(
            Model("../model/cuup/cup.obj"));
    Actor item10(model9);
    item10.SetWorldLocation(-0.2f, -6.8f, 2.f);
    //x,减小向右，y增大向上,z减小向前
    item10.SetWorldScale(8.f, 8.f, 8.8f);
    //Y,X,Z
    //,Y从侧面转，增加左侧逆时针，x从上面转，减小是顺时针，Z从正面旋转。减小是顺时针
    //item10.SetWorldRotation(5.f,-68.f,5.5f);

    //人
    shared_ptr<Model> model5 = std::make_shared<Model>(
            Model("../model/MaleGreekWarrior/warrior.obj"));
    Actor item6(model5);
    item6.SetWorldLocation(-7.5f, -4.7f, 2.f);
    //x,减小向右，y增大向上,z减小向前
    item6.SetWorldScale(1.f, 1.f, 1.f);
    //Y,X,Z
    //x从上面转，减小是顺时针，Z从正面旋转。减小是顺时针,Y从侧面转
    item6.SetWorldRotation(-3.f, -150.f, -0.5f);

    //宝藏
    shared_ptr<Model> model6 = std::make_shared<Model>(
            Model("../model/pedistal/pedistal.obj"));
    Actor item7(model6);
    item7.SetWorldLocation(-3.f, -6.8f, 1.f);
    //x,减小向右，y增大向上,z减小向前
    item7.SetWorldScale(1.f, 1.f, 1.f);
    //Y,X,Z
    //,Y从侧面转，增加左侧逆时针，x从上面转，减小是顺时针，Z从正面旋转。减小是顺时针
    item7.SetWorldRotation(5.f, -68.f, 6.5f);

    //卷轴
    shared_ptr<Model> model7 = std::make_shared<Model>(
            Model("../model/scoll/scroll.obj"));
    Actor item8(model7);
    item8.SetWorldLocation(-3.f, -4.8f, 1.f);
    //x,减小向右，y增大向上,z减小向前
    item8.SetWorldScale(1.f, 1.f, 1.f);
    //Y,X,Z
    //,Y从侧面转，增加左侧逆时针，x从上面转，减小是顺时针，Z从正面旋转。减小是顺时针
    item8.SetWorldRotation(5.f, -68.f, 5.5f);


    //玉
    shared_ptr<Model> model8 = std::make_shared<Model>(
            Model("../model/yu/yu.obj"));
    Actor item9(model8);
    item9.SetWorldLocation(-3.f, -4.77f, 2.f);
    //x,减小向右，y增大向上,z减小向前
    item9.SetWorldScale(2.f, 2.f, 2.f);
    //Y,X,Z
    //,Y从侧面转，增加左侧逆时针，x从上面转，减小是顺时针，Z从正面旋转。减小是顺时针
    item9.SetWorldRotation(5.f, -68.f, 5.5f);
    Actor item14(model8);
    item14.SetWorldLocation(-3.f, -6.77f, 0.f);
    //x,减小向右，y增大向上,z减小向前
    item14.SetWorldScale(2.f, 2.f, 2.f);
    //Y,X,Z
    //,Y从侧面转，增加左侧逆时针，x从上面转，减小是顺时针，Z从正面旋转。减小是顺时针
    item14.SetWorldRotation(5.f, -68.f, 5.5f);
    Actor item15(model8);
    item15.SetWorldLocation(-4.f, -6.77f, -0.5f);
    //x,减小向右，y增大向上,z减小向前
    item15.SetWorldScale(2.f, 2.f, 2.f);
    //Y,X,Z
    //,Y从侧面转，增加左侧逆时针，x从上面转，减小是顺时针，Z从正面旋转。减小是顺时针
    item5.SetWorldRotation(5.f, -68.f, 5.5f);

    //保存完好盔甲
    shared_ptr<Model> model10 = std::make_shared<Model>(
            Model("../model/shield/shield.obj"));
    Actor item11(model10);
    item11.SetWorldLocation(-3.3f, -6.9f, 0.f);
    //x,减小向右，y增大向上,z减小向前
    item11.SetWorldScale(0.6f, 0.6f, 0.6f);
    //Y,X,Z
    //,Y从侧面转，增加左侧逆时针，x从上面转，减小是顺时针，Z从正面旋转。减小是顺时针
    item11.SetWorldRotation(5.f, -68.f, 5.5f);
    Actor item12(model10);
    item12.SetWorldLocation(-3.6f, -6.9f, 2.f);
    //x,减小向右，y增大向上,z减小向前
    item12.SetWorldScale(0.6f, 0.6f, 0.6f);
    //Y,X,Z
    //,Y从侧面转，增加左侧逆时针，x从上面转，减小是顺时针，Z从正面旋转。减小是顺时针
    item12.SetWorldRotation(5.f, -60.f, 5.5f);
    Actor item13(model10);
    item13.SetWorldLocation(-2.3f, -6.9f, 2.f);
    //x,减小向右，y增大向上,z减小向前
    item13.SetWorldScale(0.6f, 0.6f, 0.6f);
    //Y,X,Z
    //,Y从侧面转，增加左侧逆时针，x从上面转，减小是顺时针，Z从正面旋转。减小是顺时针
    item13.SetWorldRotation(5.f, 60.f, 5.5f);


    //月亮
    shared_ptr<Model> model11 = std::make_shared<Model>(
            Model("../model/moon/moon.obj"));
    Actor item16(model11);
    item16.SetWorldLocation(15.f, 20.f, 20.f);
    //x,减小向右，y增大向上,z减小向前
    item16.SetWorldScale(0.06f, 0.06f, 0.06f);





//    Plane plane;
//    plane.SetWorldLocation(0.f, -1.f, 0.f);
//
////
//    plane.setTexDiffuse(Load_Tex("../tex/Brick_albedo.jpg"));
//    plane.setTexNormal(Load_Tex("../tex/Brick_normal.jpg"));



//点光源


//    Lamp[0].SetWorldScale(0.1f, 0.1f, 0.1f);
//    Lamp[0].SetWorldLocation(2.0f, 2.0f, -2.0f);
//
//    Lamp[1].SetWorldScale(0.1f, 0.1f, 0.1f);
//    Lamp[1].SetWorldLocation(-2.0f, 2.0f, -2.0f);
//
//    Lamp[2].SetWorldScale(0.1f, 0.1f, 0.1f);
//    Lamp[2].SetWorldLocation(2.0f, -2.0f, -2.0f);
//
//    Lamp[3].SetWorldScale(0.1f, 0.1f, 0.1f);
//    Lamp[3].SetWorldLocation(-2.0f, -2.0f, -2.0f);
//
    SkyBox skyBox;
//    Lamp[4].SetWorldLocation(-3.f, -6.8f, 1.f); // 光源位置
//    Lamp[4].SetWorldScale(0.3f, 0.3f, 0.3f);

    //着色器准备完毕
    Shader &CurrentSharder = shader_MultiLight;
    ShaderMode shaderMode = ShaderMode::MultiLight;


//    switch (shaderMode) {
//        case ShaderMode::MultiLight:
    CurrentSharder = shader_MultiLight;
    CurrentSharder.use();

//            CurrentSharder.setVec3("dirLight.direction", 1.f, 1.0f, 1.f);
//            CurrentSharder.setVec3("dirLight.ambient", 0.6f, 0.6f, 0.6f);
//            CurrentSharder.setVec3("dirLight.diffuse", 0.8f, 0.8f, 0.8f);
//            CurrentSharder.setVec3("dirLight.specular", 0.3f, 0.3f, 0.3f);
//
//            CurrentSharder.setVec3("pointLights[0].position", Lamp[0].GetWorldLocation());
//            CurrentSharder.setVec3("pointLights[0].ambient", 0.f, 0.f, 0.f);
//            CurrentSharder.setVec3("pointLights[0].diffuse", 0.5f, 0.5f, 0.5f);
//            CurrentSharder.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
//            CurrentSharder.setFloat("pointLights[0].constant", 1.0f);
//            CurrentSharder.setFloat("pointLights[0].linear", 0.09f);
//            CurrentSharder.setFloat("pointLights[0].quadratic", 0.032f);
//            // point light 2
//            CurrentSharder.setVec3("pointLights[1].position", Lamp[1].GetWorldLocation());
//            CurrentSharder.setVec3("pointLights[1].ambient", 0.f, 0.f, 0.f);
//            CurrentSharder.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
//            CurrentSharder.setVec3("pointLights[1].specular", 0.8f, 0.8f, 0.8f);
//            CurrentSharder.setFloat("pointLights[1].constant", 1.0f);
//            CurrentSharder.setFloat("pointLights[1].linear", 0.09f);
//            CurrentSharder.setFloat("pointLights[1].quadratic", 0.032f);
//            // point light 3
//            CurrentSharder.setVec3("pointLights[2].position", Lamp[2].GetWorldLocation());
//            CurrentSharder.setVec3("pointLights[2].ambient", 0.f, 0.f, 0.f);
//            CurrentSharder.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
//            CurrentSharder.setVec3("pointLights[2].specular", 0.8f, 0.8f, 0.8f);
//            CurrentSharder.setFloat("pointLights[2].constant", 1.0f);
//            CurrentSharder.setFloat("pointLights[2].linear", 0.09f);
//            CurrentSharder.setFloat("pointLights[2].quadratic", 0.032f);
//            // point light 4
//            CurrentSharder.setVec3("pointLights[3].position", Lamp[3].GetWorldLocation());
//            CurrentSharder.setVec3("pointLights[3].ambient", 0.f, 0.f, 0.f);
//            CurrentSharder.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
//            CurrentSharder.setVec3("pointLights[3].specular", 0.8f, 0.8f, 0.8f);
//            CurrentSharder.setFloat("pointLights[3].constant", 1.0f);
//            CurrentSharder.setFloat("pointLights[3].linear", 0.09f);
//            CurrentSharder.setFloat("pointLights[3].quadratic", 0.032f);

// 月光（直射光）设置
    CurrentSharder.setBool("self_luminous", false);

    CurrentSharder.setVec3("dirLight.direction", -0.3f, -1.0f, 0.2f); // 斜射方向，增强立体感
    CurrentSharder.setVec3("dirLight.ambient", 0.15f, 0.15f, 0.15f);   // 灰白环境光（减少蓝调）
    CurrentSharder.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.45f);     // 冷白漫反射（保持月光感）
    CurrentSharder.setVec3("dirLight.specular", 0.2f, 0.2f, 0.2f);     // 适当增强高光
    Light Lamp[5];
    // 四个小红灯设置（遗迹四周）
    // 位置调整
    Lamp[0].SetWorldLocation(7.4f, -5.2f, -3.4f);  // 右前
    Lamp[1].SetWorldLocation(-11.5f, -5.6f, 4.0f); // 左前
    Lamp[2].SetWorldLocation(-6.9f, -5.6f, -8.2f); // 右后
    Lamp[0].SetWorldScale(0.5f, 0.5f, 0.5f);
    Lamp[1].SetWorldScale(0.5f, 0.5f, 0.5f);
    Lamp[2].SetWorldScale(0.5f, 0.5f, 0.5f);

    // 小红灯参数
    for (int i = 0; i < 4; i++) {
        CurrentSharder.setVec3("pointLights[" + std::to_string(i) + "].position", Lamp[i].GetWorldLocation());
        // 环境光：微弱红光（避免过亮）
        CurrentSharder.setVec3("pointLights[" + std::to_string(i) + "].ambient", 0.3f, 0.0f, 0.0f);

        // 漫反射：降低亮度但保持纯红（减少杂色）
        CurrentSharder.setVec3("pointLights[" + std::to_string(i) + "].diffuse", 1.0f, 0.3f, 0.3f);

        // 高光：保留微弱红色（避免白色高光干扰）
        CurrentSharder.setVec3("pointLights[" + std::to_string(i) + "].specular", 0.5f, 0.1f, 0.1f);

        // 扩大光源范围（让红光覆盖更广但更暗）
        CurrentSharder.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.05f);     // 原0.09
        CurrentSharder.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.09f);  // 原0.032
    }
    CurrentSharder.setVec3("pointLights[0].position", Lamp[0].GetWorldLocation());
    CurrentSharder.setVec3("pointLights[1].position", Lamp[1].GetWorldLocation());
    CurrentSharder.setVec3("pointLights[2].position", Lamp[2].GetWorldLocation());


    // 古墓内部灯光（使用第五个点光源）
    Lamp[3].SetWorldLocation(-2.5f, -6.0f, 2.f); // 调整到古墓内部
    CurrentSharder.setVec3("pointLights[3].position", Lamp[3].GetWorldLocation());
    CurrentSharder.setVec3("pointLights[3].ambient", 0.0f, 0.1f, 0.0f);  // 绿色环境
    CurrentSharder.setVec3("pointLights[3].diffuse", 0.2f, 0.75f, 0.2f);   // 幽绿色
    CurrentSharder.setVec3("pointLights[3].specular", 0.0f, 0.0f, 0.0f);  // 无高光
    CurrentSharder.setFloat("pointLights[3].linear", 0.5f);
    CurrentSharder.setFloat("pointLights[3].quadratic", 0.5f);


    //手电筒
    CurrentSharder.setVec3("flashLight.position", camera.Position);
    CurrentSharder.setVec3("flashLight.direction", camera.Lookat);
    CurrentSharder.setFloat("flashLight.cutOff", glm::cos(glm::radians(12.5f)));
    CurrentSharder.setFloat("flashLight.outerCutOff", glm::cos(glm::radians(17.5f)));
    CurrentSharder.setVec3("flashLight.ambient", 0.f, 0.f, 0.f);
    CurrentSharder.setVec3("flashLight.diffuse", 0.8f, 0.8f, 0.8f);
    CurrentSharder.setVec3("flashLight.specular", 1.0f, 1.0f, 1.0f);
    CurrentSharder.setFloat("flashLight.constant", 1.0f);
    CurrentSharder.setFloat("flashLight.linear", 0.09f);
    CurrentSharder.setFloat("flashLight.quadratic", 0.032f);


//            // 墓碑光源参数（在原有4个光源后添加）
//            CurrentSharder.setVec3("pointLights[4].position", Lamp[4].GetWorldLocation());
//            CurrentSharder.setVec3("pointLights[4].ambient", 0.0f, 0.0f, 0.0f);
//            CurrentSharder.setVec3("pointLights[4].diffuse", 0.1f, 0.8f, 0.3f); // 幽绿色
//            CurrentSharder.setVec3("pointLights[4].specular", 0.0f, 0.0f, 0.0f); // 无高光
//            CurrentSharder.setFloat("pointLights[4].constant", 1.0f);
//            CurrentSharder.setFloat("pointLights[4].linear", 0.07f);
//            CurrentSharder.setFloat("pointLights[4].quadratic", 0.017f); // 柔和衰减
//           // 在渲染循环内（约第436行后）
//            static float breathTime = 0.0f;
//            breathTime += 0.02f;
//            float intensity = (sin(breathTime) * 0.3f + 0.7f); // 波动范围0.4~1.0
//
//            CurrentSharder.use();
//            CurrentSharder.setVec3("pointLights[4].diffuse",
//                                   0.1f * intensity,
//                                   0.8f * intensity,
//                                   0.3f * intensity); // 动态调整颜色
//            break;
//    }


    CurrentSharder.setFloat("material.shininess", 32.0f);
    CurrentSharder.setInt("material.diffuse", 0);
    CurrentSharder.setInt("material.specular", 1);
//    CurrentSharder.setInt("material.emission", 2);

    mat4 ViewMatrix = glm::mat4(1.0f);
    mat4 ProjectionMatrix = glm::mat4(1.0f);
    mat4 modelMatrix4f = glm::mat4(1.0f);
    mat4 transMatrix = glm::mat4(1.0f);

    if (!SkyBoxShader.isValid()) {
        std::cout << "有效1\n";
    }
    if (!Lampshader.isValid()) {
        std::cout << "有效2\n";
    }
    //开启深度测试
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    while (!glfwWindowShouldClose(window)) {

        // input
        // -----
        processInput(window);


        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        ViewMatrix = camera.GetViewMatrix();
        ProjectionMatrix = glm::perspective(glm::radians(camera.Zoom), (float) WIDTH / (float) HEIGHT, 0.1f, 100.0f);


        glDepthFunc(GL_LEQUAL);    // 天空盒没有做模型变换，导致其深度异常
        glDepthMask(GL_FALSE);     // 禁止向深度缓冲写入
        if (!SkyBoxShader.isValid()) {
            GLenum err = glGetError();
            if (err != GL_NO_ERROR) {
                std::cerr << "OpenGL SkyBoxShader Error: " << err << std::endl;
            }

        }
        SkyBoxShader.use();
        SkyBoxShader.setMat4("view", ViewMatrix);
        SkyBoxShader.setMat4("projection", ProjectionMatrix);
        skyBox.Draw(SkyBoxShader);
        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            std::cerr << "OpenGL  SkyBox Error: " << err << std::endl;
        }
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);


        //着色器
        CurrentSharder.use();


        CurrentSharder.setVec3("flashLight.position", camera.Position);
        CurrentSharder.setVec3("flashLight.direction", camera.Lookat);

        CurrentSharder.setFloat("mixValue", mixValue);
        CurrentSharder.setFloat("offset_u", offset_u);
        CurrentSharder.setFloat("offset_v", offset_v);
        CurrentSharder.setFloat("scaleValue", scaleValue);

        auto ViewPoint = camera.GetViewPoint();
        CurrentSharder.setVec3("viewPos", camera.Position);


        CurrentSharder.setVec3("viewPos", camera.Position);
        CurrentSharder.setBool("FlashSwitch", FlashSwitch);

        if (!CurrentSharder.isValid()) {
            GLenum err = glGetError();
            if (err != GL_NO_ERROR) {
                std::cerr << "OpenGL  param Error: " << err << std::endl;
            }

        }
        //遗迹的内容
        Ancient.Draw(ViewMatrix, ProjectionMatrix, CurrentSharder);

//两把剑
        item2.Draw(ViewMatrix, ProjectionMatrix, CurrentSharder);
        item3.Draw(ViewMatrix, ProjectionMatrix, CurrentSharder);
        //古墓
        //雕像
        item4.Draw(ViewMatrix, ProjectionMatrix, CurrentSharder);
        item5.Draw(ViewMatrix, ProjectionMatrix, CurrentSharder);
        //人
        item6.Draw(ViewMatrix, ProjectionMatrix, CurrentSharder);
        //宝藏
        item7.Draw(ViewMatrix, ProjectionMatrix, CurrentSharder);
        //卷轴

        item8.Draw(ViewMatrix, ProjectionMatrix, CurrentSharder);
        //玉佩
        item9.Draw(ViewMatrix, ProjectionMatrix, CurrentSharder);
        item14.Draw(ViewMatrix, ProjectionMatrix, CurrentSharder);
        item15.Draw(ViewMatrix, ProjectionMatrix, CurrentSharder);
        //金属盆

        item10.Draw(ViewMatrix, ProjectionMatrix, CurrentSharder);

        //盔甲
        item11.Draw(ViewMatrix, ProjectionMatrix, CurrentSharder);

        item12.Draw(ViewMatrix, ProjectionMatrix, CurrentSharder);
        item13.Draw(ViewMatrix, ProjectionMatrix, CurrentSharder);

        CurrentSharder.setBool("self_luminous", true);
        item16.Draw(ViewMatrix, ProjectionMatrix, CurrentSharder);
        CurrentSharder.setBool("self_luminous", false);

        if (!Lampshader.isValid()) {
            GLenum err = glGetError();
            if (err != GL_NO_ERROR) {
                std::cerr << "OpenGL  Lampshader Error: " << err << std::endl;
            }

        }
//        Lamp[3].SetWorldLocation(position.x, position.y, position.z);
//        CurrentSharder.setVec3("pointLights[3].position", Lamp[3].GetWorldLocation());
//        cout<<position.x<<","<<position.y<<","<<position.z<<","<<std::endl;
        Lampshader.use();
        Lampshader.setFloat("time", glfwGetTime());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, flameTex);

        for (int i = 0; i < 3; i++) {
            Lamp[i].Draw(ViewMatrix, ProjectionMatrix, Lampshader);
        }

//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, flameTex_green);
//        Lamp[3].Draw(ViewMatrix, ProjectionMatrix, Lampshader);

        err = glGetError();
        if (err != GL_NO_ERROR) {
            std::cerr << "OpenGL Error: " << err << std::endl;
        }

        //划线模式和填充模式
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        //交换双缓冲内容,交换缓冲区并轮询输入输出（IO）事件（如按键按下 / 释放、鼠标移动等）。
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


//    glDeleteTextures(1, &Tex_box2);
//    glDeleteTextures(1, &Tex_SmileFace);
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {

    static bool HasPress = false;
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE) {
        HasPress = false;
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
        mixValue += 0.01f; // change this value accordingly (might be too slow or too fast based on system hardware)
        if (mixValue >= 1.0f)
            mixValue = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS) {
        mixValue -= 0.01f; // change this value accordingly (might be too slow or too fast based on system hardware)
        if (mixValue <= 0.0f)
            mixValue = 0.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        offset_v += 0.01f; // change this value accordingly (might be too slow or too fast based on system hardware)
        if (offset_v >= 1.0f)
            offset_v = offset_v - 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        offset_v -= 0.01f;
        if (offset_v <= 0.0f)
            offset_v = 1.0f - offset_v;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        offset_u += 0.01f; // change this value accordingly (might be too slow or too fast based on system hardware)
        if (offset_u >= 1.0f)
            offset_u = offset_u - 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        offset_u -= 0.01f;
        if (offset_u <= 0.0f)
            offset_u = 1.0f - offset_u;
    }

    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) {
        scaleValue += 0.01f; // change this value accordingly (might be too slow or too fast based on system hardware)
        if (scaleValue >= 1.0f)
            scaleValue = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
        scaleValue -= 0.01f;
        if (scaleValue <= 0.0f)
            scaleValue = 0.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        angle_X -= 0.2f;
        if (angle_X <= 0.0f) {
            angle_X = 360 + angle_Y;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        angle_Y -= 0.2f;
        if (angle_Y <= 0.0f) {
            angle_Y = 360 + angle_Y;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        angle_Z -= 0.2f;
        if (angle_Z <= 0.0f) {
            angle_Z = 360 + angle_Y;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS) {
        position.z -= 0.1;
    }
    if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS) {
        position.z += 0.1;
    }
    if (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS) {
        position.x -= 0.1;
    }
    if (glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS) {
        position.x += 0.1;
    }
    if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS) {
        position.y -= 0.1;
    }
    if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS) {
        position.y += 0.1;
    }


    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { ;
        camera.MoveForward(0.1f);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.MoveForward(-0.1f);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.MoveRight(-0.1f);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.MoveRight(0.1f);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera.MoveRise(+0.1f);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        camera.MoveRise(-0.1f);
    }

    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {

        auto it = keyPressTable.find(GLFW_KEY_V);
        if (it == keyPressTable.end()) {
            keyPressTable.emplace(GLFW_KEY_V, true);
            FlashSwitch = !FlashSwitch;
        } else if (!it->second) {
            FlashSwitch = !FlashSwitch;
        }

    } else if (glfwGetKey(window, GLFW_KEY_V) == GLFW_RELEASE) {
        auto it = keyPressTable.find(GLFW_KEY_V);
        if (it != keyPressTable.end())it->second = false;
    }


}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.

    WIDTH = width;
    HEIGHT = height;
    glViewport(0, 0, width, height);
}

void CalculateColor(const float &t, float &red, float &green, float &blue) {
    // 使用相位差为120度的正弦波（2π/3弧度）
    red = 0.5f + 0.5f * sin(t);
    green = 0.5f + 0.5f * sin(t + 2.0f * M_PI / 3.0f);
    blue = 0.5f + 0.5f * sin(t + 4.0f * M_PI / 3.0f);
}



