#ifndef MATERIAL_H
#define MATERIAL_H

#include <vector>

#include "core/texture.hpp"


class Material{
public:
    // blinn
    glm::vec3 ka;
    glm::vec3 kd;
    glm::vec3 ks;
    std::vector<Texture*> diffuse_textures;
    std::vector<Texture*> specular_textures;
    std::vector<Texture*> ambient_textures;
    std::vector<Texture*> emissive_textures;
    std::vector<Texture*> height_textures;
    std::vector<Texture*> normal_textures;
    std::vector<Texture*> shininess_textures;
    std::vector<Texture*> opacity_textures;
    std::vector<Texture*> displacement_textures;
    std::vector<Texture*> lightmap_textures;
    std::vector<Texture*> reflection_textures;
    // pbr
    std::vector<Texture*> base_color_textures;
    std::vector<Texture*> nomal_camera_textures;
    std::vector<Texture*> emission_color_textures;
    std::vector<Texture*> metalness_textures;
    std::vector<Texture*> diffuse_roughness_textures;
    std::vector<Texture*> ambient_occlusion_textures;

    ~Material(){
        this->destroy_textures(diffuse_textures);
        this->destroy_textures(specular_textures);
        this->destroy_textures(ambient_textures);
        this->destroy_textures(emissive_textures);
        this->destroy_textures(height_textures);
        this->destroy_textures(normal_textures);
        this->destroy_textures(shininess_textures);
        this->destroy_textures(opacity_textures);
        this->destroy_textures(displacement_textures);
        this->destroy_textures(lightmap_textures);
        this->destroy_textures(reflection_textures);

        this->destroy_textures(base_color_textures);
        this->destroy_textures(nomal_camera_textures);
        this->destroy_textures(emission_color_textures);
        this->destroy_textures(metalness_textures);
        this->destroy_textures(diffuse_roughness_textures);
        this->destroy_textures(ambient_occlusion_textures);
    }
private:
    void destroy_textures(std::vector<Texture*> textures){
        for each(auto texture in textures){
            delete texture;
        }
    }
};


#endif //MATERIAL_H
