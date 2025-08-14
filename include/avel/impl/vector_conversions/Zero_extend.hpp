#ifndef AVEL_ZERO_EXTEND_HPP
#define AVEL_ZERO_EXTEND_HPP

#define AVEL_ZEXT_DECL(T) \
    template<class V, std::uint32_t N = 0> \
    [[nodiscard]] \
    AVEL_FINL V zero_extend(T);\


#define AVEL_ZEXT_POLY_TO_POLY_IMPL(T, U, I, IMPL) \
    template<> \
    [[nodiscard]] \
    AVEL_FINL U zero_extend<U, I>(T v) { \
        return IMPL; \
    } \


#define AVEL_ZEXT_SCALAR_VECTOR_TO_SCALAR_VECTOR_SPEC(T, U) \
    template<> \
    [[nodiscard]] \
    AVEL_FINL U zero_extend<U, 0>(T v) { \
        return U{decay(v)}; \
    }\


#define AVEL_ZEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(T, U, IMPL) \
    template<> \
    [[nodiscard]] \
    AVEL_FINL U zero_extend<U, 0>(T v) { \
        return U{IMPL}; \
    } \


namespace avel {

    //=====================================================
    // Declarations for zero extensions from scalars
    //=====================================================

    AVEL_ZEXT_DECL(vec1x8u);
    AVEL_ZEXT_DECL(vec1x16u);
    AVEL_ZEXT_DECL(vec1x32u);
    AVEL_ZEXT_DECL(vec1x64u);

    //=====================================================
    // Declarations for zero extensions from vectors
    //=====================================================

    #if defined(AVEL_SSE2) || defined(AVEL_NEON)
        AVEL_ZEXT_DECL(vec16x8u);
        AVEL_ZEXT_DECL(vec8x16u);
        AVEL_ZEXT_DECL(vec4x32u);
        AVEL_ZEXT_DECL(vec2x64u);
    #endif

    #if defined(AVEL_AVX2)
        AVEL_ZEXT_DECL(vec32x8u);
        AVEL_ZEXT_DECL(vec16x16u);
        AVEL_ZEXT_DECL(vec8x32u);
        AVEL_ZEXT_DECL(vec4x64u);
    #endif

    #if defined(AVEL_AVX512F)
        AVEL_ZEXT_DECL(vec16x32u);
        AVEL_ZEXT_DECL(vec8x64u);
    #endif

    #if defined(AVEL_AVX512BW)
        AVEL_ZEXT_DECL(vec64x8u);
        AVEL_ZEXT_DECL(vec32x16u);
    #endif

    //=====================================================
    // Conversions to Scalars
    //=====================================================

    AVEL_ZEXT_SCALAR_VECTOR_TO_SCALAR_VECTOR_SPEC(vec1x8u, vec1x8u);
    AVEL_ZEXT_SCALAR_VECTOR_TO_SCALAR_VECTOR_SPEC(vec1x8u, vec1x16u);
    AVEL_ZEXT_SCALAR_VECTOR_TO_SCALAR_VECTOR_SPEC(vec1x8u, vec1x32u);
    AVEL_ZEXT_SCALAR_VECTOR_TO_SCALAR_VECTOR_SPEC(vec1x8u, vec1x64u);

    AVEL_ZEXT_SCALAR_VECTOR_TO_SCALAR_VECTOR_SPEC(vec1x16u, vec1x16u);
    AVEL_ZEXT_SCALAR_VECTOR_TO_SCALAR_VECTOR_SPEC(vec1x16u, vec1x32u);
    AVEL_ZEXT_SCALAR_VECTOR_TO_SCALAR_VECTOR_SPEC(vec1x16u, vec1x64u);

    AVEL_ZEXT_SCALAR_VECTOR_TO_SCALAR_VECTOR_SPEC(vec1x32u, vec1x32u);
    AVEL_ZEXT_SCALAR_VECTOR_TO_SCALAR_VECTOR_SPEC(vec1x32u, vec1x64u);

    AVEL_ZEXT_SCALAR_VECTOR_TO_SCALAR_VECTOR_SPEC(vec1x64u, vec1x64u);

    //=====================================================
    // Conversions to 128-bit vectors
    //=====================================================

