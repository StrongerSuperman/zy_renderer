#ifndef USERDATA_H
#define USERDATA_H

#include "scene.hpp"

class Platform;

class Userdata{
public:
    Userdata(Platform* owner){this->m_Owner=owner;};
    virtual ~Userdata(){};
    virtual void Tick()=0;
    virtual Scene* GetScene() const=0;

    Platform* GetOwner(){return m_Owner;};

private:
    Platform* m_Owner;
};

#endif //USERDATA_H
