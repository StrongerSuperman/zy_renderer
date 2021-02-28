#ifndef BLINN_MATERIAL_H
#define BLINN_MATERIAL_H

#include <vector>
#include <algorithm>

#include <glm/glm.hpp>


class BlinnMaterial{
public:
    float alpha;
    float shininess;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 normal;
    glm::vec3 emission;

    BlinnMaterial(void* fs_in, void* uniforms, int backface);
};

#endif //BLINN_MATERIAL_H