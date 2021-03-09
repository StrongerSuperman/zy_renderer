
#include <string>
#include <vector>

#include "core/mesh.hpp"
#include "core/texture.hpp"


Mesh::~Mesh(){
    auto iter = this->textures.begin();
    while(iter != textures.end()) {
        for each(auto texture in iter->second){
            delete texture;
        }
		iter++;
    }
}

std::vector<Texture*>* Mesh::GetTexture(std::string& texture_name) {
    if(this->textures.find(texture_name) == this->textures.end()){
        return nullptr;
    }
    return &this->textures[texture_name];
};

