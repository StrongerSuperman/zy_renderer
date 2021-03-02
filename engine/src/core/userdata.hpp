#pragma once

#include <glm/glm.hpp>

#include "camera.hpp"


class Userdata{
public:
    Userdata(){
        m_Camera = new Camera();
    }
    ~Userdata(){
        delete m_Camera;
    }

    Camera* GetCamera() const {return m_Camera;};
private:
    Camera* m_Camera;
};
