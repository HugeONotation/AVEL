#ifndef AVEL_SIGN_EXTEND_HPP
#define AVEL_SIGN_EXTEND_HPP

#define AVEL_SEXT_DECL(T) \
    template<class V, std::uint32_t N = 0> \
    [[nodiscard]] \
    AVEL_FINL V sign_extend(T);\


#define AVEL_SEXT_POLY_TO_POLY_IMPL(T, U, I, IMPL) \
    template<> \
    [[nodiscard]] \
    AVEL_FINL U sign_extend<U, I>(T v) { \
        return IMPL; \
    } \


#define AVEL_SEXT_SCALAR_VECTOR_TO_SCALAR_VECTOR_SPEC(T, U) \
    template<> \
    [[nodiscard]] \
    AVEL_FINL U sign_extend<U, 0>(T v) { \
        return U{decay(v)}; \
    }\


#define AVEL_SEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(T, U, IMPL) \
    template<> \
    [[nodiscard]] \
    AVEL_FINL U sign_extend<U, 0>(T v) { \
        return U{IMPL}; \
    } \


namespace avel {

    //=====================================================
    // Declarations for zero extensions from scalars
    //=====================================================

    AVEL_SEXT_DECL(vec1x8i);
    AVEL_SEXT_DECL(vec1x16i);
    AVEL_SEXT_DECL(vec1x32i);
    AVEL_SEXT_DECL(vec1x64i);

    //=====================================================
    // Declarations for zero extensions from vectors
    //=====================================================

    #if defined(AVEL_SSE2) || defined(AVEL_NEON)
        AVEL_SEXT_DECL(vec16x8i);
        AVEL_SEXT_DECL(vec8x16i);
        AVEL_SEXT_DECL(vec4x32i);
        AVEL_SEXT_DECL(vec2x64i);
    #endif

    #if defined(AVEL_AVX2)
        AVEL_SEXT_DECL(vec32x8i);
        AVEL_SEXT_DECL(vec16x16i);
        AVEL_SEXT_DECL(vec8x32i);
        AVEL_SEXT_DECL(vec4x64i);
    #endif

    #if defined(AVEL_AVX512F)
        AVEL_SEXT_DECL(vec16x32i);
        AVEL_SEXT_DECL(vec8x64i);
    #endif

    #if defined(AVEL_AVX512BW)
        AVEL_SEXT_DECL(vec64x8i);
        AVEL_SEXT_DECL(vec32x16i);
    #endif

    //=====================================================
    // Conversions to Scalars
    //=====================================================

    AVEL_SEXT_SCALAR_VECTOR_TO_SCALAR_VECTOR_SPEC(vec1x8i, vec1x8i);
    AVEL_SEXT_SCALAR_VECTOR_TO_SCALAR_VECTOR_SPEC(vec1x8i, vec1x16i);
    AVEL_SEXT_SCALAR_VECTOR_TO_SCALAR_VECTOR_SPEC(vec1x8i, vec1x32i);
    AVEL_SEXT_SCALAR_VECTOR_TO_SCALAR_VECTOR_SPEC(vec1x8i, vec1x64i);

    AVEL_SEXT_SCALAR_VECTOR_TO_SCALAR_VECTOR_SPEC(vec1x16i, vec1x16i);
    AVEL_SEXT_SCALAR_VECTOR_TO_SCALAR_VECTOR_SPEC(vec1x16i, vec1x32i);
    AVEL_SEXT_SCALAR_VECTOR_TO_SCALAR_VECTOR_SPEC(vec1x16i, vec1x64i);

    AVEL_SEXT_SCALAR_VECTOR_TO_SCALAR_VECTOR_SPEC(vec1x32i, vec1x32i);
    AVEL_SEXT_SCALAR_VECTOR_TO_SCALAR_VECTOR_SPEC(vec1x32i, vec1x64i);

    AVEL_SEXT_SCALAR_VECTOR_TO_SCALAR_VECTOR_SPEC(vec1x64i, vec1x64i);

    //=====================================================
    // Conversions to 128-bit vectors
    //=====================================================

