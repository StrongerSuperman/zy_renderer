#pragma once

#include <vector>
#include <algorithm>

#include <glm/glm.hpp>


class BlinnMaterial{
public:
    glm::vec3 diffuse;
    glm::vec3 specular;
    float alpha;
    float shininess;
    glm::vec3 normal;
    glm::vec3 emission;

    BlinnMaterial(void* fs_in, void* uniforms, int backface);
};
