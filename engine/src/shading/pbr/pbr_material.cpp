#include <glm/glm.hpp>

#include "pbr_shader.hpp"
#include "pbr_material.hpp"


PBRMaterial::PBRMaterial(void* fs_in, void* uniforms, int backface){
    auto _fs_in = static_cast<BlinnFSIn*>(fs_in);
    auto _uniforms = static_cast<BlinnUniforms*>(uniforms);
    auto texcoord = _fs_in->texcoord;

    auto _base_color = glm::vec3(0, 0, 0);
    has_base_color = false;
    if (_uniforms->diffuse_map) {
        auto sample = _uniforms->diffuse_map->Sample(texcoord);
        _base_color = glm::vec3(sample);
        has_base_color = true;
    }

    auto _nomal_camera = glm::vec3(0, 0, 0);
    has_nomal_camera = false;
    if (_uniforms->specular_map) {
        auto sample = _uniforms->specular_map->Sample(texcoord);
        _nomal_camera = glm::vec3(sample);
        has_nomal_camera = true;
    }

    auto _emission_color = glm::vec3(0, 0, 0);
    has_emission_color = false;
    if (_uniforms->ambient_map) {
        auto sample = _uniforms->ambient_map->Sample(texcoord);
        _emission_color = glm::vec3(sample);
        has_emission_color = true;
    }

    auto _metalness = glm::vec3(0, 0, 0);
    has_metalness = false;
    if (_uniforms->emission_map) {
        auto sample = _uniforms->emission_map->Sample(texcoord);
        _metalness = glm::vec3(sample);
        has_metalness = true;
    }

    auto _diffuse_roughness = glm::vec3(0, 0, 0);
    has_diffuse_roughness = false;
    if (_uniforms->height_map) {
        auto sample = _uniforms->height_map->Sample(texcoord);
        _diffuse_roughness = glm::vec3(sample);
        has_diffuse_roughness = true;
    }

    auto _ambient_occlusion = glm::vec3(0, 0, 0);
    has_ambient_occlusion = false;
    if (_uniforms->normal_map) {
        auto sample = _uniforms->normal_map->Sample(texcoord);
        _ambient_occlusion = glm::vec3(sample);
        has_ambient_occlusion = true;
    }

    this->base_color = _diffuse;
    this->nomal_camera = _specular;
    this->emission_color = _ambient;
    this->metalness = _emission;
    this->diffuse_roughness = _height;
    this->ambient_occlusion = backface > 0 ? -_normal : _normal;
}
