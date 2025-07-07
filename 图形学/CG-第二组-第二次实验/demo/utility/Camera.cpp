//
// Created by cgl on 25-4-2.
//

#include "Camera.h"


void Camera::MoveForward(float velocityRate) {
//    auto ForwardDirection = glm::normalize(vec3(Lookat.x, Lookat.y, 0.f));
    Position += velocityRate * Lookat;
}

void Camera::MoveRight(float velocityRate) {
    // 提取 Lookat 和 Up 的前三个元素并归一化
    glm::vec3 z = glm::normalize(glm::cross(glm::normalize(Lookat), glm::normalize(Up)));
    Position += velocityRate * z;
}

void Camera::MoveRise(float velocityRate) {
//    Position += velocityRate * Up;
    Position.y += velocityRate;
}

void Camera::TurnX(float angle) {
// 绕Y轴旋转（偏航）
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), -TurnRate * angle, glm::vec3(0.0f, 1.0f, 0.0f));
    Lookat = glm::vec3(rotation * glm::vec4(Lookat, 0.0f));
    // 保持Up向量正交
    glm::vec3 right = glm::cross(Lookat, Up);
    Up = glm::cross(right, Lookat);
    Up = glm::normalize(Up);


}

void Camera::TurnY(float angle) {

    glm::vec3 right = glm::cross(glm::normalize(Lookat), glm::normalize(Up));
    right.y = 0.0f;
    right = glm::normalize(right);
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), -TurnRate * angle, right);
    Lookat = glm::vec3(rotation * glm::vec4(Lookat, 0.0f));
    // 更新Up向量保持正交
    Up = glm::cross(right, Lookat);
    Up = glm::normalize(Up);
}


mat4 Camera::GetViewMatrix() {

    return glm::lookAt(Position, Position + Lookat, Up);
}


