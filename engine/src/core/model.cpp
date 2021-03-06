#include <glm/glm.hpp>

#include "mesh.hpp"
#include "model.hpp"
#include "scene.hpp"
#include "Program.hpp"


Model::Model(Scene* scene, Mesh* mesh, glm::mat4x4& transform, Program* program) {
    this->mesh = mesh;
    this->transform = transform;
    this->program = program;
    this->scene = scene;
};
