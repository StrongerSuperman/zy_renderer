#ifndef USERDATA_H
#define USERDATA_H

#include "scene.hpp"

class Userdata{
public:
    Userdata(){};
    virtual ~Userdata(){};
    virtual void Tick()=0;
    virtual Scene* GetScene()=0;
};

#endif //USERDATA_H
