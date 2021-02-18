#include <glm/glm.hpp>

#include "blinn_shader.hpp"
#include "blinn_material.hpp"


BlinnMaterial::BlinnMaterial(void* fs_in, void* uniforms, int backface){
    auto _fs_in = static_cast<BlinnFSIn*>(fs_in);
    auto _uniforms = static_cast<BlinnUniforms*>(uniforms);

    auto texcoord = _fs_in->texcoord;
    glm::vec3 _diffuse, _specular, _normal, _emission;
    float _alpha, _shininess;

    _diffuse = glm::vec3(_uniforms->basecolor);
    _alpha = _uniforms->basecolor.w;
    if (_uniforms->diffuse_map) {
        auto sample = _uniforms->diffuse_map->Sample(texcoord);
        _diffuse = _diffuse*glm::vec3(sample);
        _alpha *= sample.w;
    }

    _specular = glm::vec3(0, 0, 0);
    if (_uniforms->specular_map) {
        auto sample = _uniforms->specular_map->Sample(texcoord);
        _specular = glm::vec3(sample);
    }
    _shininess = _uniforms->shininess;

    _normal = glm::normalize(_fs_in->normal);
    if (backface) {
        _normal = -normal;
    }

    _emission = glm::vec3(0, 0, 0);
    if (_uniforms->emission_map) {
        auto sample = _uniforms->emission_map->Sample(texcoord);
        _emission = glm::vec3(sample);
    }

    this->diffuse = _diffuse;
    this->specular = _specular;
    this->alpha = _alpha;
    this->shininess = _shininess;
    this->normal = _normal;
    this->emission = _emission;
}
