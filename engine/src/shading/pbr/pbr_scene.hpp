#ifndef PBR_SCENE_H
#define PBR_SCENE_H

#include <glm/glm.hpp>

#include "../../core/scene.hpp"


class PBRScene : public Scene{
public:
    PBRScene(std::string& filename);
    void Update() override;
    void Render() override;

private:
    void sortModels(glm::mat4x4& view_matrix);
};

#endif //PBR_SCENE_H
