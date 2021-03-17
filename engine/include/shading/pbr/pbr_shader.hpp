#ifndef PBR_SHADER_H
#define PBR_SHADER_H

#include <glm/glm.hpp>

#include "core/texture.hpp"
#include "core/shader.hpp"


struct PBRVSIn{
    glm::vec3 position;
    glm::vec2 texcoord;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec4 joint;
    glm::vec4 weight;
};

struct PBRFSIn{
    glm::vec3 world_position;
    glm::vec3 depth_position;
    glm::vec3 clip_position;
    glm::vec3 world_normal;
    glm::vec3 world_tangent;
    glm::vec3 world_bitangent;
    glm::vec2 texcoord;
};

struct PBRUniforms{
    /* scene intensity */
    float ambient_intensity;
    float punctual_intensity;
    /* light/model/camera mat */
    glm::vec3 light_pos;
    glm::vec3 camera_pos;  
    glm::mat4x4 model_mat;
    glm::mat3x3 normal_mat;
    glm::mat4x4 light_view_mat;
    glm::mat4x4 light_proj_mat;
    glm::mat4x4 camera_view_mat;
    glm::mat4x4 camera_proj_mat;
    /* shading param */
    glm::vec3 ka;
    glm::vec3 kd;
    glm::vec3 ks;
    /* shadow_map */
    Texture* shadow_map;
    /* surface textures */
    Texture *base_color_map;
    Texture *nomal_camera_map;
    Texture *emission_color_map;
    Texture *metalness_map;
    Texture *diffuse_roughness_map;
    Texture *ambient_occlusion_map;
    /* render controls */
    const float alpha_cutoff = 0.02f;
    int shadow_pass;
};

class PBRShader : public Shader{
public:
    glm::vec4 ExecuteVertexShader(void* vs_in, void* fs_in, void* uniforms) override;
    glm::vec4 ExecuteFragmentShader(void* fs_in, void* uniforms, int *discard, int backface) override;
};

#endif //PBR_SHADER_H
