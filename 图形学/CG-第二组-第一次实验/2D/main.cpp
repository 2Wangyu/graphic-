#include <iostream>
#include "glframework/core.h"
#include "glframework/shader.h"
#include <string>
#include <assert.h>
#include "wrapper/checkError.h"
#include "application/Application.h"
#include "glframework/texture.h"
#include <glm/gtc/matrix_transform.hpp>

// 添加全局变量控制变换类型
int transformType = 0; // 0:旋转动画, 1-6: 不同变换
glm::mat4 transform(1.0f);

GLuint vao;
Shader* shader = nullptr;
Texture* texture = nullptr;


void OnResize(int width, int height) {
	GL_CALL(glViewport(0, 0, width, height));
	std::cout << "OnResize" << std::endl;
}

void OnKey(int key, int action, int mods) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_1: transformType = 1; break; // 缩放
		case GLFW_KEY_2: transformType = 2; break; // 反射
		case GLFW_KEY_3: transformType = 3; break; // 切变
		case GLFW_KEY_4: transformType = 4; break; // 平移
		case GLFW_KEY_5: transformType = 5; break; // 组合变换
		case GLFW_KEY_6: transformType = 6; break; // 复位
		default: transformType = 0;        // 持续旋转
		}
	}
}

void doRotationTransform() {
	//构建一个旋转矩阵，绕着z轴旋转45度角
	//rotate函数：用于生成旋转矩阵
	//bug1:rotate必须得到一个float类型的角度，c++的template
	//bug2:rotate函数接受的不是角度（degree），接收的弧度（radians）
	//注意点：radians函数也是模板函数，切记要传入float类型数据，加f后缀
	transform = glm::rotate(glm::mat4(1.0f),glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
}

//平移变换
void doTranslationTransform() {
	transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.0f));
}

//缩放变换
void doScaleTransform() {
	transform = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.5f, 1.0f));
}

// 反射变换函数
void doReflectionTransform() {
	transform = glm::scale(glm::mat4(1.0f), glm::vec3(-1.0f, 1.0f, 1.0f));
}

//切变变换函数
void doShearTransform() {
	glm::mat4 shearMat = glm::mat4(1.0f);
	shearMat[1][0] = 0.5f; // X方向剪切
	transform = shearMat;
}

// 修改组合变换函数
void doCompositeTransform() {
	//先进行平移变换，将物体在 x轴方向向右平移 0.3 个单位。接着绕z轴旋转 45 度。最后在 x轴和 y轴方向进行缩放，缩放因子为 0.7。
	glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(0.3f, 0.0f, 0.0f));
	glm::mat4 rot = glm::rotate(trans, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	transform = glm::scale(rot, glm::vec3(0.7f, 0.7f, 1.0f));
}

float angle = 0.0f;
void doRotation() {

	angle += 2.0f;
	//每一帧都会“重新”构建一个旋转矩阵
	transform = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0, 0.0, 1.0));
}

void prepareVAO() {
	//1 准备positions colors
	float positions[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f,
	};

	float colors[] = {
		1.0f, 0.0f,0.0f,
		0.0f, 1.0f,0.0f,
		0.0f, 0.0f,1.0f,
	};

	float uvs[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.5f, 1.0f,
	};

	unsigned int indices[] = {
		0, 1, 2,
	};

	//2 VBO创建
	GLuint posVbo, colorVbo, uvVbo;
	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	glGenBuffers(1, &colorVbo);
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	glGenBuffers(1, &uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

	//3 EBO创建
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//4 VAO创建
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//5 绑定vbo ebo 加入属性描述信息
	//5.1 加入位置属性描述信息
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	//5.2 加入颜色属性描述数据
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	//5.3 加入uv属性描述数据
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	//5.4 加入ebo到当前的vao
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBindVertexArray(0);
}

void prepareShader() {
	shader = new Shader("assets/shaders/vertex.glsl","assets/shaders/fragment.glsl");
}

void prepareTexture() {
	texture = new Texture("assets/textures/1.jpg", 0);
}
void render() {
	//执行opengl画布清理操作
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

	//绑定当前的program
	shader->begin();
	shader->setInt("sampler", 0);
	shader->setMatrix4x4("transform", transform);

	//绑定当前的vao
	GL_CALL(glBindVertexArray(vao));

	//发出绘制指令
	GL_CALL(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0));
	GL_CALL(glBindVertexArray(0));

	shader->end();
}


int main() {
	if (!app->init(800, 600)) {
		return -1;
	}

	app->setResizeCallback(OnResize);
	app->setKeyBoardCallback(OnKey);

	//设置opengl视口以及清理颜色
	GL_CALL(glViewport(0, 0, 800, 600));
	GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

	prepareShader();
	prepareVAO();
	prepareTexture();
	// 主循环逻辑
	while (app->update()) {
		switch (transformType) {
		case 1: doScaleTransform(); break;
		case 2: doReflectionTransform(); break;
		case 3: doShearTransform(); break;
		case 4: doTranslationTransform(); break;
		case 5: doCompositeTransform(); break;
		case 6: transform = glm::mat4(1.0f); break;
		default: doRotation(); // 默认持续旋转
		}
		render();
	}

	app->destroy();

	return 0;
}