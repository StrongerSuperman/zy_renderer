#ifndef MY_USERDATA_H
#define MY_USERDATA_H

#include "../core/userdata.hpp"


class MyUserdata : public Userdata{
public:
    MyUserdata();
    ~MyUserdata();
    void Tick() override;
    Scene* GetScene() override{return m_Scene;};

private:
    Scene* m_Scene;
};

#endif //MY_USERDATA_H
