#include "blinn_program.hpp"
#include "blinn_shader.hpp"


BlinnProgram::BlinnProgram():
    Program(new BlinnShader(),
        sizeof(BlinnVSIn),
        sizeof(BlinnFSIn),
        sizeof(BlinnUniforms)
    ){
    
}
