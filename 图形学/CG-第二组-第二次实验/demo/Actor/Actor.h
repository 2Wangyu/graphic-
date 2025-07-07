//
// Created by 分子轨道mo法 on 25-4-4.
//

#ifndef CG_E1_1_ACTOR_H
#define CG_E1_1_ACTOR_H

#include <memory>

#include "Mesh.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Model.h"
#include "geometry/box.h"

using namespace glm;

namespace ActorInner {

    static unsigned int box_VAO;
    static Shader WrongShader;
    static bool HasInit;

    static Shader DrawWrongBox() {
        if (!HasInit) {
            BindBoxMesh(box_VAO, BOX_FACE::OUTWARD);
            WrongShader=Shader("../Shaders/VertexShader.glsl","..Shaders/WrongShader.glsl");
            HasInit = true;
        }
        WrongShader.use();
        glBindVertexArray(box_VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

    }

}


class Actor {


    //父级
    Actor *Parent;
    //没有父级时,方位通过此属性确定
    vec3 WorldLocation;
    vec3 WorldRotation;
    vec3 WorldScale;
    //相对于父级的位置
    vec3 RelativeLocation;
    vec3 RelativeRotation;
    vec3 RelativeScale;

public:
    //
    std::shared_ptr<Model> ActorMesh;

    Actor();

    explicit Actor(const std::shared_ptr<Model> &mesh);

    void SetMesh(const std::shared_ptr<Model> &mesh);

    void AttachTo(Actor *parent);

    // 委托构造函数，调用默认构造函数并更新 WorldLocation
    explicit Actor(const vec3 &Location) : Actor() {
        WorldLocation = Location;
    };

    //世界方位
    void SetWorldLocation(const float &x, const float &y, const float &z);

    void SetWorldRotation(const float &Yaw, const float &Pitch, const float &Roll);

    void SetWorldScale(const float &x, const float &y, const float &z);

    //相对方位
    void SetRelativeLocation(const float &x, const float &y, const float &z);

    void SetRelativeRotation(const float &Yaw, const float &Pitch, const float &Roll);

    void SetRelativeScale(const float &x, const float &y, const float &z);


    void SetWorldLocation(const vec3 &worldLocation);

    void SetWorldRotation(const vec3 &worldRotation);

    void SetWorldScale(const vec3 &worldScale);

    void SetRelativeLocation(const vec3 &relativeLocation);

    void SetRelativeRotation(const vec3 &relativeRotation);

    void SetRelativeScale(const vec3 &relativeScale);

    vec3 GetWorldLocation();

    vec3 GetWorldRotation();

    vec3 GetWorldScale();

    mat4 GetModelMatrix4f();

//    virtual void Draw(mat4 transMat4, mat4 modelMat4, Shader &shader);


    virtual void Draw(mat4 ViewMat4, mat4 ProjectionMat4, Shader &shader);
};


#endif //CG_E1_1_ACTOR_H
