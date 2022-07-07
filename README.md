# math - Minimal math library

`math.h` is a very minimal vector and matrix math library that only defines the essential functions for
computer graphics and games.

Since this branch uses no types, `smath.h` is not provided.

## API
```c
static inline float fast_sinf           (float x);
static inline float fast_cosf           (float x);
static inline float fast_tanf           (float x);
static inline float fast_sqrtf          (float x);

// Common
static inline void  vecN_copy           (T out[N], T a[N]);
static inline void  vecN_add            (T out[N], T a[N], T b[N]);
static inline void  vecN_sub            (T out[N], T a[N], T b[N]);
static inline void  vecN_mul            (T out[N], T a[N], T b[N]);

// Vector
static inline void  vecN_scale          (T out[N], T a[N], T b);
static inline T     vecN_dot            (T a[N], T b[N]);
static inline T     vecN_len2           (T a[N]);
static inline T     vecN_len            (T a[N]);
static inline void  vecN_norm           (T out[N], T a[N]);

// Vector specific
static inline void  vec3_cross          (T out[3], T a[3], T b[3]);

// Matrix specific
static inline void  mat4_copy           (T out[4][4], T a[4][4]);
static inline void  mat4_identity       (T out[4][4]);
static inline void  mat4_mul            (T out[4][4], T a[4][4], T b[4][4]);
static inline void  mat4_translate      (T out[4][4], T translation[3]);
static inline void  mat4_rotate         (T out[4][4], T axis[3], T angle);
static inline void  mat4_scale          (T out[4][4], T scale[3]);
static inline void  mat4_lookat         (T out[4][4], T eye[3], T target[3], T up[3]);
static inline void  mat4_perspective    (T out[4][4], float fov, float aspect, float near, float far);
static inline void  mat4_ortho          (T out[4][4], float bottom, float top, float left, float right, float near, float far);
```
