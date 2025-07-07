//
// Created by cgl on 25-4-2.
//

#include "Camera.h"

#include <iostream>

#include "TransMatrix.h"

using namespace Eigen;

void Camera::MoveForward(float velocityRate) {
    Position += velocityRate * Lookat;
}

void Camera::MoveRight(float velocityRate) {
    // 提取 Lookat 和 Up 的前三个元素并归一化
    // 提取 Lookat 和 Up 的前三个元素并归一化
    Eigen::Vector3f z = Lookat.normalized().cross(Up.normalized());
    Position += velocityRate * z;
}

void Camera::MoveRise(float velocityRate) {
    Position += velocityRate * Up;

}

void Camera::TurnX(float angle) {

    Eigen::Vector3f right = Lookat.normalized().cross(Up.normalized());
    right.y()=0;
    right.normalize();

    Matrix3f RotationMatrix = GetYRotationMatrix4f(TurnRate * angle).block(0, 0, 3, 3);

    Lookat = RotationMatrix * Lookat;
    Up = right.cross(Lookat).normalized();
}

void Camera::TurnY(float angle) {
// 绕相机的右轴（Right Vector）旋转（俯仰角 Pitch）
    Eigen::Vector3f right = Lookat.normalized().cross(Up.normalized());
    right.y()=0;
    right.normalize();
    Eigen::Matrix3f RotationMatrix = GetRotation(right, TurnRate * angle).block<3, 3>(0, 0);
    // 保持 Up 向量与 Lookat 垂直（可选，防止误差累积）
    Lookat = RotationMatrix * Lookat.normalized();
    Up = right.cross(Lookat).normalized();
}

Matrix4f Camera::GetViewMatrix() {

    Matrix4f Trans = Matrix4f::Identity();
    Trans << 1, 0, 0, -Position.x(),
            0, 1, 0, -Position.y(),
            0, 0, 1, -Position.z(),
            0, 0, 0, 1;

    // 提取 Lookat 和 Up 的前三个元素并归一化
    Eigen::Vector3f g = Lookat.normalized();
    Eigen::Vector3f t = Up.normalized();
    Eigen::Vector3f z = g.cross(t);

    // 构建旋转矩阵
    Eigen::Matrix4f rotation = Eigen::Matrix4f::Identity();
    rotation << z.x(), z.y(), z.z(), 0,
            t.x(), t.y(), t.z(), 0,
            g.x(), g.y(), g.z(), 0,
            0, 0, 0, 1;
    // 视图矩阵是旋转矩阵和平移矩阵的乘积
    Eigen::Matrix4f view = rotation * Trans;
    return view;
}


