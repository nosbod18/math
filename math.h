#pragma once

static inline float fast_sinf(float x) {
    x *= 0.15915494309189534f;
    x *= 0.5f - (x * ((x > 0) - (x < 0)));
    return x * (57.3460872862336f * (x * ((x > 0) - (x < 0))) + 12.4158695446104f);
}
static inline float fast_cosf(float x) {
    return fast_sinf(x + 1.570796327f);
}
static inline float fast_tanf(float x) {
    return fast_sinf(x) / fast_cosf(x);
}
static inline float fast_sqrtf(float x) {
    int y = ((*(int *)&x - 0x00800000) >> 1) + 0x20000000;
    float z = *(float *)&y;
    return (x / z + z) * 0.5f;
}

#define MATH_DEFINE_VECTOR(n)\
static inline void  vec##n##_copy(float out[n], float a[n])             { for (int i = 0; i < n; i++) out[i] = a[i];        }\
static inline void  vec##n##_add(float out[n], float a[n], float b[n])  { for (int i = 0; i < n; i++) out[i] = a[i] + b[i]; }\
static inline void  vec##n##_sub(float out[n], float a[n], float b[n])  { for (int i = 0; i < n; i++) out[i] = a[i] - b[i]; }\
static inline void  vec##n##_mul(float out[n], float a[n], float b[n])  { for (int i = 0; i < n; i++) out[i] = a[i] * b[i]; }\
static inline void  vec##n##_scale(float out[n], float a[n], float b)   { for (int i = 0; i < n; i++) out[i] = a[i] * b;    }\
static inline float vec##n##_dot(float a[n], float b[n])                { float d = 0.f; for (int i = 0; i < n; i++) d += a[i] * b[i]; return d; }\
static inline float vec##n##_len2(float a[n])                           { return vec##n##_dot(a, a); }\
static inline float vec##n##_len(float a[n])                            { return fast_sqrtf(vec##n##_len2(a)); }\
static inline void  vec##n##_norm(float out[n], float a[n])             { vec##n##_scale(out, a, 1.f / vec##n##_len(a)); }

MATH_DEFINE_VECTOR(2)
MATH_DEFINE_VECTOR(3)
MATH_DEFINE_VECTOR(4)

static inline void vec3_cross(float out[3], float a[3], float b[3]) {
    out[0] = a[1] * b[2] - a[2] * b[1];
    out[1] = a[2] * b[0] - a[0] * b[2];
    out[2] = a[0] * b[1] - a[1] * b[0];
}
static inline void mat4_copy(float out[4][4], float a[4][4]) {
    for (int i = 0; i < 4; i++)
        vec4_copy(out[i], a[i]);
}
static inline void mat4_identity(float out[4][4]) {
    mat4_copy(out, (float [4][4]){[0][0] = 1.f, [1][1] = 1.f, [2][2] = 1.f, [3][3] = 1.f});
}
static inline void mat4_mul(float out[4][4], float a[4][4], float b[4][4]) {
    for (int i = 0; i < 4; i++)
        vec4_mul(out[i], a[i], b[i]);
}
static inline void mat4_translate(float out[4][4], float translation[3]) {
    mat4_identity(out);
    out[3][0] = translation[0];
    out[3][1] = translation[1];
    out[3][2] = translation[2];
}
static inline void mat4_rotate(float out[4][4], float axis[3], float angle) {
    float c = fast_cosf(angle);
    float an[3], at[3], as[3];
    vec3_norm(an, axis);
    vec3_scale(at, an, 1.f - c);
    vec3_scale(as, an, fast_sinf(angle));
    mat4_copy(out, (float [4][4]){
        an[0] * at[0] + c,     an[1] * at[0] + as[2], an[2] * at[0] - as[1], 0.f,
        an[0] * at[1] - as[2], an[1] * at[1] + c,     an[2] * at[1] + as[0], 0.f,
        an[0] * at[2] + as[2], an[1] * at[2] + as[0], an[2] * at[2] + c,     0.f,
        0.f,                   0.f,                   0.f,                   1.f
    });
}
static inline void mat4_scale(float out[4][4], float scale[3]) {
    mat4_identity(out);
    out[0][0] = scale[0];
    out[1][1] = scale[1];
    out[2][2] = scale[2];
}
static inline void mat4_lookat(float out[4][4], float eye[3], float target[3], float up[3]) {
    float f[3], r[3], u[3];
    vec3_sub(f, target, eye);
    vec3_norm(f, f);
    vec3_cross(r, f, up);
    vec3_norm(r, r);
    vec3_cross(u, r, f);
    mat4_copy(out, (float [4][4]){
        {r[0], u[0], -f[0], 0.0f},
        {r[1], u[1], -f[1], 0.0f},
        {r[2], u[2], -f[2], 0.0f},
        {-vec3_dot(r, eye), -vec3_dot(u, eye), -vec3_dot(f, eye), 1.0f}
    });
}
static inline void mat4_perspective(float out[4][4], float fov, float aspect, float near, float far) {
    float nf  = 1.f / (near - far);
    fov = 1.f / fast_tanf(fov * 0.5f);
    mat4_copy(out, (float [4][4]){
        [0][0] = fov / aspect,
        [1][1] = fov,
        [2][2] = (near + far) * nf,
        [2][3] = -1.f,
        [3][2] = 2.f * near * far * nf
    });
}
static inline void mat4_ortho(float out[4][4], float bottom, float top, float left, float right, float near, float far) {
    float rl =  1.f / (right - bottom);
    float tb =  1.f / (top - bottom);
    float fn = -1.f / (far - near);
    mat4_copy(out, (float [4][4]){
        [0][0] = 2.f * rl,
        [1][1] = 2.f * tb,
        [2][2] = 2.f * fn,
        [3][0] = -(right + left) * rl,
        [3][1] = -(top + bottom) * tb,
        [3][2] = -(far + near) * fn,
        [3][3] = 1.f
    });
}
