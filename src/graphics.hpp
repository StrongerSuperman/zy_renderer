#pragma once

#include "maths.hpp"


typedef enum {
    FORMAT_LDR,
    FORMAT_HDR
} Format;

typedef struct {
    Format format;
    int width, height, channels;
    unsigned char *ldr_buffer;
    float *hdr_buffer;
} Image;

/* image creating/releasing */
Image *image_create(int width, int height, int channels, Format format);
void image_release(Image *image);


typedef struct {
    int width, height;
    unsigned char *color_buffer;
    float *depth_buffer;
} FrameBuffer;

/* framebuffer management */
FrameBuffer *framebuffer_create(int width, int height);
void framebuffer_release(FrameBuffer *framebuffer);
void framebuffer_clear_color(FrameBuffer *framebuffer, vec4_t color);
void framebuffer_clear_depth(FrameBuffer *framebuffer, float depth);


/* framebuffer blitting */
void blit_bgr(FrameBuffer *source, Image *target);
