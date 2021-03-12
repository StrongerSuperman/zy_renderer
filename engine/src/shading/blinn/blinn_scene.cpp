#include <stdlib.h>
#include <algorithm>
#include <string>

#include "shading/blinn/blinn_scene.hpp"
#include "core/context.hpp"
#include "core/model.hpp"
#include "utility/scene_loader.hpp"


BlinnScene::BlinnScene(const std::string& filename){
    this->type = SceneType::SCENE_TYPE_BLINN;
    this->render_quality = Usage::USAGE_LDR_COLOR;
    LoadScene(this, filename);
}

void BlinnScene::Update(){
    auto perframe = this->GetPerframe();

    if(this->skybox){
        this->skybox->Update();
    }
    for each(auto &model in this->models){
        model->Update();
    }
    this->ambient_intensity = perframe->ambient_intensity;
    this->punctual_intensity = perframe->punctual_intensity;
}

void BlinnScene::Render(){
    auto perframe = this->GetPerframe();
    auto framebuffer = Context::GetWindowsInstance()->GetFrameBuffer();

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
    framebuffer->ClearColor(this->background);
    framebuffer->ClearDepth(1);
    int num_opaques = 0;
    for each(auto &model in this->models){
        if (model->opaque) {
            num_opaques += 1;
        } else {
            break;
        }
    }
    for (int i = 0; i < num_opaques; i++) {
        models[i]->Draw(framebuffer);
    }
    if (this->skybox){
        this->skybox->Draw(framebuffer);
    }
    for (int i = num_opaques; i < this->models.size(); i++) {
        models[i]->Draw(framebuffer);
    }
}

static bool compareModels(Model* model1, Model* model2) {
    if (model1->opaque && model2->opaque) {
        return model1->distance > model2->distance;
    } else if (model1->opaque && !model2->opaque) {
        return false;
    } else if (!model1->opaque && model2->opaque) {
        return true;
    } else {
        return model1->distance < model2->distance;
    }
}

void BlinnScene::sortModels(const glm::mat4x4& view_matrix){
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
