#pragma once

#include <glm/glm.hpp>


class Shader{
public:
    Shader();
    virtual ~Shader();
    virtual glm::vec4 ExecuteVertexShader(void* vs_in, void* fs_in, void* uniforms)=0;
    virtual glm::vec4 ExecuteFragmentShader(void* fs_in, void* uniforms, int *discard, int backface)=0;
};
