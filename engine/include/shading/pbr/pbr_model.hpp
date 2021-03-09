#ifndef PBR_MODEL_H
#define PBR_MODEL_H

#include <vector>

#include <glm/glm.hpp>

#include "../../core/framebuffer.hpp"
#include "../../core/perframe.hpp"
#include "../../core/mesh.hpp"
#include "../../core/model.hpp"
#include "../../core/scene.hpp"


class PBRModel : public Model{
public:
    PBRModel(Scene* scene, Mesh* mesh, glm::mat4x4& transform);
    void Update() override;
    void Draw(FrameBuffer* framebuffer, bool shadow_pass=false) override;
};

#endif //PBR_MODEL_H
