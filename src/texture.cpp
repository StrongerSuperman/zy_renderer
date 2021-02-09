#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "graphics.hpp"
#include "image.hpp"
#include "texture.hpp"
#include "mathtool.hpp"


static void LDRImageToTexture(Image *image, Texture *texture) {
    int num_pixels = image->m_Width * image->m_Height;
    int i;

    for (i = 0; i < num_pixels; i++) {
        unsigned char *pixel = &image->m_LDRBuffer[i * image->m_Channels];
        glm::vec4 texel = {0, 0, 0, 1};
        if (image->m_Channels == 1) {             /* GL_LUMINANCE */
            texel.x = texel.y = texel.z = float_from_uchar(pixel[0]);
        } else if (image->m_Channels == 2) {      /* GL_LUMINANCE_ALPHA */
            texel.x = texel.y = texel.z = float_from_uchar(pixel[0]);
            texel.w = float_from_uchar(pixel[1]);
        } else if (image->m_Channels == 3) {      /* GL_RGB */
            texel.x = (unsigned char)(pixel[0]);
            texel.y = (unsigned char)(pixel[1]);
            texel.z = (unsigned char)(pixel[2]);
        } else {                                /* GL_RGBA */
            texel.x = (unsigned char)(pixel[0]);
            texel.y = (unsigned char)(pixel[1]);
            texel.z = (unsigned char)(pixel[2]);
            texel.w = (unsigned char)(pixel[3]);
        }
        texture->m_Buffer[i] = texel;
    }
}

static void HDRImageToTexture(Image *image, Texture *texture) {
    int num_pixels = image->m_Width * image->m_Height;
    int i;

    for (i = 0; i < num_pixels; i++) {
        float *pixel = &image->m_HDRBuffer[i * image->m_Channels];
        glm::vec4 texel = {0, 0, 0, 1};
        if (image->m_Channels == 1) {             /* GL_LUMINANCE */
            texel.x = texel.y = texel.z = pixel[0];
        } else if (image->m_Channels == 2) {      /* GL_LUMINANCE_ALPHA */
            texel.x = texel.y = texel.z = pixel[0];
            texel.w = pixel[1];
        } else if (image->m_Channels == 3) {      /* GL_RGB */
            texel.x = pixel[0];
            texel.y = pixel[1];
            texel.z = pixel[2];
        } else {                                /* GL_RGBA */
            texel.x = pixel[0];
            texel.y = pixel[1];
            texel.z = pixel[2];
            texel.w = pixel[3];
        }
        texture->m_Buffer[i] = texel;
    }
}

static void SRGBToLinear(Texture *texture) {
    int num_pixels = texture->m_Width * texture->m_Height;
    int i;

    for (i = 0; i < num_pixels; i++) {
        glm::vec4 *pixel = &texture->m_Buffer[i];
        pixel->x = float_srgb2linear(pixel->x);
        pixel->y = float_srgb2linear(pixel->y);
        pixel->z = float_srgb2linear(pixel->z);
    }
}

static void LinearToSRGB(Texture *texture) {
    int num_pixels = texture->m_Width * texture->m_Height;
    int i;

    for (i = 0; i < num_pixels; i++) {
        glm::vec4 *pixel = &texture->m_Buffer[i];
        pixel->x = float_linear2srgb(float_aces(pixel->x));
        pixel->y = float_linear2srgb(float_aces(pixel->y));
        pixel->z = float_linear2srgb(float_aces(pixel->z));
    }
}

void Texture::Create(int width, int height) {
    assert(width > 0 && height > 0);
    int buffer_size = sizeof(glm::vec4) * width * height;

    this->m_Width = width;
    this->m_Height = height;
    this->m_Buffer = (glm::vec4*)malloc(buffer_size);
    memset(this->m_Buffer, 0, buffer_size);
}

void Texture::Release() {
    free(this->m_Buffer);
}

void Texture::CreateFromFile(std::string& filename, Usage usage) {
    Image image;
    image.LoadFromFile(filename);
    this->Create(image.m_Width, image.m_Height);
    if (image.m_Format == FORMAT_LDR) {
        LDRImageToTexture(&image, this);
        if (usage == USAGE_HDR_COLOR) {
            SRGBToLinear(this);
        }
    } else {
        HDRImageToTexture(&image, this);
        if (usage == USAGE_LDR_COLOR) {
            LinearToSRGB(this);
        }
    }
    image.Release();
}

void Texture::CreateFromColorBuffer(FrameBuffer *framebuffer) {
    int num_pixels = this->m_Width * this->m_Height;
    int i;

    assert(this->m_Width == framebuffer->m_Width);
    assert(this->m_Height == framebuffer->m_Height);

    for (i = 0; i < num_pixels; i++) {
        unsigned char *color = &framebuffer->m_ColorBuffer[i * 4];
        float r = float_from_uchar(color[0]);
        float g = float_from_uchar(color[1]);
        float b = float_from_uchar(color[2]);
        float a = float_from_uchar(color[3]);
        this->m_Buffer[i] = glm::vec4(r, g, b, a);
    }
}

