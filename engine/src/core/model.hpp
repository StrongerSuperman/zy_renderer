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

    virtual ~Model() {delete this->mesh; delete this->program;};
    virtual void Update(Perframe *perframe) {(void)(perframe);};
    virtual void Draw(FrameBuffer *framebuffer, bool shadow_pass=false) {
        (void)(framebuffer);
        (void)(shadow_pass);
    };
};

#endif //MODEL_H
