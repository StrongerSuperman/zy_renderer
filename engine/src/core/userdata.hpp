#pragma once

#include <glm/glm.hpp>

#include "camera.hpp"


class Userdata{
public:
    Userdata(){
        m_Camera = new Camera();
        auto eye = glm::vec3(0,0,10);
        auto dir = glm::vec3(0,0,-1);
        auto fov = 45.0f;
        m_Camera->SetEyeAndDir(eye,dir);
        m_Camera->SetFov(fov);
    }
    ~Userdata(){
        delete m_Camera;
    }

    Camera* GetCamera() const {return m_Camera;};
private:
    Camera* m_Camera;
};