void Texture::CreateFromDepthBuffer(FrameBuffer *framebuffer) {
    int num_pixels = this->m_Width * this->m_Height;
    int i;

    assert(this->m_Width == framebuffer->m_Width);
    assert(this->m_Height == framebuffer->m_Height);

    for (i = 0; i < num_pixels; i++) {
        float depth = framebuffer->m_DepthBuffer[i];
        this->m_Buffer[i] = glm::vec4(depth, depth, depth, 1);
    }
}

glm::vec4 Texture::RepeatSample(glm::vec2& texcoord) {
    float u = texcoord.x - (float)floor(texcoord.x);
    float v = texcoord.y - (float)floor(texcoord.y);
    int c = (int)((this->m_Width - 1) * u);
    int r = (int)((this->m_Height - 1) * v);
    int index = r * this->m_Width + c;
    return this->m_Buffer[index];
}

glm::vec4 Texture::ClampSample(glm::vec2& texcoord) {
    float u = float_saturate(texcoord.x);
    float v = float_saturate(texcoord.y);
    int c = (int)((this->m_Width - 1) * u);
    int r = (int)((this->m_Height - 1) * v);
    int index = r * this->m_Width + c;
    return this->m_Buffer[index];
}

glm::vec4 Texture::Sample(glm::vec2& texcoord) {
    return RepeatSample(texcoord);
}



/*
 * for cubemap sampling, see subsection 3.7.5 of
 * https://www.khronos.org/registry/OpenGL/specs/es/2.0/es_full_spec_2.0.pdf
 */
static int SelectCubemapFace(glm::vec3& direction, glm::vec2 *texcoord) {
    float abs_x = (float)fabs(direction.x);
    float abs_y = (float)fabs(direction.y);
    float abs_z = (float)fabs(direction.z);
    float ma, sc, tc;
    int face_index;

    if (abs_x > abs_y && abs_x > abs_z) {   /* major axis -> x */
        ma = abs_x;
        if (direction.x > 0) {                  /* positive x */
            face_index = 0;
            sc = -direction.z;
            tc = -direction.y;
        } else {                                /* negative x */
            face_index = 1;
            sc = +direction.z;
            tc = -direction.y;
        }
    } else if (abs_y > abs_z) {             /* major axis -> y */
        ma = abs_y;
        if (direction.y > 0) {                  /* positive y */
            face_index = 2;
            sc = +direction.x;
            tc = +direction.z;
        } else {                                /* negative y */
            face_index = 3;
            sc = +direction.x;
            tc = -direction.z;
        }
    } else {                                /* major axis -> z */
        ma = abs_z;
        if (direction.z > 0) {                  /* positive z */
            face_index = 4;
            sc = +direction.x;
            tc = -direction.y;
        } else {                                /* negative z */
            face_index = 5;
            sc = -direction.x;
            tc = -direction.y;
        }
    }

    texcoord->x = (sc / ma + 1) / 2;
    texcoord->y = (tc / ma + 1) / 2;
    return face_index;
}

void Cubemap::CreateFromFile(std::string& positive_x, std::string& negative_x,
                              std::string& positive_y, std::string& negative_y,
                              std::string& positive_z, std::string& negative_z,
                              Usage usage) {
    this->m_Faces[0]->CreateFromFile(positive_x, usage);
    this->m_Faces[1]->CreateFromFile(negative_x, usage);
    this->m_Faces[2]->CreateFromFile(positive_y, usage);
    this->m_Faces[3]->CreateFromFile(negative_y, usage);
    this->m_Faces[4]->CreateFromFile(positive_z, usage);
    this->m_Faces[5]->CreateFromFile(negative_z, usage);
}

void Cubemap::Release() {
    for(int i = 0; i < 6; i++){
        this->m_Faces[i]->Release();
    }
}

glm::vec4 Cubemap::RepeatSample(glm::vec3& direction) {
    glm::vec2 texcoord;
    int face_index = SelectCubemapFace(direction, &texcoord);
    texcoord.y = 1 - texcoord.y;
    return this->m_Faces[face_index]->RepeatSample(texcoord);
}

glm::vec4 Cubemap::ClampSample(glm::vec3& direction) {
    glm::vec2 texcoord;
    int face_index = SelectCubemapFace(direction, &texcoord);
    texcoord.y = 1 - texcoord.y;
    return this->m_Faces[face_index]->ClampSample(texcoord);
}

glm::vec4 Cubemap::Sample(glm::vec3& direction) {
    return RepeatSample(direction);
}
