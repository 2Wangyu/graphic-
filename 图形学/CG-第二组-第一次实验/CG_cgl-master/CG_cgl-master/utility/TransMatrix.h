//
// Created by cgl on 25-4-2.
//

#ifndef TRANSMATRIX_H
#define TRANSMATRIX_H

#include "Eigen/Eigen"

#define ANGLE_TO_RADIAN *M_PI/180
using namespace Eigen;

//projection矩阵
//返回投影矩阵,fov 长宽比
inline Matrix4f GetPerspectiveProjectionMatrix(float fov, float aspect, float zNear, float zFar) {
    Matrix4f projection = Matrix4f::Zero();

    float tanHalfFov = tan(fov ANGLE_TO_RADIAN);
    float range = zFar - zNear;

    projection(0, 0) = 1.0f / (aspect * tanHalfFov);
    projection(1, 1) = 1.0f / tanHalfFov;
    projection(2, 2) = -1.0f / (zFar - zNear);
    projection(2, 3) = -zNear / (zFar - zNear);
    projection(3, 2) = -1.0f;

    return projection;
}

// 正交投影矩阵
// 返回正交投影矩阵, fov 长宽比
inline Matrix4f GetOrthographicProjectionMatrix(float fov, float aspect, float zNear, float zFar) {
    Matrix4f projection = Matrix4f::Zero();

    float tanHalfFov = tan(fov * M_PI / 360.0f);
    float height = 2.0f * tanHalfFov;
    float width = height * aspect;
    float range = -zFar + zNear;

    projection(0, 0) = 2.0f / width;
    projection(1, 1) = 2.0f / height;
    projection(2, 2) = -2.0f / range;
    projection(2, 3) = -(zFar + zNear) / range;
    projection(3, 3) = 1.0f;

    return projection;
}

//model矩阵
//平移
inline Matrix4f GetMoveMatrix(const Vector3f &position) {
    Matrix4f orthMS = Matrix4f::Identity();
    orthMS << 1, 0, 0, position[0],
            0, 1, 0, position[1],
            0, 0, 1, position[2],
            0, 0, 0, 1;
    return orthMS;
}

//返回对应角度的旋转矩阵


//返回对应角度的旋转矩阵
inline Matrix4f GetXRotationMatrix4f(float rotation_angle) {
    //创建单位矩阵
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity(); //Identity单位矩阵
    model <<
          1, 0, 0, 0,
            0, cos(rotation_angle ANGLE_TO_RADIAN), -sin(rotation_angle ANGLE_TO_RADIAN), 0,
            0, sin(rotation_angle ANGLE_TO_RADIAN), cos(rotation_angle ANGLE_TO_RADIAN), 0,
            0, 0, 0, 1;
    return model;
}

inline Matrix4f GetYRotationMatrix4f(float rotation_angle) {
    //创建单位矩阵
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity(); //Identity单位矩阵
    model <<
          cos(rotation_angle ANGLE_TO_RADIAN), 0, sin(rotation_angle ANGLE_TO_RADIAN), 0,
            0, 1, 0, 0,
            -sin(rotation_angle ANGLE_TO_RADIAN), 0, cos(rotation_angle ANGLE_TO_RADIAN), 0,

            0, 0, 0, 1;
    return model;
}

inline Matrix4f GetZRotationMatrix4f(float rotation_angle) {
    //创建单位矩阵
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity(); //Identity单位矩阵
    model <<
          cos(rotation_angle ANGLE_TO_RADIAN), -sin(rotation_angle ANGLE_TO_RADIAN), 0, 0,
            sin(rotation_angle ANGLE_TO_RADIAN), cos(rotation_angle ANGLE_TO_RADIAN), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1;
    return model;
}

inline Matrix4f GetRotation(Vector3f YawPitchRoll) {
    // 获取各轴旋转矩阵（Yaw绕Y轴，Pitch绕X轴，Roll绕Z轴）
    Matrix4f R_yaw = GetYRotationMatrix4f(YawPitchRoll.x());
    Matrix4f R_pitch = GetXRotationMatrix4f(YawPitchRoll.y());
    Matrix4f R_roll = GetZRotationMatrix4f(YawPitchRoll.z());

    // 组合顺序：Roll(Z) * Pitch(X) * Yaw(Y)
    return R_roll * R_pitch * R_yaw;
}

//对任意转轴实现的旋转,传入转轴、旋转角
inline Matrix4f GetRotation(Vector3f axis, float angle) {
    //创建单位矩阵
    Eigen::Matrix3f m = Eigen::Matrix3f::Identity(); //Identity单位矩阵

    Eigen::Matrix4f model = Eigen::Matrix4f::Identity(); //Identity单位矩阵

    //单位向量化
    float k = sqrt(pow(axis(0), 2) + pow(axis(1), 2) + pow(axis(2), 2));
    axis = axis / k;

    Eigen::Matrix3f Rk = Eigen::Matrix3f::Identity();
    Rk << 0, -axis(2), axis(1),
            axis(2), 0, -axis(0),
            -axis(1), -axis(1), 0;

    m = cos(angle ANGLE_TO_RADIAN) * Eigen::Matrix3f::Identity() + (1 - cos(angle ANGLE_TO_RADIAN)) * axis * axis.
            transpose() + sin(angle ANGLE_TO_RADIAN) * Rk;

    model << m(0, 0), m(0, 1), m(0, 2), 0,
            m(1, 0), m(1, 1), m(1, 2), 0,
            m(2, 0), m(2, 1), m(2, 2), 0,
            0, 0, 0, 1;

    return model;
}

inline Matrix4f GetScale(const Vector3f &Scale) {
    Matrix4f ScaleMat = Matrix4f::Identity();

    ScaleMat(0, 0) = Scale.x();
    ScaleMat(1, 1) = Scale.y();
    ScaleMat(2, 2) = Scale.z();

    return ScaleMat;
}

#endif //TRANSMATRIX_H
