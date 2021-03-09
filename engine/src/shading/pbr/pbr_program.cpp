#include "shading/pbr/pbr_program.hpp"
#include "shading/pbr/pbr_shader.hpp"


PBRProgram::PBRProgram():
    Program(new PBRShader(),
        sizeof(PBRVSIn),
        sizeof(PBRFSIn),
        sizeof(PBRUniforms)
    ){
    
}
