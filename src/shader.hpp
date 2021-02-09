#pragma once

#include <algorithm>

#include <glm/glm.hpp>

#include "texture.hpp"


struct BlinnMaterial{
    glm::vec3 diffuse;
    glm::vec3 specular;
    float alpha;
    float shininess;
    glm::vec3 normal;
    glm::vec3 emission;
} ;

struct BlinnVSIn{
    glm::vec3 position;
    glm::vec2 texcoord;
    glm::vec3 normal;
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
    glm::mat4 model_mat;
    glm::mat4 view_mat;
    glm::mat4 proj_mat;
    float ambient_intensity;
    float punctual_intensity;
    Texture* shadow_map;
    /* surface parameters */
    glm::vec3 basecolor;
    float shininess;
    Texture *diffuse_map;
    Texture *specular_map;
    Texture *emission_map;
    /* render controls */
    float alpha_cutoff;
    int shadow_pass;
};

glm::vec4 ExecuteBlinnVertexShader(BlinnVSIn *vs_in, BlinnFSIn *fs_in, BlinnUniforms *uniforms);
glm::vec4 ExecuteBlinnFragmentShader(BlinnFSIn *fs_in, BlinnUniforms *uniforms, BlinnMaterial *material, int *discard);
