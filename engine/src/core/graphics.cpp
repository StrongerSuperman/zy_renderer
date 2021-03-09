#include <vector>

#include <glm/glm.hpp>

#include "core/graphics.hpp"
#include "utility/math_tool.hpp"


#define EPSILON 1e-5f

typedef enum {
    POSITIVE_W,
    POSITIVE_X,
    NEGATIVE_X,
    POSITIVE_Y,
    NEGATIVE_Y,
    POSITIVE_Z,
    NEGATIVE_Z
} PlaneType;

static int is_inside_plane(const glm::vec4& coord, const PlaneType& plane) {
    switch (plane) {
        case POSITIVE_W:
            return coord.w >= EPSILON;
        case POSITIVE_X:
            return coord.x <= +coord.w;
        case NEGATIVE_X:
            return coord.x >= -coord.w;
        case POSITIVE_Y:
            return coord.y <= +coord.w;
        case NEGATIVE_Y:
            return coord.y >= -coord.w;
        case POSITIVE_Z:
            return coord.z <= +coord.w;
        case NEGATIVE_Z:
            return coord.z >= -coord.w;
        default:
            assert(0);
            return 0;
    }
}

static float get_intersect_ratio(const glm::vec4& prev, const glm::vec4& curr, const PlaneType& plane) {
    switch (plane) {
        case POSITIVE_W:
            return (prev.w - EPSILON) / (prev.w - curr.w);
        case POSITIVE_X:
            return (prev.w - prev.x) / ((prev.w - prev.x) - (curr.w - curr.x));
        case NEGATIVE_X:
            return (prev.w + prev.x) / ((prev.w + prev.x) - (curr.w + curr.x));
        case POSITIVE_Y:
            return (prev.w - prev.y) / ((prev.w - prev.y) - (curr.w - curr.y));
        case NEGATIVE_Y:
            return (prev.w + prev.y) / ((prev.w + prev.y) - (curr.w + curr.y));
        case POSITIVE_Z:
            return (prev.w - prev.z) / ((prev.w - prev.z) - (curr.w - curr.z));
        case NEGATIVE_Z:
            return (prev.w + prev.z) / ((prev.w + prev.z) - (curr.w + curr.z));
        default:
            assert(0);
            return 0;
    }
}

static int clip_against_plane(
		const PlaneType& plane, int in_num_vertices, int fs_in_num_floats,
        std::vector<glm::vec4>& vs_in_clip_coords, std::vector<void*>& fs_in_clip_data,
        std::vector<glm::vec4>& vs_out_clip_coords, std::vector<void*>& fs_out_clip_data) {
    int out_num_vertices = 0;
    assert(in_num_vertices >= 3);
    for (int i = 0; i < in_num_vertices; i++) {
        int prev_index = (i - 1 + in_num_vertices) % in_num_vertices;
        int curr_index = i;
        auto prev_coord = vs_in_clip_coords[prev_index];
        auto curr_coord = vs_in_clip_coords[curr_index];
        float *prev_clip_data = (float*)fs_in_clip_data[prev_index];
        float *curr_clip_data = (float*)fs_in_clip_data[curr_index];
        int prev_inside = is_inside_plane(prev_coord, plane);
        int curr_inside = is_inside_plane(curr_coord, plane);

        if (prev_inside != curr_inside) {
            auto *dest_clip_coord = &vs_out_clip_coords[out_num_vertices];
            float *dest_clip_data = (float*)fs_out_clip_data[out_num_vertices];
            float ratio = get_intersect_ratio(prev_coord, curr_coord, plane);

            *dest_clip_coord = vec4_lerp(prev_coord, curr_coord, ratio);
            /*
             * since this computation is performed in clip space before
             * division by w, clipped varying values are perspective-correct
             */
            for (int j = 0; j < fs_in_num_floats; j++) {
                dest_clip_data[j] = float_lerp(
                    prev_clip_data[j], curr_clip_data[j],ratio);
            }
            out_num_vertices += 1;
        }

        if (curr_inside) {
            auto *dest_clip_coord = &vs_out_clip_coords[out_num_vertices];
            float *dest_clip_data = (float*)fs_out_clip_data[out_num_vertices];
            int sizeof_fs_in = fs_in_num_floats * sizeof(float);

            *dest_clip_coord = curr_coord;
            memcpy(dest_clip_data, curr_clip_data, sizeof_fs_in);
            out_num_vertices += 1;
        }
    }
    return out_num_vertices;
}

