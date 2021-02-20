#ifndef BLINN_SCENE_H
#define BLINN_SCENE_H

#include <glm/glm.hpp>

#include "../core/framebuffer.hpp"
#include "../core/perframe.hpp"

#include "../core/scene.hpp"


class BlinnScene : public Scene{
public:
    BlinnScene(std::string& filename);
    virtual void Update(Perframe *perframe) override;
    virtual void Render(FrameBuffer *framebuffer, Perframe *perframe) override;

    void sortModels(glm::mat4x4& view_matrix);
};

#endif //BLINN_SCENE_H
