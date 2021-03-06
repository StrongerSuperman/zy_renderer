#include <glm/glm.hpp>

#include "scene.hpp"
#include "userdata.hpp"
#include "framebuffer.hpp"
#include "texture.hpp"


Scene::Scene(){
    this->skybox = nullptr;
    this->shadow_buffer = nullptr;
    this->shadow_map = nullptr;
    this->m_Camera = new Camera();
    this->m_Perframe = new Perframe();
}

Scene::~Scene() {
    delete this->skybox;
    for(size_t i = 0; i < this->models.size(); i++){
        delete this->models[i];
    }
    delete this->shadow_buffer;
    delete this->shadow_map;
    delete this->m_Camera;
    delete this->m_Perframe;
}

void Scene::InitShadow(int shadow_width, int shadow_height) {
    this->shadow_buffer = new FrameBuffer(shadow_width, shadow_height);
    this->shadow_map = new Texture(shadow_width, shadow_height);
}

