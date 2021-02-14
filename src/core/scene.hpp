#pragma once

#include <glm/glm.hpp>

#include "framebuffer.hpp"
#include "perframe.hpp"
#include "texture.hpp"
#include "model.hpp"


class Scene{
public:
    glm::vec4 background;
    Model* skybox;
    std::vector<Model*> models;
    /* light intensity */
    float ambient_intensity;
    float punctual_intensity;
    /* shadow mapping */
    FrameBuffer* shadow_buffer;
    Texture* shadow_map;

    Scene();
    virtual ~Scene()=0;
    virtual void Update(Perframe *perframe)=0;
    virtual void Render(FrameBuffer *framebuffer, Perframe *perframe)=0;
    void InitShadow(int shadow_width, int shadow_height) {
        this->shadow_buffer = new FrameBuffer(shadow_width, shadow_height);
        this->shadow_map = new Texture(shadow_width, shadow_height);
    };
};