#define CLIP_IN2OUT(plane)                                                                      \
    do {                                                                                        \
        num_vertices = clip_against_plane(                                                      \
            plane, num_vertices, fs_in_num_floats,                                            \
            vs_in_clip_coords, fs_in_clip_data, vs_out_clip_coords, fs_out_clip_data);          \
        if (num_vertices < 3) {                                                                 \
            return 0;                                                                           \
        }                                                                                       \
    } while (0)

#define CLIP_OUT2IN(plane)                                                                      \
    do {                                                                                        \
        num_vertices = clip_against_plane(                                                      \
            plane, num_vertices, fs_in_num_floats,                                            \
            vs_out_clip_coords, fs_out_clip_data, vs_in_clip_coords, fs_in_clip_data);          \
        if (num_vertices < 3) {                                                                 \
            return 0;                                                                           \
        }                                                                                       \
    } while (0)

static bool is_vertex_visible(glm::vec4& v) {
    return fabs(v.x) <= v.w && fabs(v.y) <= v.w && fabs(v.z) <= v.w;
}

static int clip_triangle(int sizeof_fs_in,
        std::vector<glm::vec4>& vs_in_clip_coords, std::vector<void*>& fs_in_clip_data,
        std::vector<glm::vec4>& vs_out_clip_coords, std::vector<void*>& fs_out_clip_data){
    auto v0_visible = is_vertex_visible(vs_in_clip_coords[0]);
    auto v1_visible = is_vertex_visible(vs_in_clip_coords[1]);
    auto v2_visible = is_vertex_visible(vs_in_clip_coords[2]);
    if (v0_visible && v1_visible && v2_visible) {
        vs_out_clip_coords[0] = vs_in_clip_coords[0];
        vs_out_clip_coords[1] = vs_in_clip_coords[1];
        vs_out_clip_coords[2] = vs_in_clip_coords[2];
        memcpy(fs_out_clip_data[0], fs_in_clip_data[0], sizeof_fs_in);
        memcpy(fs_out_clip_data[1], fs_in_clip_data[1], sizeof_fs_in);
        memcpy(fs_out_clip_data[2], fs_in_clip_data[2], sizeof_fs_in);
        return 3;
    } else {
        int fs_in_num_floats = sizeof_fs_in / sizeof(float);
        int num_vertices = 3;
        CLIP_IN2OUT(POSITIVE_W);
        CLIP_OUT2IN(POSITIVE_X);
        CLIP_IN2OUT(NEGATIVE_X);
        CLIP_OUT2IN(POSITIVE_Y);
        CLIP_IN2OUT(NEGATIVE_Y);
        CLIP_OUT2IN(POSITIVE_Z);
        CLIP_IN2OUT(NEGATIVE_Z);
        return num_vertices;
    }
}


/*
 * for facing determination, see subsection 3.5.1 of
 * https://www.khronos.org/registry/OpenGL/specs/es/2.0/es_full_spec_2.0.pdf
 *
 * this is the same as (but more efficient than)
 *     vec3 ab = vec3_sub(b, a);
 *     vec3 ac = vec3_sub(c, a);
 *     return vec3_cross(ab, ac).z <= 0;
 */
static int is_back_facing(const std::vector<glm::vec3>& ndc_coords) {
    auto a = ndc_coords[0];
    auto b = ndc_coords[1];
    auto c = ndc_coords[2];
    float signed_area = a.x * b.y - a.y * b.x +
                        b.x * c.y - b.y * c.x +
                        c.x * a.y - c.y * a.x;
    return signed_area <= 0;
}

/*
 * for viewport transformation, see subsection 2.12.1 of
 * https://www.khronos.org/registry/OpenGL/specs/es/2.0/es_full_spec_2.0.pdf
 */
static glm::vec3 viewport_transform(int width, int height, glm::vec3& ndc_coord) {
    float x = (ndc_coord.x + 1) * 0.5f * (float)width;   /* [-1, 1] -> [0, w] */
    float y = (ndc_coord.y + 1) * 0.5f * (float)height;  /* [-1, 1] -> [0, h] */
    float z = (ndc_coord.z + 1) * 0.5f;                  /* [-1, 1] -> [0, 1] */
    return glm::vec3(x, y, z);
}

typedef struct {int min_x, min_y, max_x, max_y;} BBox;

