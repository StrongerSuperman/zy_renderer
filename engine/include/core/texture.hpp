#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

#include <glm/glm.hpp>

#include "framebuffer.hpp"


enum class Usage{
    USAGE_LDR_COLOR,
    USAGE_HDR_COLOR
};

class Texture{
public:
    int m_Width, m_Height;
    glm::vec4 *m_Buffer;

    Texture(int width, int height);
    Texture(const std::string& filename, const Usage& usage);
    void SetWithColorBuffer(FrameBuffer *frame_color_buffer);
    void SetWithDepthBuffer(FrameBuffer *frame_depth_buffer);
    ~Texture();
    glm::vec4 RepeatSample(const glm::vec2& texcoord);
    glm::vec4 ClampSample(const glm::vec2& texcoord);
    glm::vec4 Sample(const glm::vec2& texcoord);
};

class Cubemap{
public:
    Texture *m_Faces[6];

    Cubemap(const std::string& positive_x, const std::string& negative_x,
			const std::string& positive_y, const std::string& negative_y,
			const std::string& positive_z, const std::string& negative_z,
			const Usage& usage);
    ~Cubemap();
    glm::vec4 RepeatSample(const glm::vec3& direction);
    glm::vec4 ClampSample(const glm::vec3& direction);
    glm::vec4 Sample(const glm::vec3& direction);
};

#endif //TEXTURE_H
