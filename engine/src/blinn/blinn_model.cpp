#include <stdlib.h>

#include "../core/framebuffer.hpp"
#include "../core/perframe.hpp"
#include "../core/mesh.hpp"
#include "../core/texture.hpp"
#include "../utility/math_tool.hpp"
#include "blinn_model.hpp"
#include "blinn_program.hpp"
#include "blinn_shader.hpp"


BlinnModel::BlinnModel(Mesh* mesh, glm::mat4x4& transform):
    Model(mesh, transform, new BlinnProgram()){
}

void BlinnModel::Update(Perframe *perframe){
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

    auto uniforms = static_cast<BlinnUniforms*>(this->program->uniforms);
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

void BlinnModel::Draw(FrameBuffer *framebuffer, bool shadow_pass){
    int num_faces = this->mesh->GetFaceNum();
    auto vertices = this->mesh->GetVertices();
    auto uniforms = static_cast<BlinnUniforms*>(this->program->uniforms);

    uniforms->shadow_pass = shadow_pass;
    for (int i = 0; i < num_faces; i++) {
        for (int j = 0; j < 3; j++) {
            auto vertex = vertices[i * 3 + j];
            auto _vs_in = static_cast<BlinnVSIn*>(this->program->vs_in[j]);
            _vs_in->position = vertex.position;
            _vs_in->texcoord = vertex.texcoord;
            _vs_in->normal = vertex.normal;
            _vs_in->joint = vertex.joint;
            _vs_in->weight = vertex.weight;
        }
        framebuffer->DrawTriangle(this->program);
    }
}
