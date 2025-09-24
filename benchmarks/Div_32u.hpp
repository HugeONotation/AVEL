#ifndef AVEL_DIVISION_UINT32_HPP
#define AVEL_DIVISION_UINT32_HPP

namespace avel::benchmarks::div_32u {

    //=====================================================
    // scalar 32u benchmarks
    //=====================================================

    std::uint32_t scalar_native_impl(std::uint32_t x, std::uint32_t y) {
        return x / y;
    }

    auto scalar_native = scalar_division_test_bench<std::uint32_t, scalar_native_impl>;

    BENCHMARK(div_32u::scalar_native);



    std::uint32_t scalar_f64_div_subnormal_impl(std::uint32_t x, std::uint32_t y) {
        return std::uint32_t(avel::bit_cast<double>(std::uint64_t(x)) / std::bit_cast<double>(std::uint64_t(y)));
    }

    auto scalar_f64_div_subnormal = scalar_division_test_bench<std::uint32_t, scalar_f64_div_subnormal_impl>;

    BENCHMARK(div_32u::scalar_f64_div_subnormal);



    std::uint32_t scalar_f64_div_impl(std::uint32_t x, std::uint32_t y) {
        return std::uint32_t(double(x) / double(y));
    }

    auto scalar_f64_div = scalar_division_test_bench<std::uint32_t, scalar_f64_div_impl>;

    BENCHMARK(div_32u::scalar_f64_div);

    //=====================================================
    // vec4x32u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec4x32u vec4x32u_64f_div_subnormal_unpacking_impl(vec4x32u x, vec4x32u y) {
        __m128i x_raw = decay(x);

        __m128i x_lo = _mm_unpacklo_epi32(x_raw, _mm_setzero_si128());
        __m128i x_hi = _mm_unpackhi_epi32(x_raw, _mm_setzero_si128());

        __m128i y_raw = decay(y);

        __m128i y_lo = _mm_unpacklo_epi32(y_raw, _mm_setzero_si128());
        __m128i y_hi = _mm_unpackhi_epi32(y_raw, _mm_setzero_si128());

        //TODO: Correct conversion. It should be to a unsigned 32-bit ints, not this.
        __m128i quot_q0 = _mm_cvttpd_epi32(_mm_div_pd(_mm_castsi128_pd(x_lo), _mm_castsi128_pd(y_lo)));
        __m128i quot_q1 = _mm_cvttpd_epi32(_mm_div_pd(_mm_castsi128_pd(x_hi), _mm_castsi128_pd(y_hi)));

        __m128i quot = _mm_castps_si128(
            _mm_shuffle_ps(
                _mm_castsi128_ps(quot_q0),
                _mm_castsi128_ps(quot_q1),
                0x88
            )
        );

