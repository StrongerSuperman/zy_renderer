#ifndef MODEL_H
#define MODEL_H

#include <glm/glm.hpp>

#include "framebuffer.hpp"
#include "mesh.hpp"


class Model{
public:
    Mesh* mesh;
    Program* program;
    glm::mat4x4 transform;
    /* for sorting */
    int opaque;
    float distance;

    Model(Mesh* mesh, glm::mat4x4& transform, Program* program) {
        this->mesh = mesh;
        this->transform = transform;
        this->program = program;
    };
    virtual ~Model() {delete this->mesh; delete this->program;};
    virtual void Update(Perframe *perframe)=0;
    virtual void Draw(FrameBuffer *framebuffer, bool shadow_pass=false)=0;
};

#endif //MODEL_H