    #if defined(AVEL_SSE2)
        AVEL_SEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x8i, vec16x8i, _mm_cvtsi64_si128(decay(v)));
        AVEL_SEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x8i, vec8x16i, _mm_cvtsi64_si128(decay(v)));
        AVEL_SEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x8i, vec4x32i, _mm_cvtsi64_si128(decay(v)));
        AVEL_SEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x8i, vec2x64i, _mm_cvtsi64_si128(decay(v)));

        AVEL_SEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x16i, vec8x16i, _mm_cvtsi64_si128(decay(v)));
        AVEL_SEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x16i, vec4x32i, _mm_cvtsi64_si128(decay(v)));
        AVEL_SEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x16i, vec2x64i, _mm_cvtsi64_si128(decay(v)));

        AVEL_SEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x32i, vec4x32i, _mm_cvtsi64_si128(decay(v)));
        AVEL_SEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x32i, vec2x64i, _mm_cvtsi64_si128(decay(v)));

        AVEL_SEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x64i, vec2x64i, _mm_cvtsi64_si128(decay(v)));
    #endif

    #if defined(AVEL_NEON)
        AVEL_SEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x8i, vec16x8i, vsetq_lane_u8(x, vdupq_n_u8(0x00), 0));
        AVEL_SEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x8i, vec8x16i, vsetq_lane_u16(x, vdupq_n_u16(0x00), 0));
        AVEL_SEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x8i, vec4x32i, vsetq_lane_u32(x, vdupq_n_u32(0x00), 0));
        AVEL_SEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x8i, vec2x64i, vsetq_lane_u64(x, vdupq_n_u64(0x00), 0));

        AVEL_SEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x16i, vec8x16i, vsetq_lane_u16(x, vdupq_n_u16(0x00), 0));
        AVEL_SEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x16i, vec4x32i, vsetq_lane_u32(x, vdupq_n_u32(0x00), 0));
        AVEL_SEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x16i, vec2x64i, vsetq_lane_u64(x, vdupq_n_u64(0x00), 0));

        AVEL_SEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x32i, vec4x32i, vsetq_lane_u32(x, vdupq_n_u32(0x00), 0));
        AVEL_SEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x32i, vec2x64i, vsetq_lane_u64(x, vdupq_n_u64(0x00), 0));

        AVEL_SEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x64i, vec2x64i, vsetq_lane_u64(x, vdupq_n_u64(0x00), 0));
    #endif

    //=====================================================
    // Conversions to 256-bit vectors
    //=====================================================

    #if defined(AVEL_AVX2)
        AVEL_SEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x8i, vec32x8i, _mm256_castsi128_si256(_mm_cvtsi64_si128(decay(v))));
        AVEL_SEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x8i, vec16x16i, _mm256_castsi128_si256(_mm_cvtsi64_si128(decay(v))));
        AVEL_SEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x8i, vec8x32i, _mm256_castsi128_si256(_mm_cvtsi64_si128(decay(v))));
        AVEL_SEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x8i, vec4x64i, _mm256_castsi128_si256(_mm_cvtsi64_si128(decay(v))));

        AVEL_SEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x16i, vec16x16i, _mm256_castsi128_si256(_mm_cvtsi64_si128(decay(v))));
        AVEL_SEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x16i, vec8x32i, _mm256_castsi128_si256(_mm_cvtsi64_si128(decay(v))));
        AVEL_SEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x16i, vec4x64i, _mm256_castsi128_si256(_mm_cvtsi64_si128(decay(v))));

        AVEL_SEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x32i, vec8x32i, _mm256_castsi128_si256(_mm_cvtsi64_si128(decay(v))));
        AVEL_SEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x32i, vec4x64i, _mm256_castsi128_si256(_mm_cvtsi64_si128(decay(v))));

        AVEL_SEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x64i, vec4x64i, _mm256_castsi128_si256(_mm_cvtsi64_si128(decay(v))));
    #endif

    //=====================================================
    // Conversions to 512-bit vectors
    //=====================================================

    #if defined(AVEL_AVX512BW)
        AVEL_SEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x8i, vec64x8i, _mm512_castsi128_si512(_mm_cvtsi64_si128(decay(v))));
        AVEL_SEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x8i, vec32x16i, _mm512_castsi128_si512(_mm_cvtsi64_si128(decay(v))));

        AVEL_SEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x16i, vec32x16i, _mm512_castsi128_si512(_mm_cvtsi64_si128(decay(v))));
    #endif

    #if defined(AVEL_AVX512F)
        AVEL_SEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x8i, vec16x32i, _mm512_castsi128_si512(_mm_cvtsi64_si128(decay(v))));
        AVEL_SEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x8i, vec8x64i, _mm512_castsi128_si512(_mm_cvtsi64_si128(decay(v))));

        AVEL_SEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x16i, vec16x32i, _mm512_castsi128_si512(_mm_cvtsi64_si128(decay(v))));
        AVEL_SEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x16i, vec8x64i, _mm512_castsi128_si512(_mm_cvtsi64_si128(decay(v))));

        AVEL_SEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x32i, vec16x32i, _mm512_castsi128_si512(_mm_cvtsi64_si128(decay(v))));
        AVEL_SEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x32i, vec8x64i, _mm512_castsi128_si512(_mm_cvtsi64_si128(decay(v))));

        AVEL_SEXT_SCALAR_VECTOR_TO_POLY_VECTOR_IMPL(vec1x64i, vec8x64i, _mm512_castsi128_si512(_mm_cvtsi64_si128(decay(v))));
    #endif

    //=====================================================
    // Zero extensions from vec16x8i
    //=====================================================

    // Conversions to 128-bit vectors

    #if defined(AVEL_SSE2) && !defined(AVEL_SSE4_1)
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x8i, vec16x8i, 0, v);

        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x8i, vec8x16i, 0, vec8x16i{_mm_unpacklo_epi8(decay(v), _mm_cmplt_epi8(decay(v), _mm_setzero_si128()))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x8i, vec8x16i, 1, vec8x16i{_mm_unpackhi_epi8(decay(v), _mm_cmplt_epi8(decay(v), _mm_setzero_si128()))});

        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x8i, vec4x32i, 0, vec4x32i{_mm_unpacklo_epi16(_mm_unpacklo_epi8(decay(v), _mm_cmplt_epi16(decay(v), _mm_setzero_si128())), _mm_cmplt_epi16(decay(v), _mm_setzero_si128()))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x8i, vec4x32i, 1, vec4x32i{_mm_unpacklo_epi16(_mm_unpacklo_epi8(decay(v), _mm_cmplt_epi16(decay(v), _mm_setzero_si128())), _mm_cmplt_epi16(decay(v), _mm_setzero_si128()))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x8i, vec4x32i, 2, vec4x32i{_mm_unpackhi_epi16(_mm_unpackhi_epi8(decay(v), _mm_cmplt_epi16(decay(v), _mm_setzero_si128())), _mm_cmplt_epi16(decay(v), _mm_setzero_si128()))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x8i, vec4x32i, 3, vec4x32i{_mm_unpackhi_epi16(_mm_unpacklo_epi8(decay(v), _mm_cmplt_epi16(decay(v), _mm_setzero_si128())), _mm_cmplt_epi16(decay(v), _mm_setzero_si128()))});

        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x8i, vec2x64i, 0, vec2x64i{_mm_unpacklo_epi32(_mm_unpacklo_epi16(_mm_unpacklo_epi8(decay(v), _mm_cmplt_epi32(decay(v), _mm_setzero_si128())), _mm_cmplt_epi32(decay(v), _mm_setzero_si128())), _mm_cmplt_epi32(decay(v), _mm_setzero_si128()))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x8i, vec2x64i, 1, vec2x64i{_mm_unpacklo_epi32(_mm_unpacklo_epi16(_mm_unpackhi_epi8(decay(v), _mm_cmplt_epi32(decay(v), _mm_setzero_si128())), _mm_cmplt_epi32(decay(v), _mm_setzero_si128())), _mm_cmplt_epi32(decay(v), _mm_setzero_si128()))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x8i, vec2x64i, 2, vec2x64i{_mm_unpacklo_epi32(_mm_unpackhi_epi16(_mm_unpacklo_epi8(decay(v), _mm_cmplt_epi32(decay(v), _mm_setzero_si128())), _mm_cmplt_epi32(decay(v), _mm_setzero_si128())), _mm_cmplt_epi32(decay(v), _mm_setzero_si128()))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x8i, vec2x64i, 3, vec2x64i{_mm_unpacklo_epi32(_mm_unpackhi_epi16(_mm_unpackhi_epi8(decay(v), _mm_cmplt_epi32(decay(v), _mm_setzero_si128())), _mm_cmplt_epi32(decay(v), _mm_setzero_si128())), _mm_cmplt_epi32(decay(v), _mm_setzero_si128()))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x8i, vec2x64i, 4, vec2x64i{_mm_unpackhi_epi32(_mm_unpacklo_epi16(_mm_unpacklo_epi8(decay(v), _mm_cmplt_epi32(decay(v), _mm_setzero_si128())), _mm_cmplt_epi32(decay(v), _mm_setzero_si128())), _mm_cmplt_epi32(decay(v), _mm_setzero_si128()))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x8i, vec2x64i, 5, vec2x64i{_mm_unpackhi_epi32(_mm_unpacklo_epi16(_mm_unpackhi_epi8(decay(v), _mm_cmplt_epi32(decay(v), _mm_setzero_si128())), _mm_cmplt_epi32(decay(v), _mm_setzero_si128())), _mm_cmplt_epi32(decay(v), _mm_setzero_si128()))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x8i, vec2x64i, 6, vec2x64i{_mm_unpackhi_epi32(_mm_unpackhi_epi16(_mm_unpacklo_epi8(decay(v), _mm_cmplt_epi32(decay(v), _mm_setzero_si128())), _mm_cmplt_epi32(decay(v), _mm_setzero_si128())), _mm_cmplt_epi32(decay(v), _mm_setzero_si128()))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x8i, vec2x64i, 7, vec2x64i{_mm_unpackhi_epi32(_mm_unpackhi_epi16(_mm_unpackhi_epi8(decay(v), _mm_cmplt_epi32(decay(v), _mm_setzero_si128())), _mm_cmplt_epi32(decay(v), _mm_setzero_si128())), _mm_cmplt_epi32(decay(v), _mm_setzero_si128()))});
    #endif

    #if defined(AVEL_SSE4_1)
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x8i, vec16x8i, 0, v);

        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x8i, vec8x16i, 0, vec8x16i{_mm_cvtepi8_epi16(decay(v))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x8i, vec8x16i, 1, vec8x16i{_mm_cvtepi8_epi16(_mm_bsrli_si128(decay(v), 8))});

        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x8i, vec4x32i, 0, vec4x32i{_mm_cvtepi8_epi32(decay(v))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x8i, vec4x32i, 1, vec4x32i{_mm_cvtepi8_epi32(_mm_bsrli_si128(decay(v), 0x4))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x8i, vec4x32i, 2, vec4x32i{_mm_cvtepi8_epi32(_mm_bsrli_si128(decay(v), 0x8))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x8i, vec4x32i, 3, vec4x32i{_mm_cvtepi8_epi32(_mm_bsrli_si128(decay(v), 0xc))});

        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x8i, vec2x64i, 0, vec2x64i{_mm_cvtepi8_epi64(decay(v))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x8i, vec2x64i, 1, vec2x64i{_mm_cvtepi8_epi64(_mm_bsrli_si128(decay(v), 0x2))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x8i, vec2x64i, 2, vec2x64i{_mm_cvtepi8_epi64(_mm_bsrli_si128(decay(v), 0x4))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x8i, vec2x64i, 3, vec2x64i{_mm_cvtepi8_epi64(_mm_bsrli_si128(decay(v), 0x6))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x8i, vec2x64i, 4, vec2x64i{_mm_cvtepi8_epi64(_mm_bsrli_si128(decay(v), 0x8))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x8i, vec2x64i, 5, vec2x64i{_mm_cvtepi8_epi64(_mm_bsrli_si128(decay(v), 0xa))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x8i, vec2x64i, 6, vec2x64i{_mm_cvtepi8_epi64(_mm_bsrli_si128(decay(v), 0xc))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x8i, vec2x64i, 7, vec2x64i{_mm_cvtepi8_epi64(_mm_bsrli_si128(decay(v), 0xe))});
    #endif

    // Conversions to 256-bit vectors

    #if defined(AVEL_AVX2)
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x8i, vec32x8i, 0, vec32x8i{_mm256_zextsi128_si256(decay(v))});

        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x8i, vec16x16i, 0, vec16x16i{_mm256_cvtepi8_epi16(decay(v))});

        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x8i, vec8x32i, 0, vec8x32i{_mm256_cvtepi8_epi32(decay(v))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x8i, vec8x32i, 1, vec8x32i{_mm256_cvtepi8_epi32(_mm_bsrli_si128(decay(v), 0x8))});

        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x8i, vec4x64i, 0, vec4x64i{_mm256_cvtepi8_epi64(_mm_bsrli_si128(decay(v), 0x0))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x8i, vec4x64i, 1, vec4x64i{_mm256_cvtepi8_epi64(_mm_bsrli_si128(decay(v), 0x4))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x8i, vec4x64i, 2, vec4x64i{_mm256_cvtepi8_epi64(_mm_bsrli_si128(decay(v), 0x8))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x8i, vec4x64i, 3, vec4x64i{_mm256_cvtepi8_epi64(_mm_bsrli_si128(decay(v), 0xc))});
    #endif

    // Conversions to 512-bit vectors

    #if defined(AVEL_AVX512BW)
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x8i, vec64x8i, 0, vec64x8i{_mm512_zextsi128_si512(decay(v))});

        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x8i, vec32x16i, 0, vec32x16i{_mm512_zextsi256_si512(_mm256_cvtepi8_epi16(decay(v)))});
    #endif

    #if defined(AVEL_AVX512F)
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x8i, vec16x32i, 0, vec16x32i{_mm512_cvtepi8_epi32(decay(v))});

        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x8i, vec8x64i, 0, vec8x64i{_mm512_cvtepi8_epi64(_mm_bsrli_si128(decay(v), 0x0))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x8i, vec8x64i, 1, vec8x64i{_mm512_cvtepi8_epi64(_mm_bsrli_si128(decay(v), 0x8))});
    #endif

    //=====================================================
    // Zero extensions from vec8x16i
    //=====================================================

    // Conversions to 128-bit vectors

    #if defined(AVEL_SSE2) && !defined(AVEL_SSE4_1)
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec8x16i, vec8x16i, 0, v);

        AVEL_SEXT_POLY_TO_POLY_IMPL(vec8x16i, vec4x32i, 0, vec4x32i{_mm_unpacklo_epi16(decay(v), _mm_cmplt_epi16(decay(v), _mm_setzero_si128()))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec8x16i, vec4x32i, 1, vec4x32i{_mm_unpackhi_epi16(decay(v), _mm_cmplt_epi16(decay(v), _mm_setzero_si128()))});

        AVEL_SEXT_POLY_TO_POLY_IMPL(vec8x16i, vec2x64i, 0, vec2x64i{_mm_unpacklo_epi32(_mm_unpacklo_epi16(decay(v), _mm_cmplt_epi32(decay(v), _mm_setzero_si128())), _mm_cmplt_epi32(decay(v), _mm_setzero_si128()))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec8x16i, vec2x64i, 1, vec2x64i{_mm_unpacklo_epi32(_mm_unpackhi_epi16(decay(v), _mm_cmplt_epi32(decay(v), _mm_setzero_si128())), _mm_cmplt_epi32(decay(v), _mm_setzero_si128()))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec8x16i, vec2x64i, 2, vec2x64i{_mm_unpackhi_epi32(_mm_unpacklo_epi16(decay(v), _mm_cmplt_epi32(decay(v), _mm_setzero_si128())), _mm_cmplt_epi32(decay(v), _mm_setzero_si128()))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec8x16i, vec2x64i, 3, vec2x64i{_mm_unpackhi_epi32(_mm_unpackhi_epi16(decay(v), _mm_cmplt_epi32(decay(v), _mm_setzero_si128())), _mm_cmplt_epi32(decay(v), _mm_setzero_si128()))});
    #endif

    #if defined(AVEL_SSE4_1)
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec8x16i, vec8x16i, 0, v);

        AVEL_SEXT_POLY_TO_POLY_IMPL(vec8x16i, vec4x32i, 0, vec4x32i{_mm_cvtepi16_epi32(decay(v))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec8x16i, vec4x32i, 1, vec4x32i{_mm_cvtepi16_epi32(decay(v))});

        AVEL_SEXT_POLY_TO_POLY_IMPL(vec8x16i, vec2x64i, 0, vec2x64i{_mm_cvtepi8_epi64(_mm_bsrli_si128(decay(v), 0x0))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec8x16i, vec2x64i, 1, vec2x64i{_mm_cvtepi8_epi64(_mm_bsrli_si128(decay(v), 0x2))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec8x16i, vec2x64i, 2, vec2x64i{_mm_cvtepi8_epi64(_mm_bsrli_si128(decay(v), 0x4))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec8x16i, vec2x64i, 3, vec2x64i{_mm_cvtepi8_epi64(_mm_bsrli_si128(decay(v), 0x6))});
    #endif

    // Conversions to 256-bit vectors

    #if defined(AVEL_AVX2)
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec8x16i, vec16x16i, 0, vec16x16i{_mm256_zextsi128_si256(decay(v))});

        AVEL_SEXT_POLY_TO_POLY_IMPL(vec8x16i, vec8x32i, 0, vec8x32i{_mm256_cvtepi16_epi32(decay(v))});

        AVEL_SEXT_POLY_TO_POLY_IMPL(vec8x16i, vec4x64i, 0, vec4x64i{_mm256_cvtepi16_epi64(decay(v))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec8x16i, vec4x64i, 1, vec4x64i{_mm256_cvtepi16_epi64(_mm_bsrli_si128(decay(v), 0x8))});
    #endif

    // Conversions to 512-bit vectors

    #if defined(AVEL_AVX512BW)
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec8x16i, vec32x16i, 0, vec32x16i{_mm512_zextsi128_si512(decay(v))});
    #endif

    #if defined(AVEL_AVX512F)
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec8x16i, vec16x32i, 0, vec16x32i{_mm512_zextsi256_si512(_mm256_cvtepi16_epi32(decay(v)))});

        AVEL_SEXT_POLY_TO_POLY_IMPL(vec8x16i, vec8x64i, 0, vec8x64i{_mm512_cvtepi16_epi64(decay(v))});
    #endif

    //=====================================================
    // Zero extensions from vec4x32i
    //=====================================================

    // Conversions to 128-bit vectors

    #if defined(AVEL_SSE2)
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec4x32i, vec4x32i, 0, v);

        AVEL_SEXT_POLY_TO_POLY_IMPL(vec4x32i, vec2x64i, 0, vec2x64i{_mm_unpacklo_epi32(decay(v), _mm_cmplt_epi32(decay(v), _mm_setzero_si128()))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec4x32i, vec2x64i, 1, vec2x64i{_mm_unpackhi_epi32(decay(v), _mm_cmplt_epi32(decay(v), _mm_setzero_si128()))});
    #endif

    // Conversions to 256-bit vectors

    #if defined(AVEL_AVX2)
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec4x32i, vec8x32i, 0, vec8x32i{_mm256_zextsi128_si256(decay(v))});

        AVEL_SEXT_POLY_TO_POLY_IMPL(vec4x32i, vec4x64i, 0, vec4x64i{_mm256_cvtepi32_epi64(decay(v))});
    #endif

    // Conversions to 512-bit vectors

    #if defined(AVEL_AVX512F)
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec4x32i, vec16x32i, 0, vec16x32i{_mm512_zextsi128_si512(decay(v))});

        AVEL_SEXT_POLY_TO_POLY_IMPL(vec4x32i, vec8x64i, 0, vec8x64i{_mm512_zextsi256_si512(_mm256_cvtepi32_epi64(decay(v)))});
    #endif

    //=====================================================
    // Zero extensions from vec2x64i
    //=====================================================

    // Conversions to 128-bit vectors

    #if defined(AVEL_SSE2)
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec2x64i, vec2x64i, 0, v);
    #endif

    // Conversions to 256-bit vectors

    #if defined(AVEL_AVX2)
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec2x64i, vec4x64i, 0, vec4x64i{_mm256_zextsi128_si256(decay(v))});
    #endif

    // Conversions to 512-bit vectors

    #if defined(AVEL_AVX512F)
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec2x64i, vec8x64i, 0, vec8x64i{_mm512_zextsi128_si512(decay(v))});
    #endif

    //=====================================================
    // Zero extensions from vec32x8i
    //=====================================================

    // Conversions to 256-bit vectors

    #if defined(AVEL_AVX2)
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec32x8i, vec32x8i, 0, v);

        AVEL_SEXT_POLY_TO_POLY_IMPL(vec32x8i, vec16x16i, 0, vec16x16i{_mm256_cvtepi8_epi16(_mm256_extractf128_si256(decay(v), 0x0))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec32x8i, vec16x16i, 1, vec16x16i{_mm256_cvtepi8_epi16(_mm256_extractf128_si256(decay(v), 0x1))});

        AVEL_SEXT_POLY_TO_POLY_IMPL(vec32x8i, vec8x32i, 0, vec8x32i{_mm256_cvtepi8_epi32(_mm256_extractf128_si256(decay(v), 0x0))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec32x8i, vec8x32i, 1, vec8x32i{_mm256_cvtepi8_epi32(_mm_bsrli_si128(_mm256_extractf128_si256(decay(v), 0x0), 0x8))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec32x8i, vec8x32i, 2, vec8x32i{_mm256_cvtepi8_epi32(_mm256_extractf128_si256(decay(v), 0x1))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec32x8i, vec8x32i, 3, vec8x32i{_mm256_cvtepi8_epi32(_mm_bsrli_si128(_mm256_extractf128_si256(decay(v), 0x1), 0x8))});

        AVEL_SEXT_POLY_TO_POLY_IMPL(vec32x8i, vec4x64i, 0, vec4x64i{_mm256_cvtepi8_epi64(_mm_bsrli_si128(_mm256_extractf128_si256(decay(v), 0x0), 0x0))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec32x8i, vec4x64i, 1, vec4x64i{_mm256_cvtepi8_epi64(_mm_bsrli_si128(_mm256_extractf128_si256(decay(v), 0x0), 0x4))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec32x8i, vec4x64i, 2, vec4x64i{_mm256_cvtepi8_epi64(_mm_bsrli_si128(_mm256_extractf128_si256(decay(v), 0x0), 0x8))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec32x8i, vec4x64i, 3, vec4x64i{_mm256_cvtepi8_epi64(_mm_bsrli_si128(_mm256_extractf128_si256(decay(v), 0x0), 0xc))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec32x8i, vec4x64i, 4, vec4x64i{_mm256_cvtepi8_epi64(_mm_bsrli_si128(_mm256_extractf128_si256(decay(v), 0x1), 0x0))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec32x8i, vec4x64i, 5, vec4x64i{_mm256_cvtepi8_epi64(_mm_bsrli_si128(_mm256_extractf128_si256(decay(v), 0x1), 0x4))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec32x8i, vec4x64i, 6, vec4x64i{_mm256_cvtepi8_epi64(_mm_bsrli_si128(_mm256_extractf128_si256(decay(v), 0x1), 0x8))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec32x8i, vec4x64i, 7, vec4x64i{_mm256_cvtepi8_epi64(_mm_bsrli_si128(_mm256_extractf128_si256(decay(v), 0x1), 0xc))});
    #endif

    // Conversions to 512-bit vectors

    #if defined(AVEL_AVX512BW)
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec32x8i, vec64x8i, 0, vec64x8i{_mm512_zextsi256_si512(decay(v))});

        AVEL_SEXT_POLY_TO_POLY_IMPL(vec32x8i, vec32x16i, 0, vec32x16i{_mm512_cvtepi8_epi16(decay(v))});
    #endif

    #if defined(AVEL_AVX512F)
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec32x8i, vec16x32i, 0, vec16x32i{_mm512_cvtepi8_epi32(_mm256_extractf128_si256(decay(v), 0x0))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec32x8i, vec16x32i, 1, vec16x32i{_mm512_cvtepi8_epi32(_mm256_extractf128_si256(decay(v), 0x1))});

        AVEL_SEXT_POLY_TO_POLY_IMPL(vec32x8i, vec8x64i, 0, vec8x64i{_mm512_cvtepi8_epi64(_mm256_extractf128_si256(decay(v), 0x0))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec32x8i, vec8x64i, 1, vec8x64i{_mm512_cvtepi8_epi64(_mm256_extractf128_si256(decay(v), 0x1))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec32x8i, vec8x64i, 2, vec8x64i{_mm512_cvtepi8_epi64(_mm256_extractf128_si256(decay(v), 0x0))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec32x8i, vec8x64i, 3, vec8x64i{_mm512_cvtepi8_epi64(_mm256_extractf128_si256(decay(v), 0x1))});
    #endif

    //=====================================================
    // Zero extensions from vec16x16i
    //=====================================================

    // Conversions to 256-bit vectors

    #if defined(AVEL_AVX2)
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x16i, vec16x16i, 0, v);

        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x16i, vec8x32i, 0, vec8x32i{_mm256_cvtepi16_epi32(_mm256_extracti128_si256(decay(v), 0x0))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x16i, vec8x32i, 1, vec8x32i{_mm256_cvtepi16_epi32(_mm256_extracti128_si256(decay(v), 0x1))});

        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x16i, vec4x64i, 0, vec4x64i{_mm256_cvtepi16_epi64(_mm256_extracti128_si256(decay(v), 0x0))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x16i, vec4x64i, 1, vec4x64i{_mm256_cvtepi16_epi64(_mm_bsrli_si128(_mm256_extracti128_si256(decay(v), 0x0), 0x8))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x16i, vec4x64i, 2, vec4x64i{_mm256_cvtepi16_epi64(_mm256_extracti128_si256(decay(v), 0x1))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x16i, vec4x64i, 3, vec4x64i{_mm256_cvtepi16_epi64(_mm_bsrli_si128(_mm256_extracti128_si256(decay(v), 0x1), 0x8))});
    #endif

    // Conversions to 512-bit vectors

    #if defined(AVEL_AVX512BW)
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x16i, vec32x16i, 0, vec32x16i{_mm512_zextsi256_si512(decay(v))});
    #endif

    #if defined(AVEL_AVX512F)
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x16i, vec16x32i, 0, vec16x32i{_mm512_cvtepi16_epi32(decay(v))});

        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x16i, vec8x64i, 0, vec8x64i{_mm512_cvtepi16_epi64(_mm256_extractf128_si256(decay(v), 0x0))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x16i, vec8x64i, 1, vec8x64i{_mm512_cvtepi16_epi64(_mm256_extractf128_si256(decay(v), 0x1))});
    #endif

    //=====================================================
    // Zero extensions from vec8x32i
    //=====================================================

    // Conversions to 256-bit vectors

    #if defined(AVEL_AVX2)
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec8x32i, vec8x32i, 0, v);

        AVEL_SEXT_POLY_TO_POLY_IMPL(vec8x32i, vec4x64i, 0, vec4x64i{_mm256_cvtepi32_epi64(_mm256_extractf128_si256(decay(v), 0x0))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec8x32i, vec4x64i, 1, vec4x64i{_mm256_cvtepi32_epi64(_mm256_extractf128_si256(decay(v), 0x1))});
    #endif

    // Conversions to 512-bit vectors

    #if defined(AVEL_AVX512F)
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec8x32i, vec16x32i, 0, vec16x32i{_mm512_zextsi256_si512(decay(v))});

        AVEL_SEXT_POLY_TO_POLY_IMPL(vec8x32i, vec8x64i, 0, vec8x64i{_mm512_cvtepi32_epi64(decay(v))});
    #endif

    //=====================================================
    // Zero extensions from vec4x64i
    //=====================================================

    // Conversions to 256-bit vectors

    #if defined(AVEL_AVX2)
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec4x64i, vec4x64i, 0, v);
    #endif

    // Conversions to 512-bit vectors

    #if defined(AVEL_AVX512F)
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec4x64i, vec8x64i, 0, vec8x64i{_mm512_zextsi256_si512(decay(v))});
    #endif

    //=====================================================
    // Zero extensions from vec64x8i
    //=====================================================

    #if defined(AVEL_AVX512BW)
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec64x8i, vec64x8i, 0, v);

        AVEL_SEXT_POLY_TO_POLY_IMPL(vec64x8i, vec32x16i, 0, vec32x16i{_mm512_cvtepi8_epi16(_mm512_extracti64x4_epi64(decay(v), 0x0))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec64x8i, vec32x16i, 1, vec32x16i{_mm512_cvtepi8_epi16(_mm512_extracti64x4_epi64(decay(v), 0x1))});

        AVEL_SEXT_POLY_TO_POLY_IMPL(vec64x8i, vec16x32i, 0, vec16x32i{_mm512_cvtepi8_epi32(_mm512_extracti64x2_epi64(decay(v), 0x0))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec64x8i, vec16x32i, 1, vec16x32i{_mm512_cvtepi8_epi32(_mm512_extracti64x2_epi64(decay(v), 0x1))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec64x8i, vec16x32i, 2, vec16x32i{_mm512_cvtepi8_epi32(_mm512_extracti64x2_epi64(decay(v), 0x2))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec64x8i, vec16x32i, 3, vec16x32i{_mm512_cvtepi8_epi32(_mm512_extracti64x2_epi64(decay(v), 0x3))});

        AVEL_SEXT_POLY_TO_POLY_IMPL(vec64x8i, vec8x64i, 0, vec8x64i{_mm512_cvtepi8_epi64(_mm512_extracti64x2_epi64(decay(v), 0x0))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec64x8i, vec8x64i, 1, vec8x64i{_mm512_cvtepi8_epi64(_mm_bsrli_si128(_mm512_extracti64x2_epi64(decay(v), 0x0), 0x1))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec64x8i, vec8x64i, 2, vec8x64i{_mm512_cvtepi8_epi64(_mm512_extracti64x2_epi64(decay(v), 0x1))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec64x8i, vec8x64i, 3, vec8x64i{_mm512_cvtepi8_epi64(_mm_bsrli_si128(_mm512_extracti64x2_epi64(decay(v), 0x1), 0x1))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec64x8i, vec8x64i, 4, vec8x64i{_mm512_cvtepi8_epi64(_mm512_extracti64x2_epi64(decay(v), 0x2))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec64x8i, vec8x64i, 5, vec8x64i{_mm512_cvtepi8_epi64(_mm_bsrli_si128(_mm512_extracti64x2_epi64(decay(v), 0x2), 0x1))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec64x8i, vec8x64i, 6, vec8x64i{_mm512_cvtepi8_epi64(_mm512_extracti64x2_epi64(decay(v), 0x3))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec64x8i, vec8x64i, 7, vec8x64i{_mm512_cvtepi8_epi64(_mm_bsrli_si128(_mm512_extracti64x2_epi64(decay(v), 0x3), 0x1))});
    #endif

    //=====================================================
    // Zero extensions from vec32x16i
    //=====================================================

    #if defined(AVEL_AVX512BW)
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec32x16i, vec32x16i, 0, v);

        AVEL_SEXT_POLY_TO_POLY_IMPL(vec32x16i, vec16x32i, 0, vec16x32i{_mm512_cvtepi16_epi32(_mm512_extracti64x4_epi64(decay(v), 0x0))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec32x16i, vec16x32i, 1, vec16x32i{_mm512_cvtepi16_epi32(_mm512_extracti64x4_epi64(decay(v), 0x1))});

        AVEL_SEXT_POLY_TO_POLY_IMPL(vec32x16i, vec8x64i, 0, vec8x64i{_mm512_cvtepi16_epi64(_mm512_extracti64x2_epi64(decay(v), 0x0))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec32x16i, vec8x64i, 1, vec8x64i{_mm512_cvtepi16_epi64(_mm512_extracti64x2_epi64(decay(v), 0x1))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec32x16i, vec8x64i, 2, vec8x64i{_mm512_cvtepi16_epi64(_mm512_extracti64x2_epi64(decay(v), 0x2))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec32x16i, vec8x64i, 3, vec8x64i{_mm512_cvtepi16_epi64(_mm512_extracti64x2_epi64(decay(v), 0x3))});
    #endif

    //=====================================================
    // Zero extensions from vec16x32i
    //=====================================================

    #if defined(AVEL_AVX512F)
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x32i, vec16x32i, 0, v);

        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x32i, vec8x64i, 0, vec8x64i{_mm512_cvtepi32_epi64(_mm512_extracti64x4_epi64(decay(v), 0x0))});
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec16x32i, vec8x64i, 1, vec8x64i{_mm512_cvtepi32_epi64(_mm512_extracti64x4_epi64(decay(v), 0x1))});
    #endif

    //=====================================================
    // Zero extensions from vec8x64i
    //=====================================================

    #if defined(AVEL_AVX512F)
        AVEL_SEXT_POLY_TO_POLY_IMPL(vec8x64i, vec8x64i, 0, v);
    #endif

}

#endif //AVEL_SIGN_EXTEND_HPP
