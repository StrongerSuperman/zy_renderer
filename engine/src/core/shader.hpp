#ifndef SHADER_H
#define SHADER_H

#include <glm/glm.hpp>


class Shader{
public:
    virtual ~Shader() {};
    virtual glm::vec4 ExecuteVertexShader(void* vs_in, void* fs_in, void* uniforms)=0;
    virtual glm::vec4 ExecuteFragmentShader(void* fs_in, void* uniforms, int *discard, int backface)=0;
};

#endif //SHADER_H
