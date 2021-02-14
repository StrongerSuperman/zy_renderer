#pragma once

#include <string>

#include <glm/glm.hpp>

#include "framebuffer.hpp"


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

    Texture(int width, int height);
    Texture(std::string& filename, Usage usage);
    void SetWithColorBuffer(FrameBuffer *frame_color_buffer);
    void SetWithDepthBuffer(FrameBuffer *frame_depth_buffer);
    ~Texture();
    glm::vec4 RepeatSample(glm::vec2& texcoord);
    glm::vec4 ClampSample(glm::vec2& texcoord);
    glm::vec4 Sample(glm::vec2& texcoord);
};

class Cubemap{
public:
    Texture *m_Faces[6];

    Cubemap(std::string& positive_x, std::string& negative_x,
            std::string& positive_y, std::string& negative_y,
            std::string& positive_z, std::string& negative_z,
            Usage usage);
    ~Cubemap();
    glm::vec4 RepeatSample(glm::vec3& direction);
    glm::vec4 ClampSample(glm::vec3& direction);
    glm::vec4 Sample(glm::vec3& direction);
};
