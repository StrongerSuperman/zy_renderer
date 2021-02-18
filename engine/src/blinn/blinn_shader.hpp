#ifndef BLINN_SHADER_H
#define BLINN_SHADER_H

#include <vector>
#include <algorithm>

#include <glm/glm.hpp>

#include "../core/texture.hpp"
#include "../core/shader.hpp"


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
    glm::vec3 light_dir;
    glm::vec3 camera_pos;  
    glm::mat4x4 model_mat;
    glm::mat3x3 normal_mat;
    glm::mat4x4 camera_vp_mat;
    glm::mat4x4 light_vp_mat;
    float ambient_intensity;
    float punctual_intensity;
    Texture* shadow_map;
    /* surface parameters */
    glm::vec4 basecolor;
    float shininess;
    Texture *diffuse_map;
    Texture *specular_map;
    Texture *emission_map;
    /* render controls */
    float alpha_cutoff;
    int shadow_pass;
};

class BlinnShader : public Shader{
public:
    BlinnShader();
    virtual ~BlinnShader();
    virtual glm::vec4 ExecuteVertexShader(void* vs_in, void* fs_in, void* uniforms) override;
    virtual glm::vec4 ExecuteFragmentShader(void* fs_in, void* uniforms, int *discard, int backface) override;
};

#endif //BLINN_SHADER_H
