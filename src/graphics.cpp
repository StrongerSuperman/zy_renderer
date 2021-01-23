#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "graphics.hpp"
#include "maths.hpp"


void Image::Create(int width, int height, int channels, Format format){
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

void Image::Release() {
    free(this->m_LDRBuffer);
    free(this->m_HDRBuffer);
}


void FrameBuffer::Create(int width, int height) {
    int color_buffer_size = width * height * 4;
    int depth_buffer_size = sizeof(float) * width * height;
    vec4_t default_color = {0, 0, 0, 1};
    float default_depth = 1;

    this->m_Width = width;
    this->m_Height = height;
    this->m_ColorBuffer = (unsigned char*)malloc(color_buffer_size);
    this->m_DepthBuffer = (float*)malloc(depth_buffer_size);

    this->ClearColor(default_color);
    this->ClearDepth(default_depth);
}

void FrameBuffer::Release() {
    free(this->m_ColorBuffer);
    free(this->m_DepthBuffer);
}

void FrameBuffer::ClearColor(vec4_t color) {
    int num_pixels = this->m_Width * this->m_Height;
    int i;
    for (i = 0; i < num_pixels; i++) {
       this->m_ColorBuffer[i * 4 + 0] = (unsigned char)(color.x * 255);
       this->m_ColorBuffer[i * 4 + 1] = (unsigned char)(color.y * 255);
       this->m_ColorBuffer[i * 4 + 2] = (unsigned char)(color.z * 255);
       this->m_ColorBuffer[i * 4 + 3] = (unsigned char)(color.w * 255);
    }
}

void FrameBuffer::ClearDepth(float depth) {
    int num_pixels = this->m_Width * this->m_Height;
    int i;
    for (i = 0; i < num_pixels; i++) {
        this->m_DepthBuffer[i] = depth;
    }
}

void FrameBuffer::BlitBGR(Image *dst) {
    int r, c;

    assert(this->m_Width == dst->m_Width && this->m_Height == dst->m_Height);
    assert(dst->m_Format == FORMAT_LDR && dst->m_Channels == 4);

    for (r = 0; r < dst->m_Height; r++) {
        for (c = 0; c < dst->m_Width; c++) {
            int flipped_r = dst->m_Height - 1 - r;
            int src_index = (r * dst->m_Width + c) * 4;
            int dst_index = (flipped_r * dst->m_Width + c) * 4;
            unsigned char *src_pixel = &this->m_ColorBuffer[src_index];
            unsigned char *dst_pixel = &dst->m_LDRBuffer[dst_index];
            dst_pixel[0] = src_pixel[2];  /* blue */
            dst_pixel[1] = src_pixel[1];  /* green */
            dst_pixel[2] = src_pixel[0];  /* red */
        }
    }
}