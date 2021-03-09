#ifndef BLINN_SCENE_H
#define BLINN_SCENE_H

#include <glm/glm.hpp>

#include "core/scene.hpp"


class BlinnScene : public Scene{
public:
    BlinnScene(const std::string& filename);
    void Update() override;
    void Render() override;

private:
    void sortModels(const glm::mat4x4& view_matrix);
};

#endif //BLINN_SCENE_H
