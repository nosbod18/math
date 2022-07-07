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

#define MATH_DEFINE_COMMON(T, n)\
static inline void T##n##_copy(T out[n], T a[n])        { for (int i = 0; i < n; i++) out[i] = a[i];        }\
static inline void T##n##_add(T out[n], T a[n], T b[n]) { for (int i = 0; i < n; i++) out[i] = a[i] + b[i]; }\
static inline void T##n##_sub(T out[n], T a[n], T b[n]) { for (int i = 0; i < n; i++) out[i] = a[i] - b[i]; }\
static inline void T##n##_mul(T out[n], T a[n], T b[n]) { for (int i = 0; i < n; i++) out[i] = a[i] * b[i]; }

#define MATH_DEFINE_VECTOR(T, n)\
static inline void T##n##_scale(T out[n], T a[n], T b)  { for (int i = 0; i < n; i++) out[i] = a[i] * b; }\
static inline T    T##n##_dot(T a[n], T b[n])           { float d = 0.f; for (int i = 0; i < n; i++) d += a[i] * b[i]; return d; }\
static inline T    T##n##_len2(T a[n])                  { return T##n##_dot(a, a); }\
static inline T    T##n##_len(T a[n])                   { return fast_sqrtf(T##n##_len2(a)); }\
static inline void T##n##_norm(T out[n], T a[n])        { T##n##_scale(out, a, 1.f / T##n##_len(a)); }

#define MATH_DEFINE_VECTOR_SPECIFIC(T)\
static inline void T##2_rotate(T out[2], T a[2], T angle) {\
    T c = fast_cosf(angle), s = fast_sinf(angle);\
    out[0] = a[0] * c - a[1] * s;\
    out[1] = a[0] * s + a[1] * c;\
}\
static inline void T##3_cross(T out[3], T a[3], T b[3]) {\
    out[0] = a[1] * b[2] - a[2] * b[1];\
    out[1] = a[2] * b[0] - a[0] * b[2];\
    out[2] = a[0] * b[1] - a[1] * b[0];\
}

#define MATH_DEFINE_MATRIX_SPEICIFIC(T)\
static inline void T##9_identity(T out[9]) {\
    T##9_copy(out, (T [9]){[0] = 1.f, [4] = 1.f, [8] = 1.f});\
}\
static inline void T##9_translate(T out[9], T translation[2]) {\
    T##9_identity(out);\
    out[6] = translation[0];\
    out[7] = translation[1];\
}\
static inline void T##9_rotate(T out[9], T angle) {\
    T s = fast_sinf(angle), c = fast_cosf(angle);\
    T##9_copy(out, (T [9]){[0] = c, [1] = -s, [3] = s, [4] = c});\
}\
static inline void T##9(T out[9], T scale[3]) {\
    T##9_identity(out);\
    out[0] = scale[0];\
    out[4] = scale[1];\
}\
static inline void T##16_identity(T out[16]) {\
    T##16_copy(out, (T [16]){[0] = 1.f, [5] = 1.f, [10] = 1.f, [15] = 1.f});\
}\
static inline void T##16_translate(T out[16], T translation[3]) {\
    T##16_identity(out);\
    out[12] = translation[0];\
    out[13] = translation[1];\
    out[14] = translation[2];\
}\
static inline void T##16_rotate(T out[16], T axis[3], T angle) {\
    T c = fast_cosf(angle);\
    T an[3], at[3], as[3];\
    T##3_norm(an, axis);\
    T##3_scale(at, an, 1.f - c);\
    T##3_scale(as, an, fast_sinf(angle));\
    T##16_copy(out, (T [16]){\
        an[0] * at[0] + c,     an[1] * at[0] + as[2], an[2] * at[0] - as[1], 0.f,\
        an[0] * at[1] - as[2], an[1] * at[1] + c,     an[2] * at[1] + as[0], 0.f,\
        an[0] * at[2] + as[2], an[1] * at[2] + as[0], an[2] * at[2] + c,     0.f,\
        0.f,                   0.f,                   0.f,                   1.f\
    });\
}\
static inline void T##16_scale(T out[16], T scale[3]) {\
    T##16_identity(out);\
    out[ 0] = scale[0];\
    out[ 5] = scale[1];\
    out[10] = scale[2];\
}\
static inline void T##16_lookat(T out[16], T eye[3], T target[3], T up[3]) {\
    T f[3], r[3], u[3];\
    T##3_sub(f, target, eye);\
    T##3_norm(f, f);\
    T##3_cross(r, f, up);\
    T##3_norm(r, r);\
    T##3_cross(u, r, f);\
    T##16_copy(out, (T [16]){\
        r[0],                 u[0],               -f[0],               0.0f,\
        r[1],                 u[1],               -f[1],               0.0f,\
        r[2],                 u[2],               -f[2],               0.0f,\
        -T##3_dot(r, eye), -T##3_dot(u, eye), -T##3_dot(f, eye), 1.0f\
    });\
}\
static inline void T##16_perspective(T out[16], T fov, T aspect, T near, T far) {\
    T nf  = 1.f / (near - far);\
    fov = 1.f / fast_tanf(fov * 0.5f);\
    T##16_copy(out, (T [16]){\
        [ 0] = fov / aspect,\
        [ 5] = fov,\
        [10] = (near + far) * nf,\
        [11] = -1.f,\
        [15] = 2.f * near * far * nf\
    });\
}\
static inline void T##16_ortho(T out[16], T bottom, T top, T left, T right, T near, T far) {\
    T rl =  1.f / (right - bottom);\
    T tb =  1.f / (top - bottom);\
    T fn = -1.f / (far - near);\
    T##16_copy(out, (T [16]){\
        [ 0] = 2.f * rl,\
        [ 5] = 2.f * tb,\
        [10] = 2.f * fn,\
        [12] = -(right + left) * rl,\
        [13] = -(top + bottom) * tb,\
        [14] = -(far + near) * fn,\
        [15] = 1.f\
    });\
}\

MATH_DEFINE_COMMON(int,     2)
MATH_DEFINE_COMMON(float,   2)
MATH_DEFINE_COMMON(float,   3)
MATH_DEFINE_COMMON(float,   4)
MATH_DEFINE_COMMON(float,   9)
MATH_DEFINE_COMMON(float,  16)
MATH_DEFINE_COMMON(double,  2)
MATH_DEFINE_COMMON(double,  3)
MATH_DEFINE_COMMON(double,  4)
MATH_DEFINE_COMMON(double,  9)
MATH_DEFINE_COMMON(double, 16)

MATH_DEFINE_VECTOR(int,     2)
MATH_DEFINE_VECTOR(float,   2)
MATH_DEFINE_VECTOR(float,   3)
MATH_DEFINE_VECTOR(float,   4)
MATH_DEFINE_VECTOR(double,  2)
MATH_DEFINE_VECTOR(double,  3)
MATH_DEFINE_VECTOR(double,  4)

MATH_DEFINE_VECTOR_SPECIFIC(float)
MATH_DEFINE_VECTOR_SPECIFIC(double)

MATH_DEFINE_MATRIX_SPEICIFIC(float)
MATH_DEFINE_MATRIX_SPEICIFIC(double)
