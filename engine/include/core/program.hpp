#ifndef PROGRAM_H
#define PROGRAM_H

#include <vector>

#include <glm/glm.hpp>

#include "shader.hpp"


#define MAX_FS_IN_LEN 10

class Program{
public:
    int double_sided;
    int enable_blend;
    /* for shader */
    Shader* shader;
    int sizeof_vs_in;
    int sizeof_fs_in;
    int sizeof_uniforms;
    void* vs_in[3];
    void* fs_in;
    void* uniforms;
    /* for clipping */
    std::vector<glm::vec4> vs_in_clip_coords;
    std::vector<glm::vec4> vs_out_clip_coords;
    std::vector<void*> fs_in_clip_data;
    std::vector<void*> fs_out_clip_data;

    Program(Shader* shader, int sizeof_vs_in, int sizeof_fs_in, int sizeof_uniforms):
            double_sided(false), enable_blend(false){
        assert(sizeof_vs_in > 0 && sizeof_fs_in > 0 && sizeof_uniforms > 0);
        assert(sizeof_fs_in % sizeof(float) == 0);

        this->shader = shader;
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

        this->vs_in_clip_coords.resize(MAX_FS_IN_LEN);
        this->vs_out_clip_coords.resize(MAX_FS_IN_LEN);
        this->fs_in_clip_data.resize(MAX_FS_IN_LEN);
        this->fs_out_clip_data.resize(MAX_FS_IN_LEN);
        for(int i = 0; i < MAX_FS_IN_LEN; i++){
            this->fs_in_clip_data[i] = malloc(sizeof_fs_in);
            this->fs_out_clip_data[i] = malloc(sizeof_fs_in);
        }
    }

    ~Program(){
        delete this->shader;

        for(int i=0; i<3; i++){
            free(this->vs_in[i]);
        }
        free(this->fs_in);
        free(this->uniforms);
        for(int i=0; i<MAX_FS_IN_LEN; i++){
            free(this->fs_in_clip_data[i]);
            free(this->fs_out_clip_data[i]);
        }
    }
};

#endif //PROGRAM_H
