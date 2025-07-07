#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include "Eigen/Eigen"
#include <stb_image.h>

#include "Camera.h"
#include "Shader.h"
#include "TransMatrix.h"
#include "Actor.h"

using namespace Eigen;


void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

void CalculateColor(const float &t, float &red, float &green, float &blue);

unsigned int Load_Tex(const char *filename);

unsigned int Load_Tex4f(const char *filename);

// settings
int WIDTH = 800;
int HEIGHT = 600;
// stores how much we're
// seeing of either texture
float angle_X = 0;
float angle_Y = 0;
float angle_Z = 0;
float offset_u = 0, offset_v = 0;
float scaleValue = 1;
float modelScale = 1;
float mixValue = 0.2f;
bool flag = true;
std::function<Matrix4f(float, float, float, float)> GetProjectionMatrix = GetPerspectiveProjectionMatrix;

Camera camera = Camera(Vector3f(0.f, 0.f, 3.f), Vector3f(0.f, 1.0f, 0.f), Vector3f(0.f, 0.f, 1.f));

Vector3f position = {0, 0, 0};

const char *VertexShader_Path = "../Shaders/VertexShader.glsl";
const char *FragmentSharder_Path = "../Shaders/FragmentShader.frag";
const char *FragmentSharder_Path2 = "../Shaders/FragmentShader2.frag";
const char *FragmentSharder_Path3 = "../Shaders/LightFragment.frag";
//
//const char *VertexShader_Path = "Shaders/VertexShader.glsl";
//const char *FragmentSharder_Path = "Shaders/FragmentShader.frag";
//const char *FragmentSharder_Path2 = "Shaders/FragmentShader2.frag";
//const char *FragmentSharder_Path3 = "Shaders/LightFragment.frag";
//
const char *container_path = "../tex/container.jpg";
const char *Tex1_Path = "../tex/tex1.jpg";
const char *Tex2_Path = "../tex/tex2.jpg";
const char *SmileFace_Path = "../tex/awesomeface.png";

//const char *container_path = "tex/container.jpg";
//const char *Tex1_Path = "tex/tex1.jpg";
//const char *Tex2_Path = "tex/tex2.jpg";
//const char *SmileFace_Path = "tex/awesomeface.png";

int main() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
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
    int nrAttributes;


    //准备微程序
    Shader shader1 = Shader(VertexShader_Path, FragmentSharder_Path);
//    Shader shader2 = Shader(VertexShader_Path, FragmentSharder_Path2);
    Shader Lampshader = Shader(VertexShader_Path, FragmentSharder_Path3);
    //着色器准备完毕


    //渲染目标信息准备
    //------------
    //纹理生成
    stbi_set_flip_vertically_on_load(true);
    unsigned int texture1 = Load_Tex(container_path);
    unsigned int texture2 = Load_Tex4f(SmileFace_Path);
    unsigned int texture3 = Load_Tex(Tex1_Path);
    unsigned int texture4 = Load_Tex(Tex2_Path);

    float vertices[] = {
            // 前面 (z = -0.5)                  //法线
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // 0
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // 1
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // 2
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, // 3

            // 后面 (z = 0.5)
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // 4 (U翻转)
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 5
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // 6
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // 7

            // 左面 (x = -0.5)
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // 8 (U映射到z轴)
            -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 9
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // 10
            -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 11

            // 右面 (x = 0.5)
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 12 (U映射到z轴)
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // 13
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 14
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // 15

            // 底面 (y = -0.5)
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // 16 (V翻转)
            0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // 17
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // 18
            -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // 19

            // 顶面 (y = 0.5)
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // 20
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // 21
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 22
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f  // 23
    };

    unsigned int indices[] = {
            // 前面
            0, 1, 2, 2, 3, 0,

            // 后面
            4, 5, 6, 6, 7, 4,

            // 左面
            8, 9, 10, 10, 11, 8,

            // 右面
            12, 13, 14, 14, 15, 12,

            // 底面
            16, 17, 18, 18, 19, 16,

            // 顶面
            20, 21, 22, 22, 23, 20
    };

    Actor box1;
    Actor Lamp;
//
    unsigned int VBO0, EBO0;

    glGenBuffers(1, &VBO0);
    glGenBuffers(1, &EBO0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO0);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    box1.Mesh->LoadMesh(VBO0, EBO0, sizeof(indices) / sizeof(unsigned int));
    box1.Mesh->SetVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    box1.Mesh->SetVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    box1.Mesh->SetVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));

    Lamp.Mesh->LoadMesh(VBO0, EBO0, sizeof(indices) / sizeof(unsigned int));
    Lamp.Mesh->SetVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    Lamp.SetWorldScale(0.2f, 0.2f, 0.2f);
    Lamp.SetWorldLocation(0.8f, 1.2f, 0.8f);

    float floor_vertices[] = {
            //     ---- 位置 ----              - 纹理坐标 -
            0.9f, -0.7f, -.9f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // 右上
            0.9f, -0.7f, .9f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 右下
            -0.9f, -0.7f, .9f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // 左下
            -0.9f, -0.7f, -.9f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f // 左上
    };
    unsigned int floor_ind[] = {
            0, 1, 3, // first triangle
            1, 2, 3 // second triangle
    };

    Actor floor;
    floor.Mesh->LoadMesh(floor_vertices, sizeof(floor_vertices), floor_ind, sizeof(floor_ind));
    floor.Mesh->SetVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    floor.Mesh->SetVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    floor.Mesh->SetVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));


    shader1.use();
    shader1.setInt("Texture", 0);
    shader1.setInt("Texture2", 1);
    auto LightPos = Lamp.GetWorldLocation();
    shader1.setVec3("lightPos", LightPos.x(), LightPos.y(), LightPos.z());


    Matrix4f ViewMatrix = Matrix4f::Identity();
    Matrix4f ProjectionMatrix = GetProjectionMatrix(45, HEIGHT / WIDTH, 0.1, 200);
    Matrix4f modelMatrix4f = Matrix4f::Identity();
    Matrix4f transMatrix = Matrix4f::Identity();

    //开启深度测试
    glEnable(GL_DEPTH_TEST);
