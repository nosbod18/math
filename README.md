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
static inline void  TN_copy             (T out[N], T a[N]);
static inline void  TN_add              (T out[N], T a[N], T b[N]);
static inline void  TN_sub              (T out[N], T a[N], T b[N]);
static inline void  TN_mul              (T out[N], T a[N], T b[N]);

// Vector
static inline void  TN_scale            (T out[N], T a[N], T b);
static inline T     TN_dot              (T a[N], T b[N]);
static inline T     TN_len2             (T a[N]);
static inline T     TN_len              (T a[N]);
static inline void  TN_norm             (T out[N], T a[N]);

// Vector specific
static inline void  T3_cross            (T out[3], T a[3], T b[3]);

// Matrix specific
static inline void  T9_copy             (T out[9], T a[9]);
static inline void  T9_identity         (T out[9]);
static inline void  T9_mul              (T out[9], T a[9], T b[9]);
static inline void  T9_translate        (T out[9], T translation[3]);
static inline void  T9_rotate           (T out[9], T axis[3], T angle);
static inline void  T9_scale            (T out[9], T scale[3]);
static inline void  T16_copy            (T out[16], T a[16]);
static inline void  T16_identity        (T out[16]);
static inline void  T16_mul             (T out[16], T a[16], T b[16]);
static inline void  T16_translate       (T out[16], T translation[3]);
static inline void  T16_rotate          (T out[16], T axis[3], T angle);
static inline void  T16_scale           (T out[16], T scale[3]);
static inline void  T16_lookat          (T out[16], T eye[3], T target[3], T up[3]);
static inline void  T16_perspective     (T out[16], float fov, float aspect, float near, float far);
static inline void  T16_ortho           (T out[16], float bottom, float top, float left, float right, float near, float far);
```
