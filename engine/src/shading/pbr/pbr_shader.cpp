#include <glm/glm.hpp>

#include "../../core/texture.hpp"
#include "pbr_shader.hpp"
#include "pbr_material.hpp"


glm::vec4 PBRShader::ExecuteVertexShader(void* vs_in, void* fs_in, void* uniforms) {
    auto _vs_in = static_cast<PBRVSIn*>(vs_in);
    auto _fs_in = static_cast<PBRFSIn*>(fs_in);
    auto _uniforms = static_cast<PBRUniforms*>(uniforms);

    auto position = _vs_in->position;
    auto texcoord = _vs_in->texcoord;
    auto normal = _vs_in->normal;
    auto tangent = _vs_in->tangent;

    auto model_mat = _uniforms->model_mat;
    auto normal_mat = _uniforms->normal_mat;
    auto camera_vp_mat = _uniforms->camera_vp_mat;
    auto light_vp_mat = _uniforms->light_vp_mat;

    auto world_position = model_mat*glm::vec4(position, 1);
    auto depth_position = light_vp_mat*world_position;
    auto clip_position = camera_vp_mat*world_position;

    if (_uniforms->shadow_pass) {
        _fs_in->texcoord = texcoord;
        return depth_position;
    }
    else {
        auto world_normal = glm::normalize(normal_mat*normal);
        if (_uniforms->nomal_camera_map) {
            auto world_tangent = glm::normalize(glm::vec3(model_mat*glm::vec4(tangent, 1)));
            auto world_bitangent = glm::cross(world_normal, world_tangent);
            _fs_in->world_tangent = world_tangent;
            _fs_in->world_bitangent = world_bitangent;
        }
        _fs_in->world_normal = world_normal;
        _fs_in->world_position = glm::vec3(world_position);
        _fs_in->depth_position = glm::vec3(depth_position);
        _fs_in->texcoord = texcoord;
        _fs_in->clip_position = clip_position;
        return clip_position;
    }
}

glm::vec4 PBRShader::ExecuteFragmentShader(void* fs_in, void* uniforms, int *discard, int backface) {
    auto _fs_in = static_cast<PBRFSIn*>(fs_in);
    auto _uniforms = static_cast<PBRUniforms*>(uniforms);

    return glm::vec4();
}
