#include <stdlib.h>

#include "shading/blinn/blinn_model.hpp"
#include "shading/blinn/blinn_program.hpp"
#include "shading/blinn/blinn_shader.hpp"
#include "core/framebuffer.hpp"
#include "core/mesh.hpp"
#include "core/texture.hpp"
#include "utility/math_tool.hpp"



BlinnModel::BlinnModel(Scene* scene, Mesh* mesh, const glm::mat4x4& transform):
        Model(scene, mesh, transform, new BlinnProgram()){
    auto uniforms = static_cast<BlinnUniforms*>(this->program->uniforms);

    std::vector<std::string> texture_names = {
        "diffuse", "specular", "ambient",
        "emissive", "height", "normal",
        "shininess", "opacity", "displacement",
        "lightmap", "reflection"
    };

    auto diffuse_textures = this->mesh->GetTexture(texture_names[0]);
    auto specular_textures = this->mesh->GetTexture(texture_names[1]);
    auto ambient_textures = this->mesh->GetTexture(texture_names[2]);
    auto emissive_textures = this->mesh->GetTexture(texture_names[3]);
    auto height_textures = this->mesh->GetTexture(texture_names[4]);
    auto normal_textures = this->mesh->GetTexture(texture_names[5]);
    auto shininess_textures = this->mesh->GetTexture(texture_names[6]);
    auto opacity_textures = this->mesh->GetTexture(texture_names[7]);
    auto displacement_textures = this->mesh->GetTexture(texture_names[8]);
    auto lightmap_textures = this->mesh->GetTexture(texture_names[9]);
    auto reflection_textures = this->mesh->GetTexture(texture_names[10]);

    if (diffuse_textures->size() > 0){
        uniforms->diffuse_map = (*diffuse_textures)[0];
    }
    else{
        uniforms->diffuse_map = nullptr;
    }

    if (specular_textures->size() > 0){
        uniforms->specular_map = (*specular_textures)[0];
    }
    else{
        uniforms->specular_map = nullptr;
    }

    if (ambient_textures->size() > 0){
        uniforms->ambient_map = (*ambient_textures)[0];
    }
    else{
        uniforms->ambient_map = nullptr;
    }

    if (emissive_textures->size() > 0){
        uniforms->emission_map = (*emissive_textures)[0];
    }
    else{
        uniforms->emission_map = nullptr;
    }

    if (height_textures->size() > 0){
        uniforms->height_map = (*height_textures)[0];
    }
    else{
        uniforms->height_map = nullptr;
    }

    if (normal_textures->size() > 0){
        uniforms->normal_map = (*normal_textures)[0];
    }
    else{
        uniforms->normal_map = nullptr;
    }

    if (shininess_textures->size() > 0){
        uniforms->shininess_map = (*shininess_textures)[0];
    }
    else{
        uniforms->shininess_map = nullptr;
    }

    if (opacity_textures->size() > 0){
        uniforms->opacity_map = (*opacity_textures)[0];
    }
    else{
        uniforms->opacity_map = nullptr;
    }

    if (displacement_textures->size() > 0){
        uniforms->displacement_map = (*displacement_textures)[0];
    }
    else{
        uniforms->displacement_map = nullptr;
    }

    if (lightmap_textures->size() > 0){
        uniforms->lightmap_map = (*lightmap_textures)[0];
    }
    else{
        uniforms->lightmap_map = nullptr;
    }

    if (reflection_textures->size() > 0){
        uniforms->reflection_map = (*reflection_textures)[0];
    }
    else{
        uniforms->reflection_map = nullptr;
    }
}

void BlinnModel::Update(){
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
    auto indices = this->mesh->GetIndices();
    auto uniforms = static_cast<BlinnUniforms*>(this->program->uniforms);

    uniforms->shadow_pass = shadow_pass;
    for (int i = 0; i < num_faces; i++) {
        for (int j = 0; j < 3; j++) {
            auto index = (*indices)[i*3 + j];
            auto vertex = (*vertices)[index];
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
