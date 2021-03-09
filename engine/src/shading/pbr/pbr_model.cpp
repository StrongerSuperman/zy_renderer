#include <stdlib.h>

#include "shading/pbr/pbr_model.hpp"
#include "shading/pbr/pbr_program.hpp"
#include "shading/pbr/pbr_shader.hpp"
#include "core/framebuffer.hpp"
#include "core/perframe.hpp"
#include "core/mesh.hpp"
#include "core/texture.hpp"
#include "utility/math_tool.hpp"


PBRModel::PBRModel(Scene* scene, Mesh* mesh, glm::mat4x4& transform):
        Model(scene, mesh, transform, new PBRProgram()){
    auto uniforms = static_cast<PBRUniforms*>(this->program->uniforms);

    std::vector<std::string> texture_names = {
        "base_color", "nomal_camera", "emission_color",
        "metalness", "diffuse_roughness", "ambient_occlusion"
    };

    auto base_color_textures = this->mesh->GetTexture(texture_names[0]);
    auto nomal_camera_textures = this->mesh->GetTexture(texture_names[1]);
    auto emission_color_textures = this->mesh->GetTexture(texture_names[2]);
    auto metalness_textures = this->mesh->GetTexture(texture_names[3]);
    auto diffuse_roughness_textures = this->mesh->GetTexture(texture_names[4]);
    auto ambient_occlusion_textures = this->mesh->GetTexture(texture_names[5]);

    if (base_color_textures->size() > 0){
        uniforms->base_color_map = (*base_color_textures)[0];
    }
    else{
        uniforms->base_color_map = nullptr;
    }

    if (nomal_camera_textures->size() > 0){
        uniforms->nomal_camera_map = (*nomal_camera_textures)[0];
    }
    else{
        uniforms->nomal_camera_map = nullptr;
    }

    if (emission_color_textures->size() > 0){
        uniforms->emission_color_map = (*emission_color_textures)[0];
    }
    else{
        uniforms->emission_color_map = nullptr;
    }

    if (metalness_textures->size() > 0){
        uniforms->metalness_map = (*metalness_textures)[0];
    }
    else{
        uniforms->metalness_map = nullptr;
    }

    if (diffuse_roughness_textures->size() > 0){
        uniforms->diffuse_roughness_map = (*diffuse_roughness_textures)[0];
    }
    else{
        uniforms->diffuse_roughness_map = nullptr;
    }

    if (ambient_occlusion_textures->size() > 0){
        uniforms->ambient_occlusion_map = (*ambient_occlusion_textures)[0];
    }
    else{
        uniforms->ambient_occlusion_map = nullptr;
    }
}

void PBRModel::Update(){
    auto perframe = this->GetScene()->GetPerframe();

    float ambient_intensity = perframe->ambient_intensity;
    float punctual_intensity = perframe->punctual_intensity;
    // auto skeleton = model->skeleton;
    auto model_matrix = this->transform;

    auto normal_matrix = glm::mat3x3(glm::transpose(glm::inverse((model_matrix))));
    // glm::mat4x4 *joint_matrices;
    // glm::mat3x3 *joint_n_matrices;

    // if (skeleton) {
    // if (0) {
    //     skeleton_update_joints(skeleton, perframe->frame_time);
    //     joint_matrices = skeleton_get_joint_matrices(skeleton);
    //     joint_n_matrices = skeleton_get_normal_matrices(skeleton);
    //     if (model->attached >= 0) {
    //         auto node_matrix = joint_matrices[model->attached];
    //         model_matrix = model_matrix*node_matrix;
    //         joint_matrices = NULL;
    //         joint_n_matrices = NULL;
    //     }
    // } else {
    //     joint_matrices = NULL;
    //     joint_n_matrices = NULL;
    // }

    auto uniforms = static_cast<PBRUniforms*>(this->program->uniforms);
    uniforms->light_dir = perframe->light_dir;
    uniforms->camera_pos = perframe->camera_pos;
    uniforms->model_mat = model_matrix;
    uniforms->normal_mat = normal_matrix;
    uniforms->light_vp_mat = perframe->light_proj_mat*perframe->light_view_mat;
    uniforms->camera_vp_mat = perframe->camera_proj_mat*perframe->camera_view_mat;
    // uniforms->joint_matrices = joint_matrices;
    // uniforms->joint_n_matrices = joint_n_matrices;
    uniforms->ambient_intensity = float_clamp(ambient_intensity, 0, 5);
    uniforms->punctual_intensity = float_clamp(punctual_intensity, 0, 5);
    uniforms->shadow_map = perframe->shadow_map;
}

void PBRModel::Draw(FrameBuffer *framebuffer, bool shadow_pass){
    int num_faces = this->mesh->GetFaceNum();
    auto vertices = this->mesh->GetVertices();
    auto indices = this->mesh->GetIndices();
    auto uniforms = static_cast<PBRUniforms*>(this->program->uniforms);

    uniforms->shadow_pass = shadow_pass;
    for (int i = 0; i < num_faces; i++) {
        for (int j = 0; j < 3; j++) {
            auto index = (*indices)[i*3 + j];
            auto vertex = (*vertices)[index];
            auto _vs_in = static_cast<PBRVSIn*>(this->program->vs_in[j]);
            _vs_in->position = vertex.position;
            _vs_in->texcoord = vertex.texcoord;
            _vs_in->normal = vertex.normal;
            _vs_in->tangent = vertex.tangent;
            _vs_in->joint = vertex.joint;
            _vs_in->weight = vertex.weight;
        }
        framebuffer->DrawTriangle(this->program);
    }
}
