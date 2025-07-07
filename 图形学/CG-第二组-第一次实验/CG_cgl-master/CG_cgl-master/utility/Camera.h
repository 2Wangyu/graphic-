//
// Created by cgl on 25-4-2.
//

#ifndef CAMERA_H
#define CAMERA_H

#include "Eigen/Eigen"

using namespace Eigen;

class Camera {
    float TurnRate = 0.2f;
    float VelocityRate = 1.f;
    Vector3f Position;
    Vector3f Up;
    Vector3f Lookat;

public:
    Camera(Vector3f position, Vector3f up, Vector3f lookat) : Position(position), Up(up), Lookat(lookat) {
    }

    Camera();

    void MoveForward(float velocityRate);

    void MoveRight(float velocityRate);

    void MoveRise(float velocityRate);

    void TurnX(float angle);

    void TurnY(float angle);

    inline Vector3f GetViewPoint() {
        return Position;
    }

    inline void SetTurnRate(const float &turnRate) {
        this->TurnRate = turnRate;
    }

    void SetVelocityRate(const float &velocityRate) {
        this->VelocityRate = velocityRate;
    }

    Matrix4f GetViewMatrix();
};


#endif //CAMERA_H
