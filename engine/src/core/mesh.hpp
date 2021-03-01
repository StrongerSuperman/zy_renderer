#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>
#include <map>

#include <glm/glm.hpp>


class Vertex{
public:
    glm::vec3 position;
    glm::vec2 texcoord;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec3 bitangent;
    glm::vec4 joint;
    glm::vec4 weight;
};

class Mesh{
public:
    int num_faces;
    glm::vec3 center;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::map<std::string, std::vector<Texture*>> textures;

    ~Mesh(){
        auto iter = this->textures.begin();
        while(iter != textures.end()) {
            for each(auto texture in iter->second){
                delete texture;
            }
        }
    }

    int GetFaceNum() {return this->num_faces;};
    glm::vec3 GetCenter() {return this->center;};
    std::vector<Vertex>* GetVertices() {return &(this->vertices);};
    std::vector<unsigned int>* GetIndices() {return &(this->indices);};
    std::vector<Texture*>* GetTexture(std::string& texture_name) {
        if(this->textures.find(texture_name) == this->textures.end()){
            return nullptr;
        }
        return &this->textures[texture_name];
    };
};

#endif //MESH_H
