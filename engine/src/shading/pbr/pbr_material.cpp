#include <glm/glm.hpp>

#include "shading/pbr/pbr_shader.hpp"
#include "shading/pbr/pbr_material.hpp"


#define SET_TEXTURE_PBR(name)                                               \
    auto _##name = glm::vec3(0, 0, 0);                                      \
    has_##name = false;                                                     \
    if (_uniforms->name##_map) {                                            \
        auto sample = _uniforms->name##_map->Sample(_fs_in->texcoord);      \
        _##name = glm::vec3(sample);                                        \
        has_##name = true;                                                  \
    }                                                                       \
    this->##name = _##name;                                                 \

PBRMaterial::PBRMaterial(void* fs_in, void* uniforms, int backface){
    auto _fs_in = static_cast<PBRFSIn*>(fs_in);
    auto _uniforms = static_cast<PBRUniforms*>(uniforms);

    SET_TEXTURE_PBR(base_color);
    SET_TEXTURE_PBR(nomal_camera);
    SET_TEXTURE_PBR(emission_color);
    SET_TEXTURE_PBR(metalness);
    SET_TEXTURE_PBR(diffuse_roughness);
    SET_TEXTURE_PBR(ambient_occlusion);

    this->nomal_camera = backface > 0 ? -this->nomal_camera : this->nomal_camera;
}
