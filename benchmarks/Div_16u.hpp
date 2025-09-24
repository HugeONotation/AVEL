#ifndef AVEL_DIVISION_UINT16_HPP
#define AVEL_DIVISION_UINT16_HPP

namespace avel::benchmarks::div_16u {

    //=====================================================
    // scalar 16u benchmarks
    //=====================================================

    std::uint16_t scalar_native_impl(std::uint16_t x, std::uint16_t y) {
        return x / y;
    }

    auto scalar_native = scalar_division_test_bench<std::uint16_t, scalar_native_impl>;

    BENCHMARK(div_16u::scalar_native);



    std::uint16_t scalar_f32_div_impl(std::uint16_t x, std::uint16_t y) {
        return std::uint16_t(float(x) / float(y));
    }

    auto scalar_f32_div = scalar_division_test_bench<std::uint16_t, scalar_f32_div_impl>;

    BENCHMARK(div_16u::scalar_f32_div);

    //=====================================================
    // vec8x16u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec8x16u vec8x16u_generic_long_division_loop_impl(vec8x16u x, vec8x16u y) {
        // The lack of 8-bit shift instructions and native comparison operators means this can theoretically be made
        // better with careful attention
        vec8x16u quotient{};

        for (std::int32_t i = 16; (i-- > 0) && any(x);) {
            mask8x16u b = ((x >> i) >= y);
            x -= keep(b, y << i);
            quotient |= (vec8x16u{b} << i);
        }

