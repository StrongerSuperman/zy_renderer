#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>


class Vertex{
public:
    glm::vec3 position;
    glm::vec2 texcoord;
    glm::vec3 normal;
    glm::vec4 tangent;
    glm::vec4 joint;
    glm::vec4 weight;
};

class Mesh{
public:
    int num_faces;
    std::vector<Vertex> vertices;
    glm::vec3 center;

    Mesh(std::string& filename);

    int GetFaceNum() {return this->num_faces;};
    std::vector<Vertex> GetVertices() const {return this->vertices;};
    glm::vec3 GetCenter() {return this->center;};
};
