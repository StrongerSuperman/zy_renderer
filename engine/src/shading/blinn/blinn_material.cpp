#include <glm/glm.hpp>

#include "shading/blinn/blinn_material.hpp"
#include "shading/blinn/blinn_shader.hpp"


BlinnMaterial::BlinnMaterial(void* fs_in, void* uniforms, int backface){
    auto _fs_in = static_cast<BlinnFSIn*>(fs_in);
    auto _uniforms = static_cast<BlinnUniforms*>(uniforms);
    auto texcoord = _fs_in->texcoord;

    auto _diffuse = glm::vec3(0, 0, 0);
    has_diffuse = false;
    if (_uniforms->diffuse_map) {
        auto sample = _uniforms->diffuse_map->Sample(texcoord);
        _diffuse = glm::vec3(sample);
        has_diffuse = true;
    }

    auto _specular = glm::vec3(0, 0, 0);
    has_specular = false;
    if (_uniforms->specular_map) {
        auto sample = _uniforms->specular_map->Sample(texcoord);
        _specular = glm::vec3(sample);
        has_specular = true;
    }

    auto _ambient = glm::vec3(0, 0, 0);
    has_ambient = false;
    if (_uniforms->ambient_map) {
        auto sample = _uniforms->ambient_map->Sample(texcoord);
        _ambient = glm::vec3(sample);
        has_ambient = true;
    }

    auto _emission = glm::vec3(0, 0, 0);
    has_emission = false;
    if (_uniforms->emission_map) {
        auto sample = _uniforms->emission_map->Sample(texcoord);
        _emission = glm::vec3(sample);
        has_emission = true;
    }

    auto _height = glm::vec3(0, 0, 0);
    has_height = false;
    if (_uniforms->height_map) {
        auto sample = _uniforms->height_map->Sample(texcoord);
        _height = glm::vec3(sample);
        has_height = true;
    }

    auto _normal = glm::vec3(0, 0, 0);
    has_normal = false;
    if (_uniforms->normal_map) {
        auto sample = _uniforms->normal_map->Sample(texcoord);
        _normal = glm::vec3(sample);
        has_normal = true;
    }

    auto _shininess = glm::vec3(0, 0, 0);
    has_shininess = false;
    if (_uniforms->shininess_map) {
        auto sample = _uniforms->shininess_map->Sample(texcoord);
        _shininess = glm::vec3(sample);
        has_shininess = true;
    }

    auto _opacity = glm::vec3(0, 0, 0);
    has_opacity = false;
    if (_uniforms->opacity_map) {
        auto sample = _uniforms->opacity_map->Sample(texcoord);
        _opacity = glm::vec3(sample);
        has_opacity = true;
    }

    auto _displacement = glm::vec3(0, 0, 0);
    has_displacement = false;
    if (_uniforms->displacement_map) {
        auto sample = _uniforms->displacement_map->Sample(texcoord);
        _displacement = glm::vec3(sample);
        has_displacement = true;
    }

    auto _lightmap = glm::vec3(0, 0, 0);
    has_lightmap = false;
    if (_uniforms->lightmap_map) {
        auto sample = _uniforms->lightmap_map->Sample(texcoord);
        _lightmap = glm::vec3(sample);
        has_lightmap = true;
    }

    auto _reflection = glm::vec3(0, 0, 0);
    has_reflection = false;
    if (_uniforms->reflection_map) {
        auto sample = _uniforms->reflection_map->Sample(texcoord);
        _reflection = glm::vec3(sample);
        has_reflection = true;
    }

    this->diffuse = _diffuse;
    this->specular = _specular;
    this->ambient = _ambient;
    this->emission = _emission;
    this->height = _height;
    this->normal = backface > 0 ? -_normal : _normal;
    this->shininess = _shininess;
    this->opacity = _opacity;
    this->displacement = _displacement;
    this->lightmap = _lightmap;
    this->reflection = _reflection;
}