static BBox find_bounding_box(std::vector<glm::vec2>& abc, int width, int height) {
    auto min = vec2_min(abc[0], abc[1]);
    auto min_min = vec2_min(min, abc[2]);
    auto max = vec2_max(abc[0], abc[1]);
    auto max_max = vec2_max(max, abc[2]);
    BBox bbox;
    bbox.min_x = max_integer((int)ceil(min_min.x), 0);
    bbox.min_y = max_integer((int)ceil(min_min.y), 0);
    bbox.max_x = min_integer((int)floor(max_max.x), width - 1);
    bbox.max_y = min_integer((int)floor(max_max.y), height - 1);
    return bbox;
}

/*
 * for barycentric coordinates, see
 * http://blackpawn.com/texts/pointinpoly/
 *
 * solve
 *     P = A + s * AB + t * AC  -->  AP = s * AB + t * AC
 * then
 *     s = (AC.y * AP.x - AC.x * AP.y) / (AB.x * AC.y - AB.y * AC.x)
 *     t = (AB.x * AP.y - AB.y * AP.x) / (AB.x * AC.y - AB.y * AC.x)
 *
 * notice
 *     P = A + s * AB + t * AC
 *       = A + s * (B - A) + t * (C - A)
 *       = (1 - s - t) * A + s * B + t * C
 * then
 *     weight_A = 1 - s - t
 *     weight_B = s
 *     weight_C = t
 */
static glm::vec3 calculate_weights(std::vector<glm::vec2>& abc, glm::vec2& p) {
   auto a = abc[0];
   auto b = abc[1];
   auto c = abc[2];
   auto ab = b - a;
   auto ac = c - a;
   auto ap = p - a;
    float factor = 1 / (ab.x * ac.y - ab.y * ac.x);
    float s = (ac.y * ap.x - ac.x * ap.y) * factor;
    float t = (ab.x * ap.y - ab.y * ap.x) * factor;
    auto weights = glm::vec3(1 - s - t, s, t);
    return weights;
}

/*
 * for depth interpolation, see subsection 3.5.1 of
 * https://www.khronos.org/registry/OpenGL/specs/es/2.0/es_full_spec_2.0.pdf
 */
static float interpolate_depth(float screen_depths[3], glm::vec3& weights) {
    float depth0 = screen_depths[0] * weights.x;
    float depth1 = screen_depths[1] * weights.y;
    float depth2 = screen_depths[2] * weights.z;
    return depth0 + depth1 + depth2;
}

/*
 * for perspective correct interpolation, see
 * https://www.comp.nus.edu.sg/~lowkl/publications/lowk_persp_interp_techrep.pdf
 * https://www.khronos.org/registry/OpenGL/specs/es/2.0/es_full_spec_2.0.pdf
 *
 * equation 15 in reference 1 (page 2) is a simplified 2d version of
 * equation 3.5 in reference 2 (page 58) which uses barycentric coordinates
 */
static void interpolate_clip_data(
        void *src_clip_data[3], void *dst_clip_data,
        int sizeof_fs_in, glm::vec3& weights, float recip_w[3]) {
    int num_floats = sizeof_fs_in / sizeof(float);
    float *src0 = (float*)src_clip_data[0];
    float *src1 = (float*)src_clip_data[1];
    float *src2 = (float*)src_clip_data[2];
    float *dst = (float*)dst_clip_data;
    float weight0 = recip_w[0] * weights.x;
    float weight1 = recip_w[1] * weights.y;
    float weight2 = recip_w[2] * weights.z;
    float normalizer = 1 / (weight0 + weight1 + weight2);
    for (int i = 0; i < num_floats; i++) {
        float sum = src0[i] * weight0 + src1[i] * weight1 + src2[i] * weight2;
        dst[i] = sum * normalizer;
    }
}

static void draw_fragment(FrameBuffer *framebuffer, Program *program,
                          int backface, int index, float depth) {
    glm::vec4 color;
    int discard;

    /* execute fragment shader */
    discard = 0;
    color = program->shader->ExecuteFragmentShader(
        program->fs_in, program->uniforms, &discard, backface);
    if (discard) {
        return;
    }
    color = vec4_saturate(color);

    /* perform blending */
    if (program->enable_blend) {
        /* out_color = src_color * src_alpha + dst_color * (1 - src_alpha) */
        unsigned char dst_r = framebuffer->m_ColorBuffer[index * 4 + 0];
        unsigned char dst_g = framebuffer->m_ColorBuffer[index * 4 + 1];
        unsigned char dst_b = framebuffer->m_ColorBuffer[index * 4 + 2];
        color.x = color.x * color.w + float_from_uchar(dst_r) * (1 - color.w);
        color.y = color.y * color.w + float_from_uchar(dst_g) * (1 - color.w);
        color.z = color.z * color.w + float_from_uchar(dst_b) * (1 - color.w);
    }

    /* write color and depth */
    framebuffer->m_ColorBuffer[index * 4 + 0] = float_to_uchar(color.x);
    framebuffer->m_ColorBuffer[index * 4 + 1] = float_to_uchar(color.y);
    framebuffer->m_ColorBuffer[index * 4 + 2] = float_to_uchar(color.z);
    framebuffer->m_ColorBuffer[index * 4 + 3] = float_to_uchar(color.w);
    framebuffer->m_DepthBuffer[index] = depth;
}

