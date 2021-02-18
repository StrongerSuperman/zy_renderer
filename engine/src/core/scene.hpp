#ifndef SCENE_H
#define SCENE_H

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

    virtual ~Scene() {
        delete this->skybox;
        for(size_t i = 0; i < this->models.size(); i++){
            delete this->models[i];
        }
        delete this->shadow_buffer;
        delete this->shadow_map;
    };
    virtual void Update(Perframe *perframe) {(void)(perframe);};
    virtual void Render(FrameBuffer *framebuffer, Perframe *perframe) {
        (void)(framebuffer);
        (void)(perframe);
    };

    void InitShadow(int shadow_width, int shadow_height) {
        this->shadow_buffer = new FrameBuffer(shadow_width, shadow_height);
        this->shadow_map = new Texture(shadow_width, shadow_height);
    };
};

#endif //SCENE_H
