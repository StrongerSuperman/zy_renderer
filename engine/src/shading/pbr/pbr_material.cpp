#include <glm/glm.hpp>

#include "shading/pbr/pbr_shader.hpp"
#include "shading/pbr/pbr_material.hpp"


PBRMaterial::PBRMaterial(void* fs_in, void* uniforms, int backface){
    auto _fs_in = static_cast<PBRFSIn*>(fs_in);
    auto _uniforms = static_cast<PBRUniforms*>(uniforms);
    auto texcoord = _fs_in->texcoord;

    auto _base_color = glm::vec3(0, 0, 0);
    has_base_color = false;
    if (_uniforms->base_color_map) {
        auto sample = _uniforms->base_color_map->Sample(texcoord);
        _base_color = glm::vec3(sample);
        has_base_color = true;
    }

    auto _nomal_camera = glm::vec3(0, 0, 0);
    has_nomal_camera = false;
    if (_uniforms->nomal_camera_map) {
        auto sample = _uniforms->nomal_camera_map->Sample(texcoord);
        _nomal_camera = glm::vec3(sample);
        has_nomal_camera = true;
    }

    auto _emission_color = glm::vec3(0, 0, 0);
    has_emission_color = false;
    if (_uniforms->emission_color_map) {
        auto sample = _uniforms->emission_color_map->Sample(texcoord);
        _emission_color = glm::vec3(sample);
        has_emission_color = true;
    }

    auto _metalness = glm::vec3(0, 0, 0);
    has_metalness = false;
    if (_uniforms->metalness_map) {
        auto sample = _uniforms->metalness_map->Sample(texcoord);
        _metalness = glm::vec3(sample);
        has_metalness = true;
    }

    auto _diffuse_roughness = glm::vec3(0, 0, 0);
    has_diffuse_roughness = false;
    if (_uniforms->diffuse_roughness_map) {
        auto sample = _uniforms->diffuse_roughness_map->Sample(texcoord);
        _diffuse_roughness = glm::vec3(sample);
        has_diffuse_roughness = true;
    }

    auto _ambient_occlusion = glm::vec3(0, 0, 0);
    has_ambient_occlusion = false;
    if (_uniforms->ambient_occlusion_map) {
        auto sample = _uniforms->ambient_occlusion_map->Sample(texcoord);
        _ambient_occlusion = glm::vec3(sample);
        has_ambient_occlusion = true;
    }

    this->base_color = _base_color;
    this->nomal_camera = backface > 0 ? -_nomal_camera : _nomal_camera;
    this->emission_color = _emission_color;
    this->metalness = _metalness;
    this->diffuse_roughness = _diffuse_roughness;
    this->ambient_occlusion = _ambient_occlusion;
}
