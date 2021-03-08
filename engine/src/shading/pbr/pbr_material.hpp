#ifndef PBR_MATERIAL_H
#define PBR_MATERIAL_H

#include <glm/glm.hpp>


class PBRMaterial{
public:
    bool has_base_color;
    bool has_nomal_camera;
    bool has_emission_color;
    bool has_metalness;
    bool has_diffuse_roughness;
    bool has_ambient_occlusion;

    glm::vec3 base_color;
    glm::vec3 nomal_camera;
    glm::vec3 emission_color;
    glm::vec3 metalness;
    glm::vec3 diffuse_roughness;
    glm::vec3 ambient_occlusion;

    PBRMaterial(void* fs_in, void* uniforms, int backface);
};

#endif //PBR_MATERIAL_H
