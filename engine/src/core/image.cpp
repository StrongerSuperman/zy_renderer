#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <glm/glm.hpp>
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

#include "image.hpp"


Image::Image(int width, int height, int channels, Format format):
        m_Format(format),
        m_Width(width),
        m_Height(height),
        m_Channels(channels),
        m_LDRBuffer(nullptr),
        m_HDRBuffer(nullptr){
    int num_elems = width * height * channels;
    assert(width > 0 && height > 0 && channels >= 1 && channels <= 4);
    assert(format == FORMAT_LDR || format == FORMAT_HDR);

    this->m_Format = format;
    this->m_Width = width;
    this->m_Height = height;
    this->m_Channels = channels;

    if (format == FORMAT_LDR) {
        int size = sizeof(unsigned char) * num_elems;
        this->m_LDRBuffer = (unsigned char*)malloc(size);
        memset(this->m_LDRBuffer, 0, size);
    } else {
        int size = sizeof(float) * num_elems;
        this->m_HDRBuffer = (float*)malloc(size);
        memset(this->m_HDRBuffer, 0, size);
    }
}

Image::Image(std::string& filename, Format format):
        m_Format(format),
        m_LDRBuffer(nullptr),
        m_HDRBuffer(nullptr){
    if(this->m_Format == FORMAT_LDR){
        this->m_LDRBuffer = stbi_load(
        filename.c_str(), &this->m_Width, &this->m_Height, &this->m_Channels, 0);
    }
}

Image::~Image() {
    free(this->m_LDRBuffer);
    free(this->m_HDRBuffer);
}
