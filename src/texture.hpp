#pragma once

#include <string>

#include <glm/glm.hpp>

#include "graphics.hpp"


enum Usage{
    USAGE_LDR_COLOR,
    USAGE_LDR_DATA,
    USAGE_HDR_COLOR,
    USAGE_HDR_DATA
};

class Texture{
public:
    int m_Width, m_Height;
    glm::vec4 *m_Buffer;

    void Create(int width, int height);
    void CreateFromFile(std::string& filename, Usage usage);
    void CreateFromColorBuffer(FrameBuffer *framebuffer);
    void CreateFromDepthBuffer(FrameBuffer *framebuffer);
    glm::vec4 RepeatSample(glm::vec2& texcoord);
    glm::vec4 ClampSample(glm::vec2& texcoord);
    glm::vec4 Sample(glm::vec2& texcoord);
    void Release();
};

class Cubemap{
public:
    Texture *m_Faces[6];

    void CreateFromFile(std::string& positive_x, std::string& negative_x,
                        std::string& positive_y, std::string& negative_y,
                        std::string& positive_z, std::string& negative_z,
                        Usage usage);
    glm::vec4 RepeatSample(glm::vec3& direction);
    glm::vec4 ClampSample(glm::vec3& direction);
    glm::vec4 Sample(glm::vec3& direction);
    void Release();
};
