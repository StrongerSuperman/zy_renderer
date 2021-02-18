#include <glm/glm.hpp>

#include "../core/texture.hpp"
#include "blinn_shader.hpp"
#include "blinn_material.hpp"


BlinnShader::~BlinnShader(){
}

glm::vec4 BlinnShader::ExecuteVertexShader(void* vs_in, void* fs_in, void* uniforms) {
    auto _vs_in = static_cast<BlinnVSIn*>(vs_in);
    auto _fs_in = static_cast<BlinnFSIn*>(fs_in);
    auto _uniforms = static_cast<BlinnUniforms*>(uniforms);

    auto position = _vs_in->position;
    auto texcoord = _vs_in->texcoord;
    auto normal = _vs_in->normal;
    auto model_mat = _uniforms->model_mat;
    auto normal_mat = _uniforms->normal_mat;
    auto camera_vp_mat = _uniforms->camera_vp_mat;
    auto light_vp_mat = _uniforms->light_vp_mat;

    auto world_position = model_mat*glm::vec4(position, 1);
    auto depth_position = light_vp_mat*world_position;

    if (_uniforms->shadow_pass) {
        _fs_in->texcoord = texcoord;
        return depth_position;
    } 
    else {
        _fs_in->normal = glm::normalize(normal_mat*normal);
        _fs_in->world_position = glm::vec3(world_position);
        _fs_in->depth_position = glm::vec3(depth_position);
        _fs_in->texcoord = texcoord;
        auto clip_position = camera_vp_mat*world_position;
        return clip_position;
    }
}

glm::vec4 BlinnShader::ExecuteFragmentShader(void* fs_in, void* uniforms, int *discard, int backface) {
    auto _fs_in = static_cast<BlinnFSIn*>(fs_in);
    auto _uniforms = static_cast<BlinnUniforms*>(uniforms);

    if (_uniforms->shadow_pass) {
        if (_uniforms->alpha_cutoff > 0) {
            float alpha = _uniforms->basecolor.w;
            if (_uniforms->diffuse_map) {
                auto texcoord = _fs_in->texcoord;
                alpha *= _uniforms->diffuse_map->Sample(texcoord).w;
            }
            if (alpha < _uniforms->alpha_cutoff) {
                *discard = 1;
            }
        }
        return glm::vec4(0, 0, 0, 0);
    }
    else {
        BlinnMaterial material(fs_in, uniforms, backface);
        if (_uniforms->alpha_cutoff > 0 && material.alpha < _uniforms->alpha_cutoff) {
            *discard = 1;
            return glm::vec4(0, 0, 0, 0);
        } 
        else {
            auto color = material.emission;
            if (_uniforms->ambient_intensity > 0) {
                auto ambient = material.diffuse;
                float intensity = _uniforms->ambient_intensity;
                color += ambient*intensity;
            }
            if (_uniforms->punctual_intensity > 0) {
                auto light_dir = -_uniforms->light_dir;
                float n_dot_l = glm::dot(material.normal, light_dir);
                bool is_in_shadow = false;
                if (_uniforms->shadow_map) {
                    float u = (_fs_in->depth_position.x + 1) * 0.5f;
                    float v = (_fs_in->depth_position.y + 1) * 0.5f;
                    float d = (_fs_in->depth_position.z + 1) * 0.5f;
                    float depth_bias = std::max(0.05f * (1 - n_dot_l), 0.005f);
                    float current_depth = d - depth_bias;
                    auto texcoord = glm::vec2(u, v);
                    float closest_depth = _uniforms->shadow_map->Sample(texcoord).x;
                    is_in_shadow = current_depth > closest_depth;
                }
                if (n_dot_l > 0 && !is_in_shadow) {
                    glm::vec3 specular = glm::vec3(0, 0, 0);
                    if (!(material.specular.x == 0 &&
                            material.specular.y == 0 &&
                            material.specular.z == 0)) {
                        auto view_dir = _uniforms->camera_pos-_fs_in->world_position;
                        view_dir = glm::normalize(view_dir);
                        auto half_dir = glm::normalize(light_dir+view_dir);
                        float n_dot_h = glm::dot(material.normal, half_dir);
                        if (n_dot_h > 0) {
                            float strength = (float)pow(n_dot_h, material.shininess);
                            specular *= strength;
                        }
                    }
                    auto diffuse = material.diffuse*n_dot_l;
                    auto punctual = diffuse + specular;
                    color += punctual*_uniforms->punctual_intensity;
                }
            }
            return glm::vec4(color, material.alpha);
        }
    }
}
