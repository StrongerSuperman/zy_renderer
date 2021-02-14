#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "../core/framebuffer.hpp"
#include "../core/perframe.hpp"
#include "../core/mesh.hpp"
#include "../core/model.hpp"


class BlinnModel : public Model{
public:
    BlinnModel(Mesh* mesh, glm::mat4x4& transform);
    virtual ~BlinnModel()=0;
    virtual void Update(Perframe *perframe);
    virtual void Draw(FrameBuffer *framebuffer, bool shadow_pass=false);
};
