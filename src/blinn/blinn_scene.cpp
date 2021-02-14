#include <stdlib.h>
#include <algorithm>

#include "../core/framebuffer.hpp"
#include "../core/perframe.hpp"
#include "../core/texture.hpp"
#include "blinn_scene.hpp"


class Model;

BlinnScene::BlinnScene(){
}

BlinnScene::~BlinnScene(){
    delete this->skybox;
    for(size_t i = 0; i < this->models.size(); i++){
        delete this->models[i];
    }
    delete this->shadow_buffer;
    delete this->shadow_map;
}

void BlinnScene::Update(Perframe *perframe){
    if(this->skybox){
        this->skybox->Update(perframe);
    }
    for each(auto &model in this->models){
        model->Update(perframe);
    }
}

void BlinnScene::Render(FrameBuffer *framebuffer, Perframe *perframe){
    // shadow pass
    if (this->shadow_buffer && this->shadow_map) {
        this->sortModels(perframe->light_view_mat);
        this->shadow_buffer->ClearDepth(1);
        for each(auto &model in this->models){
            if(model->opaque){
                model->Draw(this->shadow_buffer, true);
            }
        }
        this->shadow_map->SetWithDepthBuffer(this->shadow_buffer);
    }

    // nomal pass
    this->sortModels(perframe->camera_view_mat);
    if(this->skybox){
        this->skybox->Draw(framebuffer);
    }
    for each(auto &model in this->models){
        model->Draw(framebuffer);
    }
}

static int compareModels(Model* model1, Model* model2) {
    if (model1->opaque && model2->opaque) {
        return model1->distance < model2->distance ? -1 : 1;
    } else if (model1->opaque && !model2->opaque) {
        return -1;
    } else if (!model1->opaque && model2->opaque) {
        return 1;
    } else {
        return model1->distance < model2->distance ? 1 : -1;
    }
}

void BlinnScene::sortModels(glm::mat4x4& view_matrix){
    if(this->models.size() <= 1){
        return;
    }
    for each(auto &model in this->models){
        auto center = model->mesh->GetCenter();
        auto local_pos = glm::vec4(center, 1);
        auto world_pos = model->transform*local_pos;
        auto view_pos = view_matrix*world_pos;
        model->distance = -view_pos.z;
    }
    std::sort(this->models.begin(), this->models.end(), compareModels);
}
