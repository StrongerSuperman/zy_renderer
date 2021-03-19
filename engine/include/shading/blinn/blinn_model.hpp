#ifndef BLINN_MODEL_H
#define BLINN_MODEL_H

#include <vector>

#include <glm/glm.hpp>

#include "core/framebuffer.hpp"
#include "core/mesh.hpp"
#include "core/material.hpp"
#include "core/model.hpp"
#include "core/scene.hpp"


class BlinnModel : public Model{
public:
    BlinnModel(Scene* scene, Mesh* mesh, Material* material, const glm::mat4x4& transform);
    void Update() override;
    void Draw(FrameBuffer* framebuffer, bool shadow_pass=false) override;
};

#endif //BLINN_MODEL_H
