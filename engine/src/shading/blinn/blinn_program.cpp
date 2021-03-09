#include "shading/blinn/blinn_program.hpp"
#include "shading/blinn/blinn_shader.hpp"


BlinnProgram::BlinnProgram():
    Program(new BlinnShader(),
        sizeof(BlinnVSIn),
        sizeof(BlinnFSIn),
        sizeof(BlinnUniforms)
    ){
    
}
