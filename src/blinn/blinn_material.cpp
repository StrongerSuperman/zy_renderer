#include <glm/glm.hpp>

#include "blinn_shader.hpp"
#include "blinn_material.hpp"


BlinnMaterial::BlinnMaterial(void* fs_in, void* uniforms, int backface){
    auto _fs_in = static_cast<BlinnFSIn*>(fs_in);
    auto _uniforms = static_cast<BlinnUniforms*>(uniforms);

    auto texcoord = _fs_in->texcoord;
    glm::vec3 diffuse, specular, normal, emission;
    float alpha, shininess;

    diffuse = glm::vec3(_uniforms->basecolor);
    alpha = _uniforms->basecolor.w;
    if (_uniforms->diffuse_map) {
        auto sample = _uniforms->diffuse_map->Sample(texcoord);
        diffuse = diffuse*glm::vec3(sample);
        alpha *= sample.w;
    }

    specular = glm::vec3(0, 0, 0);
    if (_uniforms->specular_map) {
        auto sample = _uniforms->specular_map->Sample(texcoord);
        specular = glm::vec3(sample);
    }
    shininess = _uniforms->shininess;

    normal = glm::normalize(_fs_in->normal);
    if (backface) {
        normal = -normal;
    }

    emission = glm::vec3(0, 0, 0);
    if (_uniforms->emission_map) {
        auto sample = _uniforms->emission_map->Sample(texcoord);
        emission = glm::vec3(sample);
    }

    this->diffuse = diffuse;
    this->specular = specular;
    this->alpha = alpha;
    this->shininess = shininess;
    this->normal = normal;
    this->emission = emission;
}
