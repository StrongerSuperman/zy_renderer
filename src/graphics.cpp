#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <glm/glm.hpp>

#include "graphics.hpp"
#include "image.hpp"


void FrameBuffer::Create(int width, int height) {
    int color_buffer_size = width * height * 4;
    int depth_buffer_size = sizeof(float) * width * height;
    auto default_color = glm::vec4(0, 0, 0, 1);
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

void FrameBuffer::ClearColor(glm::vec4 color) {
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