        return quotient;
    }

    auto vec8x16u_generic_long_division_loop = vector_division_test_bench<vec8x16u, vec8x16u_generic_long_division_loop_impl>;

    BENCHMARK(div_16u::vec8x16u_generic_long_division_loop);

    #endif



    #if defined(AVEL_SSE4_1)

    vec8x16u vec8x16u_32f_div_normal_unpacking_impl(vec8x16u x, vec8x16u y) {
        __m128i x_raw = decay(x);

        __m128i x_h0 = _mm_unpacklo_epi8(x_raw, _mm_setzero_si128());
        __m128i x_h1 = _mm_unpackhi_epi8(x_raw, _mm_setzero_si128());

        __m128i y_raw = decay(y);

        __m128i y_h0 = _mm_unpacklo_epi8(y_raw, _mm_setzero_si128());
        __m128i y_h1 = _mm_unpackhi_epi8(y_raw, _mm_setzero_si128());

        __m128i quot_q0 = _mm_cvttps_epi32(_mm_div_ps(_mm_cvtepi32_ps(x_h0), _mm_cvtepi32_ps(y_h0)));
        __m128i quot_q1 = _mm_cvttps_epi32(_mm_div_ps(_mm_cvtepi32_ps(x_h1), _mm_cvtepi32_ps(y_h1)));

        __m128i quot = _mm_packus_epi32(quot_q0, quot_q1);

        return vec8x16u{quot};
    }

    auto vec8x16u_32f_div_normal_unpacking = vector_division_test_bench<vec8x16u, vec8x16u_32f_div_normal_unpacking_impl>;

    BENCHMARK(div_16u::vec8x16u_32f_div_normal_unpacking);

    #endif



    #if defined(AVEL_SSE4_1)

    vec8x16u vec8x16u_32f_div_subnormal_unpacking_impl(vec8x16u x, vec8x16u y) {
        __m128i x_raw = decay(x);

        __m128i x_lo = _mm_unpacklo_epi16(x_raw, _mm_setzero_si128());
        __m128i x_hi = _mm_unpackhi_epi16(x_raw, _mm_setzero_si128());

        __m128i y_raw = decay(y);

        __m128i y_lo = _mm_unpacklo_epi16(y_raw, _mm_setzero_si128());
        __m128i y_hi = _mm_unpackhi_epi16(y_raw, _mm_setzero_si128());

        __m128i quot_q0 = _mm_cvttps_epi32(_mm_div_ps(_mm_castsi128_ps(x_lo), _mm_castsi128_ps(y_lo)));
        __m128i quot_q1 = _mm_cvttps_epi32(_mm_div_ps(_mm_castsi128_ps(x_hi), _mm_castsi128_ps(y_hi)));

        __m128i quot = _mm_packus_epi32(quot_q0, quot_q1);
        return vec8x16u{quot};
    }

    auto vec8x16u_32f_div_subnormal_unpacking = vector_division_test_bench<vec8x16u, vec8x16u_32f_div_subnormal_unpacking_impl>;

    BENCHMARK(div_16u::vec8x16u_32f_div_subnormal_unpacking);

    #endif

    //=====================================================
    // vec16x16u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec16x16u vec16x16u_generic_long_division_loop_impl(vec16x16u x, vec16x16u y) {
        // The lack of 8-bit shift instructions and native comparison operators means this can theoretically be made
        // better with careful attention
        vec16x16u quotient{};

        for (std::int32_t i = 16; (i-- > 0) && any(x);) {
            mask16x16u b = ((x >> i) >= y);
            x -= keep(b, y << i);
            quotient |= (vec16x16u{b} << i);
        }

        return quotient;
    }

    auto vec16x16u_generic_long_division_loop = vector_division_test_bench<vec16x16u, vec16x16u_generic_long_division_loop_impl>;

    BENCHMARK(div_16u::vec16x16u_generic_long_division_loop);

    #endif



    #if defined(AVEL_AVX2)

    vec16x16u vec16x16u_32f_div_subnormal_unpacking_impl(vec16x16u x, vec16x16u y) {
        __m256i x_raw = decay(x);

        __m256i x_lo = _mm256_unpacklo_epi16(x_raw, _mm256_setzero_si256());
        __m256i x_hi = _mm256_unpackhi_epi16(x_raw, _mm256_setzero_si256());

        __m256i y_raw = decay(y);

        __m256i y_lo = _mm256_unpacklo_epi16(y_raw, _mm256_setzero_si256());
        __m256i y_hi = _mm256_unpackhi_epi16(y_raw, _mm256_setzero_si256());

        __m256i quot_q0 = _mm256_cvttps_epi32(_mm256_div_ps(_mm256_castsi256_ps(x_lo), _mm256_castsi256_ps(y_lo)));
        __m256i quot_q1 = _mm256_cvttps_epi32(_mm256_div_ps(_mm256_castsi256_ps(x_hi), _mm256_castsi256_ps(y_hi)));

        __m256i quot = _mm256_packus_epi32(quot_q0, quot_q1);
        return vec16x16u{quot};
    }

    auto vec16x16u_32f_div_subnormal_unpacking = vector_division_test_bench<vec16x16u, vec16x16u_32f_div_subnormal_unpacking_impl>;

    BENCHMARK(div_16u::vec16x16u_32f_div_subnormal_unpacking);

    #endif



    #if defined(AVEL_AVX2)

    vec16x16u vec16x16u_32f_div_normal_unpacking_impl(vec16x16u x, vec16x16u y) {
        __m256i x_raw = decay(x);

        __m256i x_h0 = _mm256_unpacklo_epi8(x_raw, _mm256_setzero_si256());
        __m256i x_h1 = _mm256_unpackhi_epi8(x_raw, _mm256_setzero_si256());

        __m256i y_raw = decay(y);

        __m256i y_h0 = _mm256_unpacklo_epi8(y_raw, _mm256_setzero_si256());
        __m256i y_h1 = _mm256_unpackhi_epi8(y_raw, _mm256_setzero_si256());

        __m256i quot_q0 = _mm256_cvttps_epi32(_mm256_div_ps(_mm256_cvtepi32_ps(x_h0), _mm256_cvtepi32_ps(y_h0)));
        __m256i quot_q1 = _mm256_cvttps_epi32(_mm256_div_ps(_mm256_cvtepi32_ps(x_h1), _mm256_cvtepi32_ps(y_h1)));

        __m256i quot = _mm256_packus_epi32(quot_q0, quot_q1);

        return vec16x16u{quot};
    }

    auto vec16x16u_32f_div_normal_unpacking = vector_division_test_bench<vec16x16u, vec16x16u_32f_div_normal_unpacking_impl>;

    BENCHMARK(div_16u::vec16x16u_32f_div_normal_unpacking);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)

    vec16x16u vec16x16u_32f_div_normal_kmasking_impl(vec16x16u x, vec16x16u y) {
        const __mmask16 mask0 = 0x5555;

        __m256i x_q0 = _mm256_maskz_mov_epi16(mask0, decay(x));
        __m256i x_q1 = _mm256_srli_epi32(decay(x), 16);

        __m256i y_q0 = _mm256_maskz_mov_epi16(mask0, decay(y));
        __m256i y_q1 = _mm256_srli_epi32(decay(y), 16);

        __m256i quot_q0 = _mm256_cvttps_epi32(_mm256_div_ps(_mm256_cvtepi32_ps(x_q0), _mm256_cvtepi32_ps(y_q0)));
        __m256i quot_q1 = _mm256_cvttps_epi32(_mm256_div_ps(_mm256_cvtepi32_ps(x_q1), _mm256_cvtepi32_ps(y_q1)));

        __m256i quot = _mm256_or_si256(quot_q0, _mm256_slli_epi32(quot_q1, 16));

        return vec16x16u{quot};
    }

    auto vec16x16u_32f_div_normal_kmasking = vector_division_test_bench<vec16x16u, vec16x16u_32f_div_normal_kmasking_impl>;

    BENCHMARK(div_16u::vec16x16u_32f_div_normal_kmasking);

    #endif

    //=====================================================
    // vec32x16u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512BW)

    vec32x16u vec32x16u_generic_long_division_loop_impl(vec32x16u x, vec32x16u y) {
        // The lack of 8-bit shift instructions and native comparison operators means this can theoretically be made
        // better with careful attention
        vec32x16u quotient{};

        for (std::int32_t i = 16; (i-- > 0) && any(x);) {
            mask32x16u b = ((x >> i) >= y);
            x -= keep(b, y << i);
            quotient |= (vec32x16u{b} << i);
        }

        return quotient;
    }

    auto vec32x16u_generic_long_division_loop = vector_division_test_bench<vec32x16u, vec32x16u_generic_long_division_loop_impl>;

    BENCHMARK(div_16u::vec32x16u_generic_long_division_loop);

    #endif



    #if defined(AVEL_AVX512BW)

    vec32x16u vec32x16u_32f_div_subnormal_unpacking_impl(vec32x16u x, vec32x16u y) {
        __m512i x_raw = decay(x);

        __m512i x_lo = _mm512_unpacklo_epi16(x_raw, _mm512_setzero_si512());
        __m512i x_hi = _mm512_unpackhi_epi16(x_raw, _mm512_setzero_si512());

        __m512i y_raw = decay(y);

        __m512i y_lo = _mm512_unpacklo_epi16(y_raw, _mm512_setzero_si512());
        __m512i y_hi = _mm512_unpackhi_epi16(y_raw, _mm512_setzero_si512());

        __m512i quot_q0 = _mm512_cvttps_epi32(_mm512_div_ps(_mm512_castsi512_ps(x_lo), _mm512_castsi512_ps(y_lo)));
        __m512i quot_q1 = _mm512_cvttps_epi32(_mm512_div_ps(_mm512_castsi512_ps(x_hi), _mm512_castsi512_ps(y_hi)));

        __m512i quot = _mm512_packus_epi32(quot_q0, quot_q1);
        return vec32x16u{quot};
    }

    auto vec32x16u_32f_div_subnormal_unpacking = vector_division_test_bench<vec32x16u, vec32x16u_32f_div_subnormal_unpacking_impl>;

    BENCHMARK(div_16u::vec32x16u_32f_div_subnormal_unpacking);

    #endif



    #if defined(AVEL_AVX512BW)

    vec32x16u vec32x16u_32f_div_normal_unpacking_impl(vec32x16u x, vec32x16u y) {
        __m512i x_raw = decay(x);

        __m512i x_h0 = _mm512_unpacklo_epi8(x_raw, _mm512_setzero_si512());
        __m512i x_h1 = _mm512_unpackhi_epi8(x_raw, _mm512_setzero_si512());

        __m512i y_raw = decay(y);

        __m512i y_h0 = _mm512_unpacklo_epi8(y_raw, _mm512_setzero_si512());
        __m512i y_h1 = _mm512_unpackhi_epi8(y_raw, _mm512_setzero_si512());

        __m512i quot_q0 = _mm512_cvttps_epi32(_mm512_div_ps(_mm512_cvtepi32_ps(x_h0), _mm512_cvtepi32_ps(y_h0)));
        __m512i quot_q1 = _mm512_cvttps_epi32(_mm512_div_ps(_mm512_cvtepi32_ps(x_h1), _mm512_cvtepi32_ps(y_h1)));

        __m512i quot = _mm512_packus_epi32(quot_q0, quot_q1);

        return vec32x16u{quot};
    }

    auto vec32x16u_32f_div_normal_unpacking = vector_division_test_bench<vec32x16u, vec32x16u_32f_div_normal_unpacking_impl>;

    BENCHMARK(div_16u::vec32x16u_32f_div_normal_unpacking);

    #endif



    #if defined(AVEL_AVX512BW)

    vec32x16u vec32x16u_32f_div_normal_kmasking_impl(vec32x16u x, vec32x16u y) {
        const __mmask32 mask0 = 0x55555555;

        __m512i x_q0 = _mm512_maskz_mov_epi16(mask0, decay(x));
        __m512i x_q1 = _mm512_srli_epi32(decay(x), 16);

        __m512i y_q0 = _mm512_maskz_mov_epi16(mask0, decay(y));
        __m512i y_q1 = _mm512_srli_epi32(decay(y), 16);

        __m512i quot_q0 = _mm512_cvttps_epi32(_mm512_div_ps(_mm512_cvtepi32_ps(x_q0), _mm512_cvtepi32_ps(y_q0)));
        __m512i quot_q1 = _mm512_cvttps_epi32(_mm512_div_ps(_mm512_cvtepi32_ps(x_q1), _mm512_cvtepi32_ps(y_q1)));

        __m512i quot = _mm512_or_si512(quot_q0, _mm512_slli_epi32(quot_q1, 16));

        return vec32x16u{quot};
    }

    auto vec32x16u_32f_div_normal_kmasking = vector_division_test_bench<vec32x16u, vec32x16u_32f_div_normal_kmasking_impl>;

    BENCHMARK(div_16u::vec32x16u_32f_div_normal_kmasking);

    #endif

}

#endif //AVEL_DIVISION_UINT16_HPP
