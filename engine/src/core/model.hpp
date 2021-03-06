#ifndef MODEL_H
#define MODEL_H

#include <glm/glm.hpp>

#include "framebuffer.hpp"
#include "mesh.hpp"
#include "program.hpp"

class Scene;

class Model{
public:
    Mesh* mesh;
    Program* program;
    glm::mat4x4 transform;
    /* for sorting */
    int opaque;
    float distance;

    Model(Scene* scene, Mesh* mesh, glm::mat4x4& transform, Program* program);
    virtual ~Model() {delete this->mesh; delete this->program;};
    virtual void Update()=0;
    virtual void Draw(FrameBuffer* framebuffer, bool shadow_pass=false)=0;

    Scene* GetScene(){return this->scene;};

private:
    Scene* scene;
};

#endif //MODEL_H
