#include <glm/glm.hpp>

#include "shading/blinn/blinn_material.hpp"
#include "shading/blinn/blinn_shader.hpp"


#define SET_TEXTURE_BLINN(name)                                     \
    auto _##name = glm::vec3(0, 0, 0);                              \
    has_##name = false;                                             \
    if (_uniforms->name##_map) {                                    \
        auto sample = _uniforms->name##_map->Sample(texcoord);      \
        _##name = glm::vec3(sample);                                \
        has_##name = true;                                          \
    }                                                               \
    this->##name = _##name;                                         \

BlinnMaterial::BlinnMaterial(void* fs_in, void* uniforms, int backface){
    auto _fs_in = static_cast<BlinnFSIn*>(fs_in);
    auto _uniforms = static_cast<BlinnUniforms*>(uniforms);
    auto texcoord = _fs_in->texcoord;

    SET_TEXTURE_BLINN(diffuse);
    SET_TEXTURE_BLINN(specular);
    SET_TEXTURE_BLINN(ambient);
    SET_TEXTURE_BLINN(emissive);
    SET_TEXTURE_BLINN(height);
    SET_TEXTURE_BLINN(normal);
    SET_TEXTURE_BLINN(shininess);
    SET_TEXTURE_BLINN(opacity);
    SET_TEXTURE_BLINN(displacement);
    SET_TEXTURE_BLINN(lightmap);
    SET_TEXTURE_BLINN(reflection);

    this->normal = backface > 0 ? -this->normal : this->normal;

    this->ka = _uniforms->ka;
    this->kd = _uniforms->kd;
    this->ks = _uniforms->ks;
}
