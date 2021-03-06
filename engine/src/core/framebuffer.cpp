#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <glm/glm.hpp>

#include "framebuffer.hpp"
#include "graphics.hpp"
#include "program.hpp"
#include "image.hpp"


FrameBuffer::FrameBuffer(int width, int height) {
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

FrameBuffer::~FrameBuffer() {
    free(this->m_ColorBuffer);
    free(this->m_DepthBuffer);
}

void FrameBuffer::ClearColor(glm::vec4 color) {
    int num_pixels = this->m_Width * this->m_Height;
    for (int i = 0; i < num_pixels; i++) {
       this->m_ColorBuffer[i * 4 + 0] = (unsigned char)(color.x * 255);
       this->m_ColorBuffer[i * 4 + 1] = (unsigned char)(color.y * 255);
       this->m_ColorBuffer[i * 4 + 2] = (unsigned char)(color.z * 255);
       this->m_ColorBuffer[i * 4 + 3] = (unsigned char)(color.w * 255);
    }
}

void FrameBuffer::ClearDepth(float depth) {
    int num_pixels = this->m_Width * this->m_Height;
    for (int i = 0; i < num_pixels; i++) {
        this->m_DepthBuffer[i] = depth;
    }
}

void FrameBuffer::DrawTriangle(Program* program){
   graphics_draw_triangle(this, program);
}
