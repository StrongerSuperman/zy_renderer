#include "pbr_program.hpp"
#include "pbr_shader.hpp"


PBRProgram::PBRProgram():
    Program(new PBRShader(),
        sizeof(PBRVSIn),
        sizeof(PBRFSIn),
        sizeof(PBRUniforms)
    ){
    
}
