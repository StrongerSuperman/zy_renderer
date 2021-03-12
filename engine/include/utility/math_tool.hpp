#ifndef MATHTOOL_H
#define MATHTOOL_H

#include <algorithm>

#include <glm/glm.hpp>


static int min_integer(int a, int b) {
    return a < b ? a : b;
}

static int max_integer(int a, int b) {
    return a > b ? a : b;
}

static float float_min(float a, float b) {
    return a < b ? a : b;
}

static float float_max(float a, float b) {
    return a > b ? a : b;
}

static float float_lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

static float float_clamp(float f, float min, float max) {
    return f < min ? min : (f > max ? max : f);
}

static float float_saturate(float f) {
    return f < 0 ? 0 : (f > 1 ? 1 : f);
}

static float float_from_uchar(unsigned char value) {
    return value / 255.0f;
}

static unsigned char float_to_uchar(float value) {
    return (unsigned char)(value * 255);
}

static float float_srgb2linear(float value) {
    return (float)pow(value, 2.2);
}

static float float_linear2srgb(float value) {
    return (float)pow(value, 1 / 2.2);
}

/*
 * for aces filmic tone mapping curve, see
 * https://knarkowicz.wordpress.com/2016/01/06/aces-filmic-tone-mapping-curve/
 */
static float float_aces(float value) {
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    value = (value * (a * value + b)) / (value * (c * value + d) + e);
    return float_saturate(value);
}

static glm::vec4 vec4_lerp(const glm::vec4& a, const glm::vec4& b, float t) {
    float x = float_lerp(a.x, b.x, t);
    float y = float_lerp(a.y, b.y, t);
    float z = float_lerp(a.z, b.z, t);
    float w = float_lerp(a.w, b.w, t);
    return glm::vec4(x, y, z, w);
}

static glm::vec2 vec2_min(const glm::vec2& a, const glm::vec2& b) {
    float x = float_min(a.x, b.x);
    float y = float_min(a.y, b.y);
    return glm::vec2(x, y);
}

static glm::vec2 vec2_max(const glm::vec2& a, const glm::vec2& b) {
    float x = float_max(a.x, b.x);
    float y = float_max(a.y, b.y);
    return glm::vec2(x, y);
}

static glm::vec3 vec3_div(const glm::vec3& v, float divisor) {
    return glm::vec3(v.x/divisor, v.y/divisor, v.z/divisor);
}

static glm::vec4 vec4_saturate(const glm::vec4& v) {
    float x = float_saturate(v.x);
    float y = float_saturate(v.y);
    float z = float_saturate(v.z);
    float w = float_saturate(v.w);
    return glm::vec4(x, y, z, w);
}

static glm::vec3 float_minus_vec3(float a, const glm::vec3& v){
    return glm::vec3(a-v.x, a-v.y, a-v.z);
}

#endif //MATHTOOL_H
