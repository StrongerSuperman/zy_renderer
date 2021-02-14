#include <algorithm>


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
