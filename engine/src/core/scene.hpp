#ifndef SCENE_H
#define SCENE_H

#include <glm/glm.hpp>

#include "framebuffer.hpp"
#include "perframe.hpp"
#include "texture.hpp"
#include "model.hpp"
#include "camera.hpp"


enum class SceneType{
    SCENE_TYPE_BLINN,
    SCENE_TYPE_PBR
};

class Scene{
public:
    std::string directory;
    SceneType type;
    glm::vec4 background;
    /* render quality */
    Usage render_quality;
    /* camera */
    Camera* camera;
    /* object */
    Model* skybox;
    std::vector<Model*> models;
    /* light intensity */
    float ambient_intensity;
    float punctual_intensity;
    /* shadow mapping */
    FrameBuffer* shadow_buffer;
    Texture* shadow_map;

    Scene(){
        this->skybox = nullptr;
        this->shadow_buffer = nullptr;
        this->shadow_map = nullptr;
        this->camera = new Camera();
    }
    virtual ~Scene() {
        delete this->skybox;
        for(size_t i = 0; i < this->models.size(); i++){
            delete this->models[i];
        }
        delete this->shadow_buffer;
        delete this->shadow_map;
        delete this->camera;
    };
    virtual void Update(Perframe *perframe)=0;
    virtual void Render(FrameBuffer *framebuffer, Perframe *perframe)=0;
    virtual Camera* GetCamera(){return this->camera;};

    void InitShadow(int shadow_width, int shadow_height) {
        this->shadow_buffer = new FrameBuffer(shadow_width, shadow_height);
        this->shadow_map = new Texture(shadow_width, shadow_height);
    };
};

#endif //SCENE_H
