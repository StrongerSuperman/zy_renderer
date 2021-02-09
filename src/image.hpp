#pragma once

#include <string>

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
    void LoadFromFile(std::string& file_name);
    void SaveToFile(std::string& file_name);
};
