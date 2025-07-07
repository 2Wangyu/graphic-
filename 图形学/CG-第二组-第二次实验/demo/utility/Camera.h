//
// Created by cgl on 25-4-2.
//

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

class Camera {

public:
    float TurnRate = 0.003f;
    float VelocityRate = 1.f;

    vec3 Position;
    vec3 Up;
    vec3 Lookat;

    float Yaw;
    float Pitch;


    float Zoom;

    Camera(vec3 position, vec3 up, vec3 lookat) : Position(position), Up(up), Lookat(lookat), Zoom(45.f) {

    }

    Camera() : Zoom(45.f) {};

    void MoveForward(float velocityRate);

    void MoveRight(float velocityRate);

    void MoveRise(float velocityRate);

    inline void ProcessMouseScroll(float yoffset) {
        Zoom -= (float) yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

    void TurnX(float angle);

    void TurnY(float angle);

    inline vec3 GetViewPoint() {
        return Position;
    }

    inline void SetTurnRate(const float &turnRate) {
        this->TurnRate = turnRate;
    }

    void SetVelocityRate(const float &velocityRate) {
        this->VelocityRate = velocityRate;
    }

    mat4 GetViewMatrix();
};


#endif //CAMERA_H
