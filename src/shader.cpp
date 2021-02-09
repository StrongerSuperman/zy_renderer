#include <glm/glm.hpp>

#include "shader.hpp"
#include "texture.hpp"


glm::vec4 ExecuteBlinnVertexShader(BlinnVSIn *vs_in, BlinnFSIn *fs_in, BlinnUniforms *uniforms) {
    auto inv_model_mat = glm::inverse(uniforms->model_mat);
    fs_in->normal = glm::vec3(glm::transpose(inv_model_mat)*glm::vec4(vs_in->normal, 1.0));
    auto mvp = uniforms->proj_mat*uniforms->view_mat*uniforms->model_mat ;
    return mvp*glm::vec4(vs_in->position, 1.0);
}

glm::vec4 ExecuteBlinnFragmentShader(BlinnFSIn *fs_in, BlinnUniforms *uniforms, BlinnMaterial *material, int *discard) {
    if (uniforms->alpha_cutoff > 0 && material->alpha < uniforms->alpha_cutoff) {
        *discard = 1;
        return glm::vec4(0, 0, 0, 0);
    } else {
        auto color = material->emission;
        if (uniforms->ambient_intensity > 0) {
            auto ambient = material->diffuse;
            float intensity = uniforms->ambient_intensity;
            color += ambient*intensity;
        }
        if (uniforms->punctual_intensity > 0) {
            auto light_dir = -uniforms->light_dir;
            float n_dot_l = glm::dot(material->normal, light_dir);
            bool is_in_shadow = false;
            if (uniforms->shadow_map) {
                float u = (fs_in->depth_position.x + 1) * 0.5f;
                float v = (fs_in->depth_position.y + 1) * 0.5f;
                float d = (fs_in->depth_position.z + 1) * 0.5f;
                float depth_bias = std::max(0.05f * (1 - n_dot_l), 0.005f);
                float current_depth = d - depth_bias;
                auto texcoord = glm::vec2(u, v);
                float closest_depth = uniforms->shadow_map->Sample(texcoord).x;
                is_in_shadow = current_depth > closest_depth;
            }
            if (n_dot_l > 0 && !is_in_shadow) {
                glm::vec3 specular = glm::vec3(0, 0, 0);
                if (!(material->specular.x == 0 &&
                        material->specular.y == 0 &&
                        material->specular.z == 0)) {
                    auto view_dir = glm::normalize(uniforms->camera_pos-fs_in->world_position);
                    auto half_dir = glm::normalize(light_dir+view_dir);
                    float n_dot_h = glm::dot(material->normal, half_dir);
                    if (n_dot_h > 0) {
                        float strength = (float)pow(n_dot_h, material->shininess);
                        specular *= strength;
                    }
                }
                auto diffuse = material->diffuse*n_dot_l;
                auto punctual = diffuse + specular;
                color += punctual*uniforms->punctual_intensity;
            }
        }
        return glm::vec4(color, material->alpha);
    }
}
