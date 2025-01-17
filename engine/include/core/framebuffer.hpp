#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <glm/glm.hpp>

#include "image.hpp"
#include "program.hpp"


class FrameBuffer{
public:
    int m_Width;
    int m_Height;
    unsigned char *m_ColorBuffer;
    float *m_DepthBuffer;

    FrameBuffer(int width, int height);
    ~FrameBuffer();
    void ClearColor(const glm::vec4& color);
    void ClearDepth(float depth);
    void DrawTriangle(Program* program);
};

#endif //FRAMEBUFFER_H
