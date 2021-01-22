#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "graphics.hpp"
#include "maths.hpp"


/* image creating/releasing */
Image *image_create(int width, int height, int channels, Format format) {
    int num_elems = width * height * channels;
    Image *image;

    assert(width > 0 && height > 0 && channels >= 1 && channels <= 4);
    assert(format == FORMAT_LDR || format == FORMAT_HDR);

    image = (Image*)malloc(sizeof(Image));
    image->format = format;
    image->width = width;
    image->height = height;
    image->channels = channels;
    image->ldr_buffer = nullptr;
    image->hdr_buffer = nullptr;

    if (format == FORMAT_LDR) {
        int size = sizeof(unsigned char) * num_elems;
        image->ldr_buffer = (unsigned char*)malloc(size);
        memset(image->ldr_buffer, 0, size);
    } else {
        int size = sizeof(float) * num_elems;
        image->hdr_buffer = (float*)malloc(size);
        memset(image->hdr_buffer, 0, size);
    }

    return image;
}

void image_release(Image *image) {
    free(image->ldr_buffer);
    free(image->hdr_buffer);
    free(image);
}


/* framebuffer management */
FrameBuffer *framebuffer_create(int width, int height) {
    int color_buffer_size = width * height * 4;
    int depth_buffer_size = sizeof(float) * width * height;
    vec4_t default_color = {0, 0, 0, 1};
    float default_depth = 1;
    FrameBuffer *framebuffer;

    framebuffer = (FrameBuffer*)malloc(sizeof(FrameBuffer));
    framebuffer->width = width;
    framebuffer->height = height;
    framebuffer->color_buffer = (unsigned char*)malloc(color_buffer_size);
    framebuffer->depth_buffer = (float*)malloc(depth_buffer_size);

    framebuffer_clear_color(framebuffer, default_color);
    framebuffer_clear_depth(framebuffer, default_depth);

    return framebuffer;
}

void framebuffer_release(FrameBuffer *framebuffer) {
    free(framebuffer->color_buffer);
    free(framebuffer->depth_buffer);
    free(framebuffer);
}

unsigned char float_to_uchar1(float value) {
    return (unsigned char)(value * 255);
}

void framebuffer_clear_color(FrameBuffer *framebuffer, vec4_t color) {
    int num_pixels = framebuffer->width * framebuffer->height;
    int i;
    for (i = 0; i < num_pixels; i++) {
        framebuffer->color_buffer[i * 4 + 0] = float_to_uchar1(color.x);
        framebuffer->color_buffer[i * 4 + 1] = float_to_uchar1(color.y);
        framebuffer->color_buffer[i * 4 + 2] = float_to_uchar1(color.z);
        framebuffer->color_buffer[i * 4 + 3] = float_to_uchar1(color.w);
    }
}

void framebuffer_clear_depth(FrameBuffer *framebuffer, float depth) {
    int num_pixels = framebuffer->width * framebuffer->height;
    int i;
    for (i = 0; i < num_pixels; i++) {
        framebuffer->depth_buffer[i] = depth;
    }
}


/* framebuffer blitting */
void blit_bgr(FrameBuffer *src, Image *dst) {
    int width = dst->width;
    int height = dst->height;
    int r, c;

    assert(src->width == dst->width && src->height == dst->height);
    assert(dst->format == FORMAT_LDR && dst->channels == 4);

    for (r = 0; r < height; r++) {
        for (c = 0; c < width; c++) {
            int flipped_r = height - 1 - r;
            int src_index = (r * width + c) * 4;
            int dst_index = (flipped_r * width + c) * 4;
            unsigned char *src_pixel = &src->color_buffer[src_index];
            unsigned char *dst_pixel = &dst->ldr_buffer[dst_index];
            dst_pixel[0] = src_pixel[2];  /* blue */
            dst_pixel[1] = src_pixel[1];  /* green */
            dst_pixel[2] = src_pixel[0];  /* red */
        }
    }
}
