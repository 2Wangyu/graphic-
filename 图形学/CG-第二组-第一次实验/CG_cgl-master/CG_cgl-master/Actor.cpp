//
// Created by 分子轨道mo法 on 25-4-4.
//

#include "Actor.h"
#include "TransMatrix.h"


// World transform setters
void Actor::SetWorldLocation(const float &x, const float &y, const float &z) {
    WorldLocation << x, y, z;
}

void Actor::SetWorldRotation(const float &Yaw, const float &Pitch, const float &Roll) {
    WorldRotation << Yaw, Pitch, Roll;
}

void Actor::SetWorldScale(const float &x, const float &y, const float &z) {
    WorldScale << x, y, z;
}

// Relative transform setters
void Actor::SetRelativeLocation(const float &x, const float &y, const float &z) {
    RelativeLocation << x, y, z;
}

void Actor::SetRelativeRotation(const float &Yaw, const float &Pitch, const float &Roll) {
    RelativeRotation << Yaw, Pitch, Roll;
}

void Actor::SetRelativeScale(const float &x, const float &y, const float &z) {
    RelativeScale << x, y, z;
}

void Actor::AttachTo(Actor *parent) {
    Parent = parent;
}

Eigen::Vector3f Actor::GetWorldLocation() {
    if (!Parent)return WorldLocation;

    Eigen::Vector3f ParentLocation = Parent->GetWorldLocation();

    return ParentLocation + RelativeLocation;
}

Eigen::Vector3f Actor::GetWorldRotation() {
    if (!Parent)return WorldRotation;


    auto ParentRotation = Parent->WorldRotation;


    return ParentRotation + RelativeRotation;
}

Eigen::Vector3f Actor::GetWorldScale() {
    if (!Parent)return WorldScale;

    auto ParentScale = Parent->WorldScale;
    Eigen::Vector3f Scale;
    Scale.x() = RelativeScale.x() * ParentScale.x();
    Scale.y() = RelativeScale.y() * ParentScale.y();
    Scale.z() = RelativeScale.z() * ParentScale.z();

    return Scale;
}

Actor::Actor() : Parent(nullptr) {
    WorldLocation << 0.f, 0.f, 0.f;
    WorldRotation << 0.f, 0.f, 0.f;
    WorldScale << 1.f, 1.f, 1.f;
    //相对于父级的位置
    RelativeLocation << 0.f, 0.f, 0.f;
    RelativeRotation << 0.f, 0.f, 0.f;
    RelativeScale << 1.f, 1.f, 1.f;
    Mesh = std::make_unique<StaticMesh>();
}

Eigen::Matrix4f Actor::GetModelMatrix4f() {

    Eigen::Matrix4f TransMat = GetMoveMatrix(GetWorldLocation());
    Eigen::Matrix4f RotationMat = GetRotation(GetWorldRotation());
    Eigen::Matrix4f ScaleMat = GetScale(GetWorldScale());

    return TransMat * RotationMat * ScaleMat;
}





