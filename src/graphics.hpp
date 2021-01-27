#pragma once

#include <glm/glm.hpp>


enum Format{
    FORMAT_LDR,
    FORMAT_HDR
};


class Image{
public:
    Format m_Format;
    int m_Width;
    int m_Height;
    int m_Channels;
    unsigned char *m_LDRBuffer;
    float *m_HDRBuffer;

    void Create(int width, int height, int channels, Format format);
    void Release();
};


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
