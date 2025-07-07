//
// Created by 分子轨道mo法 on 25-4-29.
//

#ifndef CG_E2_SKYBOX_H
#define CG_E2_SKYBOX_H


#include "Shader.h"
#include "Actor.h"

class SkyBox : public Actor {


    unsigned int VAO;


public:
    SkyBox();

    void Draw(Shader &shader);

};


#endif //CG_E2_SKYBOX_H
