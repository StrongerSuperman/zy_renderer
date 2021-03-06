#ifndef BLINN_SHADER_H
#define BLINN_SHADER_H

#include <vector>
#include <algorithm>

#include <glm/glm.hpp>

#include "../../core/texture.hpp"
#include "../../core/shader.hpp"


struct BlinnVSIn{
    glm::vec3 position;
    glm::vec2 texcoord;
    glm::vec3 normal;
    glm::vec4 joint;
    glm::vec4 weight;
};

struct BlinnFSIn{
    glm::vec3 world_position;
    glm::vec3 depth_position;
    glm::vec2 texcoord;
    glm::vec3 normal;
};

struct BlinnUniforms{
    /* scene intensity */
    float ambient_intensity;
    float punctual_intensity;
    /* light/model/camera mat */
    glm::vec3 light_dir;
    glm::vec3 camera_pos;  
    glm::mat4x4 model_mat;
    glm::mat3x3 normal_mat;
    glm::mat4x4 camera_vp_mat;
    glm::mat4x4 light_vp_mat;
    /* shadow_map */
    Texture* shadow_map;
    /* surface textures */
    Texture *diffuse_map;
    Texture *specular_map;
    Texture *ambient_map;
    Texture *emission_map;
    Texture *height_map;
    Texture *normal_map;
    Texture *shininess_map;
    Texture *opacity_map;
    Texture *displacement_map;
    Texture *lightmap_map;
    Texture *reflection_map;
    /* render controls */
    const float alpha_cutoff = 0.02f;
    int shadow_pass;
};

class BlinnShader : public Shader{
public:
    virtual glm::vec4 ExecuteVertexShader(void* vs_in, void* fs_in, void* uniforms) override;
    virtual glm::vec4 ExecuteFragmentShader(void* fs_in, void* uniforms, int *discard, int backface) override;
};

#endif //BLINN_SHADER_H