    #if defined(AVEL_SSE2)
        AVEL_ZEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x8u, vec16x8u, _mm_cvtsi64_si128(decay(v)));
        AVEL_ZEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x8u, vec8x16u, _mm_cvtsi64_si128(decay(v)));
        AVEL_ZEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x8u, vec4x32u, _mm_cvtsi64_si128(decay(v)));
        AVEL_ZEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x8u, vec2x64u, _mm_cvtsi64_si128(decay(v)));

        AVEL_ZEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x16u, vec8x16u, _mm_cvtsi64_si128(decay(v)));
        AVEL_ZEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x16u, vec4x32u, _mm_cvtsi64_si128(decay(v)));
        AVEL_ZEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x16u, vec2x64u, _mm_cvtsi64_si128(decay(v)));

        AVEL_ZEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x32u, vec4x32u, _mm_cvtsi64_si128(decay(v)));
        AVEL_ZEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x32u, vec2x64u, _mm_cvtsi64_si128(decay(v)));

        AVEL_ZEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x64u, vec2x64u, _mm_cvtsi64_si128(decay(v)));
    #endif

    #if defined(AVEL_NEON)
        AVEL_ZEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x8u, vec16x8u, vsetq_lane_u8(x, vdupq_n_u8(0x00), 0));
        AVEL_ZEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x8u, vec8x16u, vsetq_lane_u16(x, vdupq_n_u16(0x00), 0));
        AVEL_ZEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x8u, vec4x32u, vsetq_lane_u32(x, vdupq_n_u32(0x00), 0));
        AVEL_ZEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x8u, vec2x64u, vsetq_lane_u64(x, vdupq_n_u64(0x00), 0));

        AVEL_ZEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x16u, vec8x16u, vsetq_lane_u16(x, vdupq_n_u16(0x00), 0));
        AVEL_ZEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x16u, vec4x32u, vsetq_lane_u32(x, vdupq_n_u32(0x00), 0));
        AVEL_ZEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x16u, vec2x64u, vsetq_lane_u64(x, vdupq_n_u64(0x00), 0));

        AVEL_ZEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x32u, vec4x32u, vsetq_lane_u32(x, vdupq_n_u32(0x00), 0));
        AVEL_ZEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x32u, vec2x64u, vsetq_lane_u64(x, vdupq_n_u64(0x00), 0));

        AVEL_ZEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x64u, vec2x64u, vsetq_lane_u64(x, vdupq_n_u64(0x00), 0));
    #endif

    //=====================================================
    // Conversions to 256-bit vectors
    //=====================================================

    #if defined(AVEL_AVX2)
        AVEL_ZEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x8u, vec32x8u, _mm256_castsi128_si256(_mm_cvtsi64_si128(decay(v))));
        AVEL_ZEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x8u, vec16x16u, _mm256_castsi128_si256(_mm_cvtsi64_si128(decay(v))));
        AVEL_ZEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x8u, vec8x32u, _mm256_castsi128_si256(_mm_cvtsi64_si128(decay(v))));
        AVEL_ZEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x8u, vec4x64u, _mm256_castsi128_si256(_mm_cvtsi64_si128(decay(v))));

        AVEL_ZEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x16u, vec16x16u, _mm256_castsi128_si256(_mm_cvtsi64_si128(decay(v))));
        AVEL_ZEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x16u, vec8x32u, _mm256_castsi128_si256(_mm_cvtsi64_si128(decay(v))));
        AVEL_ZEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x16u, vec4x64u, _mm256_castsi128_si256(_mm_cvtsi64_si128(decay(v))));

        AVEL_ZEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x32u, vec8x32u, _mm256_castsi128_si256(_mm_cvtsi64_si128(decay(v))));
        AVEL_ZEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x32u, vec4x64u, _mm256_castsi128_si256(_mm_cvtsi64_si128(decay(v))));

        AVEL_ZEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x64u, vec4x64u, _mm256_castsi128_si256(_mm_cvtsi64_si128(decay(v))));
    #endif

    //=====================================================
    // Conversions to 512-bit vectors
    //=====================================================

    #if defined(AVEL_AVX512BW)
        AVEL_ZEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x8u, vec64x8u, _mm512_castsi128_si512(_mm_cvtsi64_si128(decay(v))));
        AVEL_ZEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x8u, vec32x16u, _mm512_castsi128_si512(_mm_cvtsi64_si128(decay(v))));

        AVEL_ZEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x16u, vec32x16u, _mm512_castsi128_si512(_mm_cvtsi64_si128(decay(v))));
    #endif

    #if defined(AVEL_AVX512F)
        AVEL_ZEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x8u, vec16x32u, _mm512_castsi128_si512(_mm_cvtsi64_si128(decay(v))));
        AVEL_ZEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x8u, vec8x64u, _mm512_castsi128_si512(_mm_cvtsi64_si128(decay(v))));

        AVEL_ZEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x16u, vec16x32u, _mm512_castsi128_si512(_mm_cvtsi64_si128(decay(v))));
        AVEL_ZEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x16u, vec8x64u, _mm512_castsi128_si512(_mm_cvtsi64_si128(decay(v))));

        AVEL_ZEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x32u, vec16x32u, _mm512_castsi128_si512(_mm_cvtsi64_si128(decay(v))));
        AVEL_ZEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x32u, vec8x64u, _mm512_castsi128_si512(_mm_cvtsi64_si128(decay(v))));

        AVEL_ZEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x64u, vec8x64u, _mm512_castsi128_si512(_mm_cvtsi64_si128(decay(v))));
    #endif

    //=====================================================
    // Zero extensions from vec16x8u
    //=====================================================

    // Conversions to 128-bit vectors

    #if defined(AVEL_SSE2) && !defined(AVEL_SSE4_1)
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x8u, vec16x8u, 0, v);

        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x8u, vec8x16u, 0, vec8x16u{_mm_unpacklo_epi8(decay(v), _mm_setzero_si128())});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x8u, vec8x16u, 1, vec8x16u{_mm_unpackhi_epi8(decay(v), _mm_setzero_si128())});

        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x8u, vec4x32u, 0, vec4x32u{_mm_unpacklo_epi16(_mm_unpacklo_epi8(decay(v), _mm_setzero_si128()), _mm_setzero_si128())});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x8u, vec4x32u, 1, vec4x32u{_mm_unpacklo_epi16(_mm_unpacklo_epi8(decay(v), _mm_setzero_si128()), _mm_setzero_si128())});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x8u, vec4x32u, 2, vec4x32u{_mm_unpackhi_epi16(_mm_unpackhi_epi8(decay(v), _mm_setzero_si128()), _mm_setzero_si128())});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x8u, vec4x32u, 3, vec4x32u{_mm_unpackhi_epi16(_mm_unpacklo_epi8(decay(v), _mm_setzero_si128()), _mm_setzero_si128())});

        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x8u, vec2x64u, 0, vec2x64u{_mm_unpacklo_epi32(_mm_unpacklo_epi16(_mm_unpacklo_epi8(decay(v), _mm_setzero_si128()), _mm_setzero_si128()), _mm_setzero_si128())});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x8u, vec2x64u, 1, vec2x64u{_mm_unpacklo_epi32(_mm_unpacklo_epi16(_mm_unpackhi_epi8(decay(v), _mm_setzero_si128()), _mm_setzero_si128()), _mm_setzero_si128())});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x8u, vec2x64u, 2, vec2x64u{_mm_unpacklo_epi32(_mm_unpackhi_epi16(_mm_unpacklo_epi8(decay(v), _mm_setzero_si128()), _mm_setzero_si128()), _mm_setzero_si128())});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x8u, vec2x64u, 3, vec2x64u{_mm_unpacklo_epi32(_mm_unpackhi_epi16(_mm_unpackhi_epi8(decay(v), _mm_setzero_si128()), _mm_setzero_si128()), _mm_setzero_si128())});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x8u, vec2x64u, 4, vec2x64u{_mm_unpackhi_epi32(_mm_unpacklo_epi16(_mm_unpacklo_epi8(decay(v), _mm_setzero_si128()), _mm_setzero_si128()), _mm_setzero_si128())});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x8u, vec2x64u, 5, vec2x64u{_mm_unpackhi_epi32(_mm_unpacklo_epi16(_mm_unpackhi_epi8(decay(v), _mm_setzero_si128()), _mm_setzero_si128()), _mm_setzero_si128())});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x8u, vec2x64u, 6, vec2x64u{_mm_unpackhi_epi32(_mm_unpackhi_epi16(_mm_unpacklo_epi8(decay(v), _mm_setzero_si128()), _mm_setzero_si128()), _mm_setzero_si128())});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x8u, vec2x64u, 7, vec2x64u{_mm_unpackhi_epi32(_mm_unpackhi_epi16(_mm_unpackhi_epi8(decay(v), _mm_setzero_si128()), _mm_setzero_si128()), _mm_setzero_si128())});
    #endif

    #if defined(AVEL_SSE4_1)
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x8u, vec16x8u, 0, v);

        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x8u, vec8x16u, 0, vec8x16u{_mm_cvtepu8_epi16(decay(v))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x8u, vec8x16u, 1, vec8x16u{_mm_cvtepu8_epi16(_mm_bsrli_si128(decay(v), 8))});

        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x8u, vec4x32u, 0, vec4x32u{_mm_cvtepu8_epi32(decay(v))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x8u, vec4x32u, 1, vec4x32u{_mm_cvtepu8_epi32(_mm_bsrli_si128(decay(v), 0x4))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x8u, vec4x32u, 2, vec4x32u{_mm_cvtepu8_epi32(_mm_bsrli_si128(decay(v), 0x8))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x8u, vec4x32u, 3, vec4x32u{_mm_cvtepu8_epi32(_mm_bsrli_si128(decay(v), 0xc))});

        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x8u, vec2x64u, 0, vec2x64u{_mm_cvtepu8_epi64(decay(v))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x8u, vec2x64u, 1, vec2x64u{_mm_cvtepu8_epi64(_mm_bsrli_si128(decay(v), 0x2))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x8u, vec2x64u, 2, vec2x64u{_mm_cvtepu8_epi64(_mm_bsrli_si128(decay(v), 0x4))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x8u, vec2x64u, 3, vec2x64u{_mm_cvtepu8_epi64(_mm_bsrli_si128(decay(v), 0x6))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x8u, vec2x64u, 4, vec2x64u{_mm_cvtepu8_epi64(_mm_bsrli_si128(decay(v), 0x8))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x8u, vec2x64u, 5, vec2x64u{_mm_cvtepu8_epi64(_mm_bsrli_si128(decay(v), 0xa))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x8u, vec2x64u, 6, vec2x64u{_mm_cvtepu8_epi64(_mm_bsrli_si128(decay(v), 0xc))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x8u, vec2x64u, 7, vec2x64u{_mm_cvtepu8_epi64(_mm_bsrli_si128(decay(v), 0xe))});
    #endif

    // Conversions to 256-bit vectors

    #if defined(AVEL_AVX2)
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x8u, vec32x8u, 0, vec32x8u{_mm256_zextsi128_si256(decay(v))});

        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x8u, vec16x16u, 0, vec16x16u{_mm256_cvtepu8_epi16(decay(v))});

        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x8u, vec8x32u, 0, vec8x32u{_mm256_cvtepu8_epi32(decay(v))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x8u, vec8x32u, 1, vec8x32u{_mm256_cvtepu8_epi32(_mm_bsrli_si128(decay(v), 0x8))});

        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x8u, vec4x64u, 0, vec4x64u{_mm256_cvtepu8_epi64(_mm_bsrli_si128(decay(v), 0x0))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x8u, vec4x64u, 1, vec4x64u{_mm256_cvtepu8_epi64(_mm_bsrli_si128(decay(v), 0x4))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x8u, vec4x64u, 2, vec4x64u{_mm256_cvtepu8_epi64(_mm_bsrli_si128(decay(v), 0x8))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x8u, vec4x64u, 3, vec4x64u{_mm256_cvtepu8_epi64(_mm_bsrli_si128(decay(v), 0xc))});
    #endif

    // Conversions to 512-bit vectors

    #if defined(AVEL_AVX512BW)
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x8u, vec64x8u, 0, vec64x8u{_mm512_zextsi128_si512(decay(v))});

        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x8u, vec32x16u, 0, vec32x16u{_mm512_zextsi256_si512(_mm256_cvtepu8_epi16(decay(v)))});
    #endif

    #if defined(AVEL_AVX512F)
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x8u, vec16x32u, 0, vec16x32u{_mm512_cvtepu8_epi32(decay(v))});

        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x8u, vec8x64u, 0, vec8x64u{_mm512_cvtepu8_epi64(_mm_bsrli_si128(decay(v), 0x0))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x8u, vec8x64u, 1, vec8x64u{_mm512_cvtepu8_epi64(_mm_bsrli_si128(decay(v), 0x8))});
    #endif

    //=====================================================
    // Zero extensions from vec8x16u
    //=====================================================

    // Conversions to 128-bit vectors

    #if defined(AVEL_SSE2) && !defined(AVEL_SSE4_1)
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec8x16u, vec8x16u, 0, v);

        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec8x16u, vec4x32u, 0, vec4x32u{_mm_unpacklo_epi16(decay(v), _mm_setzero_si128())});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec8x16u, vec4x32u, 1, vec4x32u{_mm_unpackhi_epi16(decay(v), _mm_setzero_si128())});

        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec8x16u, vec2x64u, 0, vec2x64u{_mm_unpacklo_epi32(_mm_unpacklo_epi16(decay(v), _mm_setzero_si128()), _mm_setzero_si128())});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec8x16u, vec2x64u, 1, vec2x64u{_mm_unpacklo_epi32(_mm_unpackhi_epi16(decay(v), _mm_setzero_si128()), _mm_setzero_si128())});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec8x16u, vec2x64u, 2, vec2x64u{_mm_unpackhi_epi32(_mm_unpacklo_epi16(decay(v), _mm_setzero_si128()), _mm_setzero_si128())});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec8x16u, vec2x64u, 3, vec2x64u{_mm_unpackhi_epi32(_mm_unpackhi_epi16(decay(v), _mm_setzero_si128()), _mm_setzero_si128())});
    #endif

    #if defined(AVEL_SSE4_1)
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec8x16u, vec8x16u, 0, v);

        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec8x16u, vec4x32u, 0, vec4x32u{_mm_cvtepu16_epi32(decay(v))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec8x16u, vec4x32u, 1, vec4x32u{_mm_cvtepu16_epi32(decay(v))});

        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec8x16u, vec2x64u, 0, vec2x64u{_mm_cvtepu8_epi64(_mm_bsrli_si128(decay(v), 0x0))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec8x16u, vec2x64u, 1, vec2x64u{_mm_cvtepu8_epi64(_mm_bsrli_si128(decay(v), 0x4))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec8x16u, vec2x64u, 2, vec2x64u{_mm_cvtepu8_epi64(_mm_bsrli_si128(decay(v), 0x8))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec8x16u, vec2x64u, 3, vec2x64u{_mm_cvtepu8_epi64(_mm_bsrli_si128(decay(v), 0xc))});
    #endif

    // Conversions to 256-bit vectors

    #if defined(AVEL_AVX2)
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec8x16u, vec16x16u, 0, vec16x16u{_mm256_zextsi128_si256(decay(v))});

        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec8x16u, vec8x32u, 0, vec8x32u{_mm256_cvtepu16_epi32(decay(v))});

        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec8x16u, vec4x64u, 0, vec4x64u{_mm256_cvtepu16_epi64(decay(v))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec8x16u, vec4x64u, 1, vec4x64u{_mm256_cvtepu16_epi64(_mm_bsrli_si128(decay(v), 0x8))});
    #endif

    // Conversions to 512-bit vectors

    #if defined(AVEL_AVX512BW)
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec8x16u, vec32x16u, 0, vec32x16u{_mm512_zextsi128_si512(decay(v))});
    #endif

    #if defined(AVEL_AVX512F)
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec8x16u, vec16x32u, 0, vec16x32u{_mm512_zextsi256_si512(_mm256_cvtepu16_epi32(decay(v)))});

        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec8x16u, vec8x64u, 0, vec8x64u{_mm512_cvtepu16_epi64(decay(v))});
    #endif

    //=====================================================
    // Zero extensions from vec4x32u
    //=====================================================

    // Conversions to 128-bit vectors

    #if defined(AVEL_SSE2)
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec4x32u, vec4x32u, 0, v);

        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec4x32u, vec2x64u, 0, vec2x64u{_mm_unpacklo_epi32(decay(v), _mm_setzero_si128())});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec4x32u, vec2x64u, 1, vec2x64u{_mm_unpackhi_epi32(decay(v), _mm_setzero_si128())});
    #endif

    // Conversions to 256-bit vectors

    #if defined(AVEL_AVX2)
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec4x32u, vec8x32u, 0, vec8x32u{_mm256_zextsi128_si256(decay(v))});

        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec4x32u, vec4x64u, 0, vec4x64u{_mm256_cvtepu32_epi64(decay(v))});
    #endif

    // Conversions to 512-bit vectors

    #if defined(AVEL_AVX512F)
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec4x32u, vec16x32u, 0, vec16x32u{_mm512_zextsi128_si512(decay(v))});

        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec4x32u, vec8x64u, 0, vec8x64u{_mm512_zextsi256_si512(_mm256_cvtepu32_epi64(decay(v)))});
    #endif

    //=====================================================
    // Zero extensions from vec2x64u
    //=====================================================

    // Conversions to 128-bit vectors

    #if defined(AVEL_SSE2)
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec2x64u, vec2x64u, 0, v);
    #endif

    // Conversions to 256-bit vectors

    #if defined(AVEL_AVX2)
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec2x64u, vec4x64u, 0, vec4x64u{_mm256_zextsi128_si256(decay(v))});
    #endif

    // Conversions to 512-bit vectors

    #if defined(AVEL_AVX512F)
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec2x64u, vec8x64u, 0, vec8x64u{_mm512_zextsi128_si512(decay(v))});
    #endif

    //=====================================================
    // Zero extensions from vec32x8u
    //=====================================================

    // Conversions to 256-bit vectors

    #if defined(AVEL_AVX2)
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec32x8u, vec32x8u, 0, v);

        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec32x8u, vec16x16u, 0, vec16x16u{_mm256_cvtepu8_epi16(_mm256_extractf128_si256(decay(v), 0x0))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec32x8u, vec16x16u, 1, vec16x16u{_mm256_cvtepu8_epi16(_mm256_extractf128_si256(decay(v), 0x1))});

        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec32x8u, vec8x32u, 0, vec8x32u{_mm256_cvtepu8_epi32(_mm256_extractf128_si256(decay(v), 0x0))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec32x8u, vec8x32u, 1, vec8x32u{_mm256_cvtepu8_epi32(_mm_bsrli_si128(_mm256_extractf128_si256(decay(v), 0x0), 0x8))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec32x8u, vec8x32u, 2, vec8x32u{_mm256_cvtepu8_epi32(_mm256_extractf128_si256(decay(v), 0x1))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec32x8u, vec8x32u, 3, vec8x32u{_mm256_cvtepu8_epi32(_mm_bsrli_si128(_mm256_extractf128_si256(decay(v), 0x1), 0x8))});

        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec32x8u, vec4x64u, 0, vec4x64u{_mm256_cvtepu8_epi64(_mm_bsrli_si128(_mm256_extractf128_si256(decay(v), 0x0), 0x0))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec32x8u, vec4x64u, 1, vec4x64u{_mm256_cvtepu8_epi64(_mm_bsrli_si128(_mm256_extractf128_si256(decay(v), 0x0), 0x4))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec32x8u, vec4x64u, 2, vec4x64u{_mm256_cvtepu8_epi64(_mm_bsrli_si128(_mm256_extractf128_si256(decay(v), 0x0), 0x8))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec32x8u, vec4x64u, 3, vec4x64u{_mm256_cvtepu8_epi64(_mm_bsrli_si128(_mm256_extractf128_si256(decay(v), 0x0), 0xc))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec32x8u, vec4x64u, 4, vec4x64u{_mm256_cvtepu8_epi64(_mm_bsrli_si128(_mm256_extractf128_si256(decay(v), 0x1), 0x0))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec32x8u, vec4x64u, 5, vec4x64u{_mm256_cvtepu8_epi64(_mm_bsrli_si128(_mm256_extractf128_si256(decay(v), 0x1), 0x4))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec32x8u, vec4x64u, 6, vec4x64u{_mm256_cvtepu8_epi64(_mm_bsrli_si128(_mm256_extractf128_si256(decay(v), 0x1), 0x8))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec32x8u, vec4x64u, 7, vec4x64u{_mm256_cvtepu8_epi64(_mm_bsrli_si128(_mm256_extractf128_si256(decay(v), 0x1), 0xc))});
    #endif

    // Conversions to 512-bit vectors

    #if defined(AVEL_AVX512BW)
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec32x8u, vec64x8u, 0, vec64x8u{_mm512_zextsi256_si512(decay(v))});

        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec32x8u, vec32x16u, 0, vec32x16u{_mm512_cvtepu8_epi16(decay(v))});
    #endif

    #if defined(AVEL_AVX512F)
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec32x8u, vec16x32u, 0, vec16x32u{_mm512_cvtepu8_epi32(_mm256_extractf128_si256(decay(v), 0x0))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec32x8u, vec16x32u, 1, vec16x32u{_mm512_cvtepu8_epi32(_mm256_extractf128_si256(decay(v), 0x1))});

        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec32x8u, vec8x64u, 0, vec8x64u{_mm512_cvtepu8_epi64(_mm256_extractf128_si256(decay(v), 0x0))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec32x8u, vec8x64u, 1, vec8x64u{_mm512_cvtepu8_epi64(_mm256_extractf128_si256(decay(v), 0x1))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec32x8u, vec8x64u, 2, vec8x64u{_mm512_cvtepu8_epi64(_mm256_extractf128_si256(decay(v), 0x0))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec32x8u, vec8x64u, 3, vec8x64u{_mm512_cvtepu8_epi64(_mm256_extractf128_si256(decay(v), 0x1))});
    #endif

    //=====================================================
    // Zero extensions from vec16x16u
    //=====================================================

    // Conversions to 256-bit vectors

    #if defined(AVEL_AVX2)
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x16u, vec16x16u, 0, v);

        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x16u, vec8x32u, 0, vec8x32u{_mm256_cvtepu16_epi32(_mm256_extracti128_si256(decay(v), 0x0))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x16u, vec8x32u, 1, vec8x32u{_mm256_cvtepu16_epi32(_mm256_extracti128_si256(decay(v), 0x1))});

        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x16u, vec4x64u, 0, vec4x64u{_mm256_cvtepu16_epi64(_mm256_extracti128_si256(decay(v), 0x0))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x16u, vec4x64u, 1, vec4x64u{_mm256_cvtepu16_epi64(_mm_bsrli_si128(_mm256_extracti128_si256(decay(v), 0x0), 0x8))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x16u, vec4x64u, 2, vec4x64u{_mm256_cvtepu16_epi64(_mm256_extracti128_si256(decay(v), 0x1))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x16u, vec4x64u, 3, vec4x64u{_mm256_cvtepu16_epi64(_mm_bsrli_si128(_mm256_extracti128_si256(decay(v), 0x1), 0x8))});
    #endif

    // Conversions to 512-bit vectors

    #if defined(AVEL_AVX512BW)
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x16u, vec32x16u, 0, vec32x16u{_mm512_zextsi256_si512(decay(v))});
    #endif

    #if defined(AVEL_AVX512F)
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x16u, vec16x32u, 0, vec16x32u{_mm512_cvtepu16_epi32(decay(v))});

        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x16u, vec8x64u, 0, vec8x64u{_mm512_cvtepu16_epi64(_mm256_extractf128_si256(decay(v), 0x0))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x16u, vec8x64u, 1, vec8x64u{_mm512_cvtepu16_epi64(_mm256_extractf128_si256(decay(v), 0x1))});
    #endif

    //=====================================================
    // Zero extensions from vec8x32u
    //=====================================================

    // Conversions to 256-bit vectors

    #if defined(AVEL_AVX2)
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec8x32u, vec8x32u, 0, v);

        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec8x32u, vec4x64u, 0, vec4x64u{_mm256_cvtepu32_epi64(_mm256_extractf128_si256(decay(v), 0x0))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec8x32u, vec4x64u, 1, vec4x64u{_mm256_cvtepu32_epi64(_mm256_extractf128_si256(decay(v), 0x1))});
    #endif

    // Conversions to 512-bit vectors

    #if defined(AVEL_AVX512F)
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec8x32u, vec16x32u, 0, vec16x32u{_mm512_zextsi256_si512(decay(v))});

        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec8x32u, vec8x64u, 0, vec8x64u{_mm512_cvtepu32_epi64(decay(v))});
    #endif

    //=====================================================
    // Zero extensions from vec4x64u
    //=====================================================

    // Conversions to 256-bit vectors

    #if defined(AVEL_AVX2)
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec4x64u, vec4x64u, 0, v);
    #endif

    // Conversions to 512-bit vectors

    #if defined(AVEL_AVX512F)
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec4x64u, vec8x64u, 0, vec8x64u{_mm512_zextsi256_si512(decay(v))});
    #endif

    //=====================================================
    // Zero extensions from vec64x8u
    //=====================================================

    #if defined(AVEL_AVX512BW)
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec64x8u, vec64x8u, 0, v);

        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec64x8u, vec32x16u, 0, vec32x16u{_mm512_cvtepu8_epi16(_mm512_extracti64x4_epi64(decay(v), 0x0))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec64x8u, vec32x16u, 1, vec32x16u{_mm512_cvtepu8_epi16(_mm512_extracti64x4_epi64(decay(v), 0x1))});

        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec64x8u, vec16x32u, 0, vec16x32u{_mm512_cvtepu8_epi32(_mm512_extracti64x2_epi64(decay(v), 0x0))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec64x8u, vec16x32u, 1, vec16x32u{_mm512_cvtepu8_epi32(_mm512_extracti64x2_epi64(decay(v), 0x1))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec64x8u, vec16x32u, 2, vec16x32u{_mm512_cvtepu8_epi32(_mm512_extracti64x2_epi64(decay(v), 0x2))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec64x8u, vec16x32u, 3, vec16x32u{_mm512_cvtepu8_epi32(_mm512_extracti64x2_epi64(decay(v), 0x3))});

        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec64x8u, vec8x64u, 0, vec8x64u{_mm512_cvtepu8_epi64(_mm512_extracti64x2_epi64(decay(v), 0x0))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec64x8u, vec8x64u, 1, vec8x64u{_mm512_cvtepu8_epi64(_mm_bsrli_si128(_mm512_extracti64x2_epi64(decay(v), 0x0), 0x1))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec64x8u, vec8x64u, 2, vec8x64u{_mm512_cvtepu8_epi64(_mm512_extracti64x2_epi64(decay(v), 0x1))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec64x8u, vec8x64u, 3, vec8x64u{_mm512_cvtepu8_epi64(_mm_bsrli_si128(_mm512_extracti64x2_epi64(decay(v), 0x1), 0x1))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec64x8u, vec8x64u, 4, vec8x64u{_mm512_cvtepu8_epi64(_mm512_extracti64x2_epi64(decay(v), 0x2))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec64x8u, vec8x64u, 5, vec8x64u{_mm512_cvtepu8_epi64(_mm_bsrli_si128(_mm512_extracti64x2_epi64(decay(v), 0x2), 0x1))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec64x8u, vec8x64u, 6, vec8x64u{_mm512_cvtepu8_epi64(_mm512_extracti64x2_epi64(decay(v), 0x3))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec64x8u, vec8x64u, 7, vec8x64u{_mm512_cvtepu8_epi64(_mm_bsrli_si128(_mm512_extracti64x2_epi64(decay(v), 0x3), 0x1))});
    #endif

    //=====================================================
    // Zero extensions from vec32x16u
    //=====================================================

    #if defined(AVEL_AVX512BW)
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec32x16u, vec32x16u, 0, v);

        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec32x16u, vec16x32u, 0, vec16x32u{_mm512_cvtepu16_epi32(_mm512_extracti64x4_epi64(decay(v), 0x0))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec32x16u, vec16x32u, 1, vec16x32u{_mm512_cvtepu16_epi32(_mm512_extracti64x4_epi64(decay(v), 0x1))});

        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec32x16u, vec8x64u, 0, vec8x64u{_mm512_cvtepu16_epi64(_mm512_extracti64x2_epi64(decay(v), 0x0))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec32x16u, vec8x64u, 1, vec8x64u{_mm512_cvtepu16_epi64(_mm512_extracti64x2_epi64(decay(v), 0x1))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec32x16u, vec8x64u, 2, vec8x64u{_mm512_cvtepu16_epi64(_mm512_extracti64x2_epi64(decay(v), 0x2))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec32x16u, vec8x64u, 3, vec8x64u{_mm512_cvtepu16_epi64(_mm512_extracti64x2_epi64(decay(v), 0x3))});
    #endif

    //=====================================================
    // Zero extensions from vec16x32u
    //=====================================================

    #if defined(AVEL_AVX512F)
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x32u, vec16x32u, 0, v);

        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x32u, vec8x64u, 0, vec8x64u{_mm512_cvtepu32_epi64(_mm512_extracti64x4_epi64(decay(v), 0x0))});
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec16x32u, vec8x64u, 1, vec8x64u{_mm512_cvtepu32_epi64(_mm512_extracti64x4_epi64(decay(v), 0x1))});
    #endif

    //=====================================================
    // Zero extensions from vec8x64u
    //=====================================================

    #if defined(AVEL_AVX512F)
        AVEL_ZEXT_POLY_TO_POLY_IMPL(vec8x64u, vec8x64u, 0, v);
    #endif

}

#endif //AVEL_ZERO_EXTEND_HPP
