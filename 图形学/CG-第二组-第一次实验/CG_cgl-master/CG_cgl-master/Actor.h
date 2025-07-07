//
// Created by 分子轨道mo法 on 25-4-4.
//

#ifndef CG_E1_1_ACTOR_H
#define CG_E1_1_ACTOR_H

#include <memory>
#include "Eigen/Eigen"
#include "StaticMesh.h"


class Actor {


    //父级
    Actor *Parent;
    //没有父级时,方位通过此属性确定
    Eigen::Vector3f WorldLocation;
    Eigen::Vector3f WorldRotation;
    Eigen::Vector3f WorldScale;
    //相对于父级的位置
    Eigen::Vector3f RelativeLocation;
    Eigen::Vector3f RelativeRotation;
    Eigen::Vector3f RelativeScale;

public:
    std::unique_ptr<StaticMesh> Mesh;

    Actor();

    void AttachTo(Actor *parent);

    // 委托构造函数，调用默认构造函数并更新 WorldLocation
    explicit Actor(const Eigen::Vector3f &Location) : Actor() {
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


    Eigen::Vector3f GetWorldLocation();

    Eigen::Vector3f GetWorldRotation();

    Eigen::Vector3f GetWorldScale();

    Eigen::Matrix4f GetModelMatrix4f();

};


#endif //CG_E1_1_ACTOR_H
