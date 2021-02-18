#ifndef PERFRAME_H
#define PERFRAME_H

#include <glm/glm.hpp>

#include "texture.hpp"


class Perframe{
public:
    float frame_time;
    float delta_time;
    glm::vec3 light_dir;
    glm::vec3 camera_pos;
    glm::mat4x4 light_view_mat;
    glm::mat4x4 light_proj_mat;
    glm::mat4x4 camera_view_mat;
    glm::mat4x4 camera_proj_mat;
    float ambient_intensity;
    float punctual_intensity;
    Texture *shadow_map;
    int layer_view;
};

#endif //PERFRAME_H
