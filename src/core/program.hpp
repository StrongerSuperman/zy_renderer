#pragma once

#include <glm/glm.hpp>

#include "shader.hpp"


#define MAX_FS_IN_LEN 10

class Program{
public:
    /* for shader */
    Shader* shader;
    int sizeof_vs_in;
    int sizeof_fs_in;
    int sizeof_uniforms;
    void* vs_in[3];
    void* fs_in;
    void* uniforms;
    /* for clipping */
    glm::vec4 vs_in_coords[MAX_FS_IN_LEN];
    glm::vec4 vs_out_coords[MAX_FS_IN_LEN];
    void* fs_in_data[MAX_FS_IN_LEN];
    void* fs_out_data[MAX_FS_IN_LEN];

    Program(int sizeof_vs_in, int sizeof_fs_in, int sizeof_uniforms){
        assert(sizeof_vs_in > 0 && sizeof_fs_in > 0 && sizeof_uniforms > 0);
        assert(sizeof_fs_in % sizeof(float) == 0);

        this->sizeof_vs_in = sizeof_vs_in;
        this->sizeof_fs_in = sizeof_fs_in;
        this->sizeof_uniforms = sizeof_uniforms;

        for (int i = 0; i < 3; i++) {
            this->vs_in[i] = malloc(sizeof_vs_in);
            memset(this->vs_in[i], 0, sizeof_vs_in);
        }
        this->fs_in = malloc(sizeof_fs_in);
        memset(this->fs_in, 0, sizeof_fs_in);
        this->uniforms = malloc(sizeof_uniforms);
        memset(this->uniforms, 0, sizeof_uniforms);

        for (int i = 0; i < MAX_FS_IN_LEN; i++) {
            this->fs_in_data[i] = malloc(sizeof_fs_in);
            memset(this->fs_in_data[i], 0, sizeof_fs_in);
            this->fs_out_data[i] = malloc(sizeof_fs_in);
            memset(this->fs_out_data[i], 0, sizeof_fs_in);
        }
    }
};
