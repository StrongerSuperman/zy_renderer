#include <glm/glm.hpp>

#include "../../core/texture.hpp"
#include "blinn_shader.hpp"
#include "blinn_material.hpp"


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

    auto depth_position = _fs_in->depth_position;
    auto world_position = _fs_in->world_position;
    auto normal = _fs_in->normal;

    auto ambient_intensity = _uniforms->ambient_intensity;
    auto punctual_intensity = _uniforms->punctual_intensity;
    auto shadow_map = _uniforms->shadow_map;
    auto camera_pos = _uniforms->camera_pos;
    auto light_dir = _uniforms->light_dir;
    auto shadow_pass = _uniforms->shadow_pass;
    auto alpha_cutoff = _uniforms->alpha_cutoff;

    BlinnMaterial material(fs_in, uniforms, backface);
    auto alpha = material.has_opacity ? material.opacity[0] : 1;

    if (alpha_cutoff > 0 && alpha < alpha_cutoff) {
        *discard = 1;
        return glm::vec4(0, 0, 0, 0);
    } 

    if (shadow_pass) {
        return glm::vec4(0, 0, 0, 0);
    }
    else {
		auto _ambient = material.has_ambient ? material.ambient : glm::vec3(0, 0, 0);
		auto _emission = material.has_emission ? material.emission : glm::vec3(0,0,0);
        auto _diffuse = material.has_diffuse ? material.diffuse : glm::vec3(0,0,0);
		auto _normal = glm::normalize(material.has_normal ? material.normal : (material.has_height ? material.height : normal));
        auto _specular = material.has_specular ? material.specular : glm::vec3(0,0,0);
        auto _shininess = material.has_shininess ? material.shininess[0] : 32.0f;
        auto color = _emission;
        if (ambient_intensity > 0) {
            float intensity = ambient_intensity;
            color += _diffuse*intensity;
        }
        if (punctual_intensity > 0) {
            float n_dot_l = glm::dot(_normal, light_dir);
            bool is_in_shadow = false;
            if (shadow_map) {
                float u = (depth_position.x + 1) * 0.5f;
                float v = (depth_position.y + 1) * 0.5f;
                float d = (depth_position.z + 1) * 0.5f;
                float depth_bias = std::max(0.05f * (1 - n_dot_l), 0.005f);
                float current_depth = d - depth_bias;
                auto texcoord = glm::vec2(u, v);
                float closest_depth = shadow_map->Sample(texcoord).x;
                is_in_shadow = current_depth > closest_depth;
            }
            if (n_dot_l > 0 && !is_in_shadow) {
                if (!(_specular.x == 0 &&
                        _specular.y == 0 &&
                        _specular.z == 0)) {
                    auto view_dir = glm::normalize(camera_pos-world_position);
                    auto half_dir = glm::normalize(light_dir+view_dir);
                    float n_dot_h = glm::dot(_normal, half_dir);
                    if (n_dot_h > 0) {
                        float strength = pow(n_dot_h, _shininess);
                        _specular *= strength;
                    }
                }
                auto diffuse = _diffuse*n_dot_l;
                auto punctual = diffuse + _specular;
                color += punctual*punctual_intensity;
            }
        }
        return glm::vec4(color, alpha) / 255.0f;
    }
}
