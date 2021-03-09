#ifndef IMAGE_H
#define IMAGE_H

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

    Image(int width, int height, int channels, const Format& format=FORMAT_LDR);
    Image(const std::string& filename, const Format& format=FORMAT_LDR);
    ~Image();
};

#endif //IMAGE_H
