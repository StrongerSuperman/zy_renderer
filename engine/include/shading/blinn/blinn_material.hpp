#ifndef BLINN_MATERIAL_H
#define BLINN_MATERIAL_H

#include <glm/glm.hpp>


class BlinnMaterial{
public:
    bool has_diffuse;
    bool has_specular;
    bool has_ambient;
    bool has_emission;
    bool has_height;
    bool has_normal;
    bool has_shininess;
    bool has_opacity;
    bool has_displacement;
    bool has_lightmap;
    bool has_reflection;

    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 ambient;
    glm::vec3 emission;
    glm::vec3 height;
    glm::vec3 normal;
    glm::vec3 shininess;
    glm::vec3 opacity;
    glm::vec3 displacement;
    glm::vec3 lightmap;
    glm::vec3 reflection;

    glm::vec3 ka;
    glm::vec3 kd;
    glm::vec3 ks;

    BlinnMaterial(void* fs_in, void* uniforms, int backface);
};

#endif //BLINN_MATERIAL_H
