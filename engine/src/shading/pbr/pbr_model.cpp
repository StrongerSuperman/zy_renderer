#include <stdlib.h>

#include "shading/pbr/pbr_model.hpp"
#include "shading/pbr/pbr_program.hpp"
#include "shading/pbr/pbr_shader.hpp"
#include "core/framebuffer.hpp"
#include "core/perframe.hpp"
#include "core/mesh.hpp"
#include "core/texture.hpp"


#define BIND_TEXTURE_PBR(name)                                      \
    auto name##_textures = this->mesh->GetTexture(#name);           \
    if (name##_textures->size() > 0){                               \
        uniforms->name##_map = (*name##_textures)[0];               \
    }                                                               \
    else{                                                           \
        uniforms->name##_map = nullptr;                             \
    }                                                               \


PBRModel::PBRModel(Scene* scene, Mesh* mesh, glm::mat4x4& transform):
        Model(scene, mesh, transform, new PBRProgram()){
    auto uniforms = static_cast<PBRUniforms*>(this->program->uniforms);

    BIND_TEXTURE_PBR(base_color);
    BIND_TEXTURE_PBR(nomal_camera);
    BIND_TEXTURE_PBR(emission_color);
    BIND_TEXTURE_PBR(metalness);
    BIND_TEXTURE_PBR(diffuse_roughness);
    BIND_TEXTURE_PBR(ambient_occlusion);
}

void PBRModel::Update(){
    auto perframe = this->GetScene()->GetPerframe();

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
    uniforms->light_pos = perframe->light_pos;
    uniforms->camera_pos = perframe->camera_pos;
    uniforms->model_mat = model_matrix;
    uniforms->normal_mat = normal_matrix;
    uniforms->light_view_mat = perframe->light_view_mat;
    uniforms->light_proj_mat = perframe->light_proj_mat;
    uniforms->camera_view_mat = perframe->camera_view_mat;
    uniforms->camera_proj_mat = perframe->camera_proj_mat;
    // uniforms->joint_matrices = joint_matrices;
    // uniforms->joint_n_matrices = joint_n_matrices;
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
