#ifndef BLINN_SHADER_H
#define BLINN_SHADER_H

#include <glm/glm.hpp>

#include "core/texture.hpp"
#include "core/shader.hpp"


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
    int shadow_pass;
    /* surface textures */
    Texture *diffuse_map;
    Texture *specular_map;
    Texture *ambient_map;
    Texture *emissive_map;
    Texture *height_map;
    Texture *normal_map;
    Texture *shininess_map;
    Texture *opacity_map;
    Texture *displacement_map;
    Texture *lightmap_map;
    Texture *reflection_map;
};

class BlinnShader : public Shader{
public:
    glm::vec4 ExecuteVertexShader(void* vs_in, void* fs_in, void* uniforms) override;
    glm::vec4 ExecuteFragmentShader(void* fs_in, void* uniforms, int *discard, int backface) override;
};

#endif //BLINN_SHADER_H
