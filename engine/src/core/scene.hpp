#ifndef SCENE_H
#define SCENE_H

#include <glm/glm.hpp>

#include "framebuffer.hpp"
#include "texture.hpp"
#include "perframe.hpp"
#include "camera.hpp"
#include "model.hpp"


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
    /* object */
    Model* skybox;
    std::vector<Model*> models;
    /* light intensity */
    float ambient_intensity;
    float punctual_intensity;
    /* shadow mapping */
    FrameBuffer* shadow_buffer;
    Texture* shadow_map;

    Scene();
    virtual ~Scene();
    virtual void Update()=0;
    virtual void Render()=0;

    Camera* GetCamera(){return this->m_Camera;};
    Perframe* GetPerframe(){return m_Perframe;};
    void InitShadow(int shadow_width, int shadow_height);

private:
    Camera* m_Camera;
    Perframe* m_Perframe;
};

#endif //SCENE_H
