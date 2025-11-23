#ifndef SIMD_H
#define SIMD_H

// == arch detection ==
// x86/x64: SSE2
#if defined(__x86_64__) || defined(__SSE2__) || defined(_M_X64) || \
    defined(_M_IX86) || defined(_M_AMD64) || defined(__i386__)
#    define ARCH_X86
#    define DEFINE_SIMD__SSE
#    include <emmintrin.h>

// ARM: NEON (AArch64 or ARMv7+NEON)
#elif defined(__aarch64__) || defined(__ARM_NEON) || defined(__ARM_NEON__) || \
    defined(_M_ARM64) ||                                                      \
    (defined(_M_ARM) && (defined(__ARM_NEON) || defined(__ARM_NEON__)))
#    define ARCH_ARM
#    define DEFINE_SIMD__NEON
#    include <arm_neon.h>
#else
#    define DISABLE_SIMD
#endif

// == simd setup ==
#if defined(ENABLE_SIMD)
#    if defined(ARCH_X86)
#        define DEFINE_SIMD__SSE
#        include <emmintrin.h>
#    elif defined(ARCH_ARM)
#        define DEFINE_SIMD__NEON
#        include <arm_neon.h>
#    endif
#endif

#if defined(_MSC_VER)
#    define align16 __declspec(align(16))
#else
#    define align16 __attribute__((aligned(16)))
#endif

// == SIMD abstraction layer ==
#if defined(DEFINE_SIMD__SSE)
typedef __m128 simd_f32x4_t;

#    define simd_load_f32(ptr) _mm_load_ps(ptr)
#    define simd_store_f32(ptr, val) _mm_store_ps(ptr, val)
#    define simd_set1_f32(val) _mm_set1_ps(val)
#    define simd_add_f32(a, b) _mm_add_ps(a, b)
#    define simd_sub_f32(a, b) _mm_sub_ps(a, b)
#    define simd_mul_f32(a, b) _mm_mul_ps(a, b)
#    define simd_div_f32(a, b) _mm_div_ps(a, b)
#    define simd_and_f32(a, b) _mm_and_ps(a, b)
#    define simd_or_f32(a, b) _mm_or_ps(a, b)
#    define simd_xor_f32(a, b) _mm_xor_ps(a, b)
#    define simd_negate_f32(a) _mm_xor_ps(a, _mm_set1_ps(-0.0f))

// Shuffle operations
#    define simd_shuffle_f32(a, imm) _mm_shuffle_ps(a, a, imm)
#    define simd_shuffle2_f32(a, b, imm) _mm_shuffle_ps(a, b, imm)
#    define SIMD_SHUFFLE(z, y, x, w) _MM_SHUFFLE(z, y, x, w)

// Extract/insert
#    define simd_extract0_f32(a) _mm_cvtss_f32(a)
#    define simd_extract1_f32(a) \
        _mm_cvtss_f32(_mm_shuffle_ps(a, a, _MM_SHUFFLE(1, 1, 1, 1)))
#    define simd_extract2_f32(a) \
        _mm_cvtss_f32(_mm_shuffle_ps(a, a, _MM_SHUFFLE(2, 2, 2, 2)))
#    define simd_extract3_f32(a) \
        _mm_cvtss_f32(_mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 3, 3, 3)))

// Fused multiply-add (emulated for SSE)
#    define simd_fmadd_f32(a, b, c) _mm_add_ps(_mm_mul_ps(a, b), c)

#elif defined(DEFINE_SIMD__NEON)
typedef float32x4_t simd_f32x4_t;

#    define simd_load_f32(ptr) vld1q_f32(ptr)
#    define simd_store_f32(ptr, val) vst1q_f32(ptr, val)
#    define simd_set1_f32(val) vdupq_n_f32(val)
#    define simd_add_f32(a, b) vaddq_f32(a, b)
#    define simd_sub_f32(a, b) vsubq_f32(a, b)
#    define simd_mul_f32(a, b) vmulq_f32(a, b)
#    define simd_div_f32(a, b) vdivq_f32(a, b)
#    define simd_and_f32(a, b) \
        vreinterpretq_f32_u32( \
            vandq_u32(vreinterpretq_u32_f32(a), vreinterpretq_u32_f32(b)))
