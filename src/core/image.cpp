#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <glm/glm.hpp>

#include "image.hpp"


Image::Image(int width, int height, int channels, Format format){
    int num_elems = width * height * channels;
    assert(width > 0 && height > 0 && channels >= 1 && channels <= 4);
    assert(format == FORMAT_LDR || format == FORMAT_HDR);

    this->m_Format = format;
    this->m_Width = width;
    this->m_Height = height;
    this->m_Channels = channels;
    this->m_LDRBuffer = nullptr;
    this->m_HDRBuffer = nullptr;

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

Image::Image(std::string& filename) {
}

Image::~Image() {
    free(this->m_LDRBuffer);
    free(this->m_HDRBuffer);
}

void Image::SaveToFile(std::string& filename) {
}