//    glDisable(GL_CULL_FACE);
    while (!glfwWindowShouldClose(window)) {

        // input
        // -----
        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        ViewMatrix = camera.GetViewMatrix();
        ProjectionMatrix = GetProjectionMatrix(45, static_cast<float>( WIDTH ) / static_cast<float>( HEIGHT ), 0.1,
                                               200);

        // render container
        shader1.use();
        shader1.setFloat("mixValue", mixValue);
        shader1.setFloat("offset_u", offset_u);
        shader1.setFloat("offset_v", offset_v);
        shader1.setFloat("scaleValue", scaleValue);

        auto ViewPoint = camera.GetViewPoint();
        shader1.setVec3("viewPos", ViewPoint.x(), ViewPoint.y(), ViewPoint.z());

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture4);

        unsigned int transformLoc = glGetUniformLocation(shader1.ID, "transMatrix");
        unsigned int model = glGetUniformLocation(shader1.ID, "model");


        //计算变换矩阵
        box1.SetWorldRotation(angle_X, angle_Y, angle_Z);
        box1.SetWorldScale(modelScale, modelScale, modelScale);
        box1.SetWorldLocation(position[0], position[1], position[2]);
        modelMatrix4f = box1.GetModelMatrix4f();
        transMatrix = ProjectionMatrix * ViewMatrix * modelMatrix4f;
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, transMatrix.data());
        glUniformMatrix4fv(model, 1, GL_FALSE, modelMatrix4f.data());
        box1.Mesh->DrawElement();


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);


        modelMatrix4f = floor.GetModelMatrix4f();
        transMatrix = ProjectionMatrix * ViewMatrix * modelMatrix4f;
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, transMatrix.data());
        glUniformMatrix4fv(model, 1, GL_FALSE, modelMatrix4f.data());
        floor.Mesh->DrawElement();


        Lampshader.use();
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, transMatrix.data());
        modelMatrix4f = Lamp.GetModelMatrix4f();
        transMatrix = ProjectionMatrix * ViewMatrix * modelMatrix4f;
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, transMatrix.data());
        Lamp.Mesh->DrawElement();

        //划线模式和填充模式
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        //交换双缓冲内容,交换缓冲区并轮询输入输出（IO）事件（如按键按下 / 释放、鼠标移动等）。
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glDeleteBuffers(1, &VBO0);
    glDeleteTextures(1, &texture1);
    glDeleteTextures(1, &texture2);
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {

    static bool HasPress = false;
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS && !HasPress) {
        if (flag) {
            flag = false;
            GetProjectionMatrix = GetPerspectiveProjectionMatrix;
        } else {
            flag = true;
            GetProjectionMatrix = GetOrthographicProjectionMatrix;
        }
        HasPress = true;
    }
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
        angle_X -= 5.f;
        if (angle_X <= 0.0f) {
            angle_X = 360 + angle_Y;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        angle_Y -= 5.f;
        if (angle_Y <= 0.0f) {
            angle_Y = 360 + angle_Y;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        angle_Z -= 5.f;
        if (angle_Z <= 0.0f) {
            angle_Z = 360 + angle_Y;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS) {
        position[1] += 0.01;
    }
    if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS) {
        position[1] -= 0.01;
    }
    if (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS) {
        position[0] += 0.01;
    }
    if (glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS) {
        position[0] -= 0.01;
    }
    if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS) {
        position[2] += 0.01;
    }
    if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS) {
        position[2] -= 0.01;
    }


    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { ;
        camera.MoveForward(-0.1f);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.MoveForward(0.1f);
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

    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
        modelScale += 0.1;


    }
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
        modelScale -= 0.1;


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

unsigned int Load_Tex(const char *filename) {
    //纹理生成
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //tex读取
    int width, height, nrChannels;
    //分别获取图片高度宽度以及颜色通道数
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (data) {
        /**
     *texture内存解释:
     *target:意味着此函数影响当前绑定的2D纹理对象,而1D和3D不受影响;
     *level:生成mipmap级别
     *internalformat:gl存储纹理类型
     *width,height:不解释
     *border:gl的屎山遗留物，不用管
     *format:原图像格式
     *type:数据类型,此处为char(byte)数组
     *图像数据
     */
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        //生成mipmap(如果不需要则可省略);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(data);
        return texture;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    std::cout << "Failed to load texture" << std::endl;
    return 0;
}

unsigned int Load_Tex4f(const char *filename) {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    //tex读取
    int width, height, nrChannels;
    //分别获取图片高度宽度以及颜色通道数
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (data) {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
        return texture;
    }

    std::cout << "Failed to load texture" << std::endl;
    return 0;
}