#    define simd_or_f32(a, b)  \
        vreinterpretq_f32_u32( \
            vorrq_u32(vreinterpretq_u32_f32(a), vreinterpretq_u32_f32(b)))
#    define simd_xor_f32(a, b) \
        vreinterpretq_f32_u32( \
            veorq_u32(vreinterpretq_u32_f32(a), vreinterpretq_u32_f32(b)))
#    define simd_negate_f32(a)                  \
        vreinterpretq_f32_u32(                  \
            veorq_u32(vreinterpretq_u32_f32(a), \
                      vreinterpretq_u32_f32(vdupq_n_f32(-0.0f))))

// Lane operations
#    define simd_dup_lane_f32(a, lane) vdupq_n_f32(vgetq_lane_f32(a, lane))
#    define simd_mul_lane_f32(a, b, lane) vmulq_lane_f32(a, b, lane)
#    define simd_mla_lane_f32(a, b, c, lane) vmlaq_lane_f32(a, b, c, lane)

// Extract low/high
#    define simd_get_low_f32(a) vget_low_f32(a)
#    define simd_get_high_f32(a) vget_high_f32(a)

// Fused multiply-add (native in NEON)
#    define simd_fmadd_f32(a, b, c) vmlaq_f32(c, a, b)

// Shuffle equivalent for NEON (limited)
#    define SIMD_SHUFFLE(z, y, x, w) ((z) << 6 | (y) << 4 | (x) << 2 | (w))

#else
// Scalar fallback
typedef struct {
    float f[4];
} simd_f32x4_t;

#    define simd_load_f32(ptr)                         \
        (simd_f32x4_t) {                               \
            {                                          \
                (ptr)[0], (ptr)[1], (ptr)[2], (ptr)[3] \
            }                                          \
        }
#    define simd_store_f32(ptr, val) \
        do {                         \
            (ptr)[0] = (val).f[0];   \
            (ptr)[1] = (val).f[1];   \
            (ptr)[2] = (val).f[2];   \
            (ptr)[3] = (val).f[3];   \
        } while (0)
#    define simd_set1_f32(val)             \
        (simd_f32x4_t) {                   \
            {                              \
                (val), (val), (val), (val) \
            }                              \
        }
#    define simd_add_f32(a, b)                                                 \
        (simd_f32x4_t) {                                                       \
            {                                                                  \
                (a).f[0] + (b).f[0], (a).f[1] + (b).f[1], (a).f[2] + (b).f[2], \
                    (a).f[3] + (b).f[3]                                        \
            }                                                                  \
        }
#    define simd_sub_f32(a, b)                                                 \
        (simd_f32x4_t) {                                                       \
            {                                                                  \
                (a).f[0] - (b).f[0], (a).f[1] - (b).f[1], (a).f[2] - (b).f[2], \
                    (a).f[3] - (b).f[3]                                        \
            }                                                                  \
        }
#    define simd_mul_f32(a, b)                                                 \
        (simd_f32x4_t) {                                                       \
            {                                                                  \
                (a).f[0] * (b).f[0], (a).f[1] * (b).f[1], (a).f[2] * (b).f[2], \
                    (a).f[3] * (b).f[3]                                        \
            }                                                                  \
        }
#    define simd_negate_f32(a)                             \
        (simd_f32x4_t) {                                   \
            {                                              \
                -(a).f[0], -(a).f[1], -(a).f[2], -(a).f[3] \
            }                                              \
        }
#    define simd_fmadd_f32(a, b, c)                 \
        (simd_f32x4_t) {                            \
            {                                       \
                (a).f[0] * (b).f[0] + (c).f[0],     \
                    (a).f[1] * (b).f[1] + (c).f[1], \
                    (a).f[2] * (b).f[2] + (c).f[2], \
                    (a).f[3] * (b).f[3] + (c).f[3]  \
            }                                       \
        }

#endif

#endif