static int rasterize_triangle(FrameBuffer *framebuffer, Program *program,
                              glm::vec4 clip_coords[3], void *clip_data[3]) {
    int width = framebuffer->m_Width;
    int height = framebuffer->m_Height;
    std::vector<glm::vec3> ndc_coords(3);
    std::vector<glm::vec2> screen_coords(3);
    float screen_depths[3];
    float recip_w[3];
    int backface;
    BBox bbox;

    /* perspective division */
    for (int i = 0; i < 3; i++) {
        auto clip_coord = glm::vec3(clip_coords[i]);
        ndc_coords[i] = vec3_div(clip_coord, clip_coords[i].w);
    }

    /* back-face culling */
    backface = is_back_facing(ndc_coords);
    if (backface && !program->double_sided) {
        return 1;
    }

    /* reciprocals of w */
    for (int i = 0; i < 3; i++) {
        recip_w[i] = 1 / clip_coords[i].w;
    }

    /* viewport mapping */
    for (int i = 0; i < 3; i++) {
        auto window_coord = viewport_transform(width, height, ndc_coords[i]);
        screen_coords[i] = glm::vec2(window_coord.x, window_coord.y);
        screen_depths[i] = window_coord.z;
    }

    /* perform rasterization */
    bbox = find_bounding_box(screen_coords, width, height);
    for (int x = bbox.min_x; x <= bbox.max_x; x++) {
        for (int y = bbox.min_y; y <= bbox.max_y; y++) {
            auto point = glm::vec2((float)x, (float)y);
            auto weights = calculate_weights(screen_coords, point);
            int weight0_okay = weights.x > -EPSILON;
            int weight1_okay = weights.y > -EPSILON;
            int weight2_okay = weights.z > -EPSILON;
            if (weight0_okay && weight1_okay && weight2_okay) {
                int index = y * width + x;
                float depth = interpolate_depth(screen_depths, weights);
                /* early depth testing */
                if (depth <= framebuffer->m_DepthBuffer[index]) {
                    interpolate_clip_data(
                        clip_data, program->fs_in, program->sizeof_fs_in, weights, recip_w);
                    draw_fragment(framebuffer, program, backface, index, depth);
                }
            }
        }
    }

    return 0;
}


void graphics_draw_triangle(FrameBuffer *framebuffer, Program *program) {
     /* execute vertex shader */
    for (int i = 0; i < 3; i++) {
        auto clip_coord = program->shader->ExecuteVertexShader(
            program->vs_in[i],
            program->fs_in_clip_data[i],
            program->uniforms
        );
        program->vs_in_clip_coords[i] = clip_coord;
    }

    /* triangle clipping */
    auto num_vertices = clip_triangle(
        program->sizeof_fs_in,
        program->vs_in_clip_coords,
        program->fs_in_clip_data,
        program->vs_out_clip_coords,
        program->fs_out_clip_data
    );

    /* triangle assembly */
    for (int i = 0; i < num_vertices - 2; i++) {
        int index0 = 0;
        int index1 = i + 1;
        int index2 = i + 2;
        glm::vec4 clip_coords[3];
        void *clip_data[3];
        int is_culled;

        clip_coords[0] = program->vs_out_clip_coords[index0];
        clip_coords[1] = program->vs_out_clip_coords[index1];
        clip_coords[2] = program->vs_out_clip_coords[index2];
        clip_data[0] = program->fs_out_clip_data[index0];
        clip_data[1] = program->fs_out_clip_data[index1];
        clip_data[2] = program->fs_out_clip_data[index2];

        is_culled = rasterize_triangle(framebuffer, program,
                                       clip_coords, clip_data);
        if (is_culled) {
            break;
        }
    }
}
