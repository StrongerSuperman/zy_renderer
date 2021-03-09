#include <glm/glm.hpp>

#include "core/model.hpp"
#include "core/mesh.hpp"
#include "core/scene.hpp"
#include "core/Program.hpp"


Model::Model(Scene* scene, Mesh* mesh, glm::mat4x4& transform, Program* program) {
    this->mesh = mesh;
    this->transform = transform;
    this->program = program;
    this->scene = scene;
};
