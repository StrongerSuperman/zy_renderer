#pragma once

#include <glm/glm.hpp>

#include "image.hpp"


class FrameBuffer{
public:
    int m_Width;
    int m_Height;
    unsigned char *m_ColorBuffer;
    float *m_DepthBuffer;

    void Create(int width, int height);
    void Release();
    void ClearColor(glm::vec4 color);
    void ClearDepth(float depth);
    void BlitBGR(Image *target);
};
