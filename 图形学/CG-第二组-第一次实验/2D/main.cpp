#include <iostream>
#include "glframework/core.h"
#include "glframework/shader.h"
#include <string>
#include <assert.h>
#include "wrapper/checkError.h"
#include "application/Application.h"
#include "glframework/texture.h"
#include <glm/gtc/matrix_transform.hpp>

// ���ȫ�ֱ������Ʊ任����
int transformType = 0; // 0:��ת����, 1-6: ��ͬ�任
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
		case GLFW_KEY_1: transformType = 1; break; // ����
		case GLFW_KEY_2: transformType = 2; break; // ����
		case GLFW_KEY_3: transformType = 3; break; // �б�
		case GLFW_KEY_4: transformType = 4; break; // ƽ��
		case GLFW_KEY_5: transformType = 5; break; // ��ϱ任
		case GLFW_KEY_6: transformType = 6; break; // ��λ
		default: transformType = 0;        // ������ת
		}
	}
}

void doRotationTransform() {
	//����һ����ת��������z����ת45�Ƚ�
	//rotate����������������ת����
	//bug1:rotate����õ�һ��float���͵ĽǶȣ�c++��template
	//bug2:rotate�������ܵĲ��ǽǶȣ�degree�������յĻ��ȣ�radians��
	//ע��㣺radians����Ҳ��ģ�庯�����м�Ҫ����float�������ݣ���f��׺
	transform = glm::rotate(glm::mat4(1.0f),glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
}

//ƽ�Ʊ任
void doTranslationTransform() {
	transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.0f));
}

//���ű任
void doScaleTransform() {
	transform = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.5f, 1.0f));
}

// ����任����
void doReflectionTransform() {
	transform = glm::scale(glm::mat4(1.0f), glm::vec3(-1.0f, 1.0f, 1.0f));
}

//�б�任����
void doShearTransform() {
	glm::mat4 shearMat = glm::mat4(1.0f);
	shearMat[1][0] = 0.5f; // X�������
	transform = shearMat;
}

// �޸���ϱ任����
void doCompositeTransform() {
	//�Ƚ���ƽ�Ʊ任���������� x�᷽������ƽ�� 0.3 ����λ��������z����ת 45 �ȡ������ x��� y�᷽��������ţ���������Ϊ 0.7��
	glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(0.3f, 0.0f, 0.0f));
	glm::mat4 rot = glm::rotate(trans, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	transform = glm::scale(rot, glm::vec3(0.7f, 0.7f, 1.0f));
}

float angle = 0.0f;
void doRotation() {

	angle += 2.0f;
	//ÿһ֡���ᡰ���¡�����һ����ת����
	transform = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0, 0.0, 1.0));
}

void prepareVAO() {
	//1 ׼��positions colors
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

	//2 VBO����
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

	//3 EBO����
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//4 VAO����
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//5 ��vbo ebo ��������������Ϣ
	//5.1 ����λ������������Ϣ
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	//5.2 ������ɫ������������
	glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	//5.3 ����uv������������
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	//5.4 ����ebo����ǰ��vao
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
	//ִ��opengl�����������
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

	//�󶨵�ǰ��program
	shader->begin();
	shader->setInt("sampler", 0);
	shader->setMatrix4x4("transform", transform);

	//�󶨵�ǰ��vao
	GL_CALL(glBindVertexArray(vao));

	//��������ָ��
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

	//����opengl�ӿ��Լ�������ɫ
	GL_CALL(glViewport(0, 0, 800, 600));
	GL_CALL(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

	prepareShader();
	prepareVAO();
	prepareTexture();
	// ��ѭ���߼�
	while (app->update()) {
		switch (transformType) {
		case 1: doScaleTransform(); break;
		case 2: doReflectionTransform(); break;
		case 3: doShearTransform(); break;
		case 4: doTranslationTransform(); break;
		case 5: doCompositeTransform(); break;
		case 6: transform = glm::mat4(1.0f); break;
		default: doRotation(); // Ĭ�ϳ�����ת
		}
		render();
	}

	app->destroy();

	return 0;
}