        return vec4x32u{quot};
    }

    auto vec4x32u_32f_div_subnormal_unpacking = vector_division_test_bench<vec4x32u, vec4x32u_64f_div_subnormal_unpacking_impl>;

    BENCHMARK(div_32u::vec4x32u_32f_div_subnormal_unpacking);

    #endif


    //TODO: Consider using i32->f64 conversion from SSE4.1


    #if defined(AVEL_AVX512VL)

    vec4x32u vec4x32u_32f_div_cvt_widening_impl(vec4x32u x, vec4x32u y) {
        __m256d xd = _mm256_cvtepu32_pd(decay(x));
        __m256d yd = _mm256_cvtepu32_pd(decay(y));

        __m128i quot = _mm256_cvttpd_epu32(_mm256_div_pd(xd, yd));

        return vec4x32u{quot};
    }

    auto vec4x32u_32f_div_cvt_widening = vector_division_test_bench<vec4x32u, vec4x32u_32f_div_cvt_widening_impl>;

    BENCHMARK(div_32u::vec4x32u_32f_div_cvt_widening);

    #endif

    //=====================================================
    // vec8x32u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512VL)

    vec8x32u vec8x32u_32f_div_cvt_widening_impl(vec8x32u x, vec8x32u y) {
        __m512d xd = _mm512_cvtepu32_pd(decay(x));
        __m512d yd = _mm512_cvtepu32_pd(decay(y));

        __m256i quot = _mm512_cvttpd_epu32(_mm512_div_pd(xd, yd));

        return vec8x32u{quot};
    }

    auto vec8x32u_32f_div_cvt_widening = vector_division_test_bench<vec8x32u, vec8x32u_32f_div_cvt_widening_impl>;

    BENCHMARK(div_32u::vec8x32u_32f_div_cvt_widening);

    #endif

    //=====================================================
    // vec16x32u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512F)

    vec16x32u vec16x32u_32f_div_cvt_impl(vec16x32u x, vec16x32u y) {
        __m512i x_raw = decay(x);

        __m512d x_lo = _mm512_cvtepu32_pd(_mm512_extracti64x4_epi64(x_raw, 0x0));
        __m512d x_hi = _mm512_cvtepu32_pd(_mm512_extracti64x4_epi64(x_raw, 0x1));

        __m512i y_raw = decay(y);

        __m512d y_lo = _mm512_cvtepu32_pd(_mm512_extracti64x4_epi64(y_raw, 0x0));
        __m512d y_hi = _mm512_cvtepu32_pd(_mm512_extracti64x4_epi64(y_raw, 0x1));

        __m256i quot_h0 = _mm512_cvttpd_epu32(_mm512_div_pd(x_lo, y_lo));
        __m256i quot_h1 = _mm512_cvttpd_epu32(_mm512_div_pd(x_hi, y_hi));

        __m512i quot = _mm512_inserti64x4(_mm512_castsi256_si512(quot_h0), quot_h1, 0x1);

        return vec16x32u{quot};
    }

    auto vec16x32u_32f_div_cvt = vector_division_test_bench<vec16x32u, vec16x32u_32f_div_cvt_impl>;

    BENCHMARK(div_32u::vec16x32u_32f_div_cvt);

    #endif



    #if defined(AVEL_AVX512DQ)

    vec16x32u vec16x32u_32f_div_subnormal_unpacking_dq_impl(vec16x32u x, vec16x32u y) {
        __m512i x_raw = decay(x);

        __m512i x_lo = _mm512_unpacklo_epi32(x_raw, _mm512_setzero_si512());
        __m512i x_hi = _mm512_unpackhi_epi32(x_raw, _mm512_setzero_si512());

        __m512i y_raw = decay(y);

        __m512i y_lo = _mm512_unpacklo_epi32(y_raw, _mm512_setzero_si512());
        __m512i y_hi = _mm512_unpackhi_epi32(y_raw, _mm512_setzero_si512());

        __m512i quot_q0 = _mm512_cvttpd_epi64(_mm512_div_pd(_mm512_castsi512_pd(x_lo), _mm512_castsi512_pd(y_lo)));
        __m512i quot_q1 = _mm512_cvttpd_epi64(_mm512_div_pd(_mm512_castsi512_pd(x_hi), _mm512_castsi512_pd(y_hi)));

        __m512i quot = _mm512_castps_si512(
            _mm512_shuffle_ps(
                _mm512_castsi512_ps(quot_q0),
                _mm512_castsi512_ps(quot_q1),
                0x88
            )
        );

        return vec16x32u{quot};
    }

    auto vec16x32u_32f_div_subnormal_unpacking_dq = vector_division_test_bench<vec16x32u, vec16x32u_32f_div_subnormal_unpacking_dq_impl>;

    BENCHMARK(div_32u::vec16x32u_32f_div_subnormal_unpacking_dq);

    #endif



    #if defined(AVEL_AVX512DQ)

    vec16x32u vec16x32u_32f_div_normal_unpacking_dq_impl(vec16x32u x, vec16x32u y) {
        __m512i x_raw = decay(x);

        __m512i x_h0 = _mm512_unpacklo_epi32(x_raw, _mm512_setzero_si512());
        __m512i x_h1 = _mm512_unpackhi_epi32(x_raw, _mm512_setzero_si512());

        __m512i y_raw = decay(y);

        __m512i y_h0 = _mm512_unpacklo_epi32(y_raw, _mm512_setzero_si512());
        __m512i y_h1 = _mm512_unpackhi_epi32(y_raw, _mm512_setzero_si512());

        __m512i quot_q0 = _mm512_cvttpd_epi64(_mm512_div_pd(_mm512_cvtepi64_pd(x_h0), _mm512_cvtepi64_pd(y_h0)));
        __m512i quot_q1 = _mm512_cvttpd_epi64(_mm512_div_pd(_mm512_cvtepi64_pd(x_h1), _mm512_cvtepi64_pd(y_h1)));

        __m512i quot = _mm512_castps_si512(
            _mm512_shuffle_ps(
                _mm512_castsi512_ps(quot_q0),
                _mm512_castsi512_ps(quot_q1),
                0x88
            )
        );

        return vec16x32u{quot};
    }

    auto vec16x32u_32f_div_normal_unpacking_dq = vector_division_test_bench<vec16x32u, vec16x32u_32f_div_normal_unpacking_dq_impl>;

    BENCHMARK(div_32u::vec16x32u_32f_div_normal_unpacking_dq);

    #endif



    #if defined(AVEL_AVX512DQ)

    vec16x32u vec16x32u_32f_div_normal_kmasking_dq_impl(vec16x32u x, vec16x32u y) {
        const __mmask16 mask0 = 0x5555;

        __m512i x_q0 = _mm512_maskz_mov_epi32(mask0, decay(x));
        __m512i x_q1 = _mm512_srli_epi64(decay(x), 32);

        __m512i y_q0 = _mm512_maskz_mov_epi32(mask0, decay(y));
        __m512i y_q1 = _mm512_srli_epi64(decay(y), 32);

        __m512i quot_q0 = _mm512_cvttpd_epi64(_mm512_div_pd(_mm512_cvtepi64_pd(x_q0), _mm512_cvtepi64_pd(y_q0)));
        __m512i quot_q1 = _mm512_cvttpd_epi64(_mm512_div_pd(_mm512_cvtepi64_pd(x_q1), _mm512_cvtepi64_pd(y_q1)));

        __m512i quot = _mm512_or_si512(quot_q0, _mm512_slli_epi64(quot_q1, 32));

        return vec16x32u{quot};
    }

    auto vec16x32u_32f_div_normal_kmasking_dq = vector_division_test_bench<vec16x32u, vec16x32u_32f_div_normal_kmasking_dq_impl>;

    BENCHMARK(div_32u::vec16x32u_32f_div_normal_kmasking_dq);

    #endif

}

#endif //AVEL_DIVISION_UINT32_HPP
