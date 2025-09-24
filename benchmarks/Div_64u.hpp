#ifndef AVEL_DIVISION_64U_HPP
#define AVEL_DIVISION_64U_HPP

namespace avel::benchmarks::div_64u {

    //TODO: Consider approaches that use double-precision division using for
    // 52-bit inputs and use a faster approach otherwise

    //=====================================================
    // scalar 64u benchmarks
    //=====================================================

    std::uint64_t scalar_native_impl(std::uint64_t x, std::uint64_t y) {
        return x / y;
    }

    auto scalar_native = scalar_test_bench<std::uint64_t, scalar_native_impl>;

    BENCHMARK(div_64u::scalar_native);

    //=====================================================
    // vec2x64u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec2x64u vec2x64u_scalarization_sse2_impl(vec2x64u v, vec2x64u w) {
        std::uint64_t v0 = _mm_cvtsi128_si64x(decay(v));
        std::uint64_t v1 = _mm_cvtsi128_si64x(_mm_bsrli_si128(decay(v), 8));

        std::uint64_t w0 = _mm_cvtsi128_si64x(decay(w));
        std::uint64_t w1 = _mm_cvtsi128_si64x(_mm_bsrli_si128(decay(w), 8));

        std::uint64_t q0 = v0 / w0;
        std::uint64_t q1 = v1 / w1;

        return vec2x64u{_mm_set_epi64x(q1, q0)};
    }

    auto vec2x64u_scalarization_sse2 = vector_division_test_bench<vec2x64u, vec2x64u_scalarization_sse2_impl>;

    BENCHMARK(div_64u::vec2x64u_scalarization_sse2);

    #endif



    /*
    #if defined(AVEL_SSE2)

    vec2x64u vec2x64u_hot_divpd_cold_scalar_fallback_impl(vec2x64u v, vec2x64u w) {
        __m128i t0 = _mm_or_si128(decay(v), decay(w));
        __m128i t1 = _mm_srli_epi64(t0, 52);
        __m128i t2 = _mm_cmpeq_epi32(t1, _mm_setzero_si128());
        bool use_hot_path = (0xffff == _mm_movemask_epi8(t2));

        if (use_hot_path) {
            __m128d n = _mm_setzero_pd();
            __m128d d = _mm_setzero_pd();

            __m128d q = _mm_div_pd(n, d);

            

            return vec2x64u{};
        } else {

        }
    }

    auto vec2x64u_hot_divpd_cold_scalar_fallback = vector_division_test_bench<vec2x64u, vec2x64u_hot_divpd_cold_scalar_fallback_impl>;

    BENCHMARK(div_64u::vec2x64u_hot_divpd_cold_scalar_fallback);

    #endif
    */



    #if defined(AVEL_SSE4_1)

    vec2x64u vec2x64u_scalarization_sse4_1_impl(vec2x64u v, vec2x64u w) {
        std::uint64_t v0 = _mm_cvtsi128_si64x(decay(v));
        std::uint64_t v1 = _mm_extract_epi64(decay(v), 1);

        std::uint64_t w0 = _mm_cvtsi128_si64x(decay(w));
        std::uint64_t w1 = _mm_extract_epi64(decay(w), 1);

        std::uint64_t q0 = v0 / w0;
        std::uint64_t q1 = v1 / w1;

        return vec2x64u{_mm_set_epi64x(q1, q0)};
    }

    auto vec2x64u_scalarization_sse4_1 = vector_division_test_bench<vec2x64u, vec2x64u_scalarization_sse4_1_impl>;

    BENCHMARK(div_64u::vec2x64u_scalarization_sse4_1);

    #endif



    /*
    #if defined(AVEL_SSE4_1)

    vec2x64u vec2x64u_53_bit_special_handling_sse4_1_impl(vec2x64u v, vec2x64u w) {
        const __m128i hi_11_bit_mask = _mm_set1_epi64x(0xffe0000000000000ull);
        const __m128i masked_input_bits = _mm_and_si128(hi_11_bit_mask, _mm_or_si128(decay(v), decay(w)));
        const __m128i comparison_result = _mm_cmpeq_epi32(masked_input_bits, _mm_setzero_si128());

        bool is_f64_division_sufficient = (0xffff == _mm_movemask_epi8(comparison_result));

        if (is_f64_division_sufficient) {
            __m128i numerator = decay(v);
            __m128i denominator = decay(w);

            __m128i undef = _mm_undefined_si128();
            __m128i nf64 = _mm_cvtsi64x_sd(undef, );

        } else {
            std::uint64_t v0 = _mm_cvtsi128_si64x(decay(v));
            std::uint64_t v1 = _mm_extract_epi64(decay(v), 1);

            std::uint64_t w0 = _mm_cvtsi128_si64x(decay(w));
            std::uint64_t w1 = _mm_extract_epi64(decay(w), 1);

            std::uint64_t q0 = v0 / w0;
            std::uint64_t q1 = v1 / w1;

            return vec2x64u{_mm_set_epi64x(q1, q0)};
        }
    }

    //auto vec2x64u_53_bit_special_handling_sse4_1 = vector_division_test_bench<vec2x64u, vec2x64u_53_bit_special_handling_sse4_1_impl>;

    //BENCHMARK(div_64u::vec2x64u_53_bit_special_handling_sse4_1);

    #endif
    */



    /*
    #if defined(AVEL_SSE2)

    vec2x64u vec2x64u_division_64f_impl(vec2x64u v, vec2x64u w) {
        //TODO: Implement
        return {};
    }

    auto vec2x64u_division_64f = vector_division_test_bench<vec2x64u, vec2x64u_division_64f_impl>;

    BENCHMARK(div_64u::vec2x64u_division_64f);

    #endif
    */

    #if defined(AVEL_AVX512VL) & defined(AVEL_AVX512DQ)

    vec2x64u vec2x64u_division_64f_impl(vec2x64u n, vec2x64u d) {
        constexpr std::uint8_t mask = 0x3;

        __m128i a = decay(n);
        __m128i b = decay(d);

        __m128d a_f64 = _mm512_castpd512_pd128(_mm512_maskz_cvt_roundepu64_pd(mask, _mm512_castsi128_si512(decay(n)), _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC));
        __m128d b_f64 = _mm512_castpd512_pd128(_mm512_maskz_cvt_roundepu64_pd(mask, _mm512_castsi128_si512(decay(d)), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC));
        __m128d b_rcp = _mm512_castpd512_pd128(_mm512_maskz_div_round_pd(mask, _mm512_castpd128_pd512(_mm_set1_pd(1.0)), _mm512_castpd128_pd512(b_f64), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC));

        __m128i first = _mm512_castsi512_si128(_mm512_maskz_cvt_roundpd_epu64(mask, _mm512_maskz_mul_round_pd(mask, _mm512_castpd128_pd512(a_f64), _mm512_castpd128_pd512(b_rcp), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC));

        a = _mm_sub_epi64(a, _mm_mullo_epi64(first, b));
        a_f64 = _mm512_castpd512_pd128(_mm512_maskz_cvt_roundepu64_pd(mask, _mm512_castsi128_si512(a), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC));

        __m128i second = _mm512_castsi512_si128(_mm512_maskz_cvt_roundpd_epu64(mask, _mm512_maskz_mul_round_pd(mask, _mm512_castpd128_pd512(a_f64), _mm512_castpd128_pd512(b_rcp), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC));

        __m128i result = _mm_add_epi64(first, second);

        a = _mm_sub_epi64(a, _mm_mullo_epi64(second, b));

        result = _mm_mask_add_epi64(result, _mm_cmp_epu64_mask(a, b, _MM_CMPINT_NLT), result, _mm_set1_epi64x(1));

        return vec2x64u{result};
    }

    auto vec2x64u_division_64f = vector_division_test_bench<vec2x64u, vec2x64u_division_64f_impl>;

    BENCHMARK(div_64u::vec2x64u_division_64f);

    #endif

    //=====================================================
    // vec4x64u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec4x64u vec4x64u_scalarization_impl(vec4x64u v, vec4x64u w) {
        std::uint64_t v0 = _mm256_extract_epi64(decay(v), 0);
        std::uint64_t v1 = _mm256_extract_epi64(decay(v), 1);
        std::uint64_t v2 = _mm256_extract_epi64(decay(v), 2);
        std::uint64_t v3 = _mm256_extract_epi64(decay(v), 3);

        std::uint64_t w0 = _mm256_extract_epi64(decay(w), 0);
        std::uint64_t w1 = _mm256_extract_epi64(decay(w), 1);
        std::uint64_t w2 = _mm256_extract_epi64(decay(w), 2);
        std::uint64_t w3 = _mm256_extract_epi64(decay(w), 3);

        std::uint64_t q0 = v0 / w0;
        std::uint64_t q1 = v1 / w1;
        std::uint64_t q2 = v2 / w2;
        std::uint64_t q3 = v3 / w3;

        return vec4x64u{_mm256_set_epi64x(q3, q2, q1, q0)};
    }

    auto vec4x64u_scalarization = vector_division_test_bench<vec4x64u, vec4x64u_scalarization_impl>;

    BENCHMARK(div_64u::vec4x64u_scalarization);

    #endif



    #if defined(AVEL_AVX512VL) & defined(AVEL_AVX512DQ)

    vec4x64u vec4x64u_division_64f_impl(vec4x64u n, vec4x64u d) {
        constexpr std::uint8_t mask = 0xf;

        __m256i a = decay(n);
        __m256i b = decay(d);

        __m256d a_f64 = _mm512_castpd512_pd256(_mm512_maskz_cvt_roundepu64_pd(mask, _mm512_castsi256_si512(decay(n)), _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC));
        __m256d b_f64 = _mm512_castpd512_pd256(_mm512_maskz_cvt_roundepu64_pd(mask, _mm512_castsi256_si512(decay(d)), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC));
        __m256d b_rcp = _mm512_castpd512_pd256(_mm512_maskz_div_round_pd(mask, _mm512_castpd256_pd512(_mm256_set1_pd(1.0)), _mm512_castpd256_pd512(b_f64), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC));

        __m256i first = _mm512_castsi512_si256(_mm512_maskz_cvt_roundpd_epu64(mask, _mm512_maskz_mul_round_pd(mask, _mm512_castpd256_pd512(a_f64), _mm512_castpd256_pd512(b_rcp), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC));

        a = _mm256_sub_epi64(a, _mm256_mullo_epi64(first, b));
        a_f64 = _mm512_castpd512_pd256(_mm512_maskz_cvt_roundepu64_pd(mask, _mm512_castsi256_si512(a), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC));

        __m256i second = _mm512_castsi512_si256(_mm512_maskz_cvt_roundpd_epu64(mask, _mm512_maskz_mul_round_pd(mask, _mm512_castpd256_pd512(a_f64), _mm512_castpd256_pd512(b_rcp), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC));

        __m256i result = _mm256_add_epi64(first, second);

        a = _mm256_sub_epi64(a, _mm256_mullo_epi64(second, b));

        result = _mm256_mask_add_epi64(result, _mm256_cmp_epu64_mask(a, b, _MM_CMPINT_NLT), result, _mm256_set1_epi64x(1));

        return vec4x64u{result};
    }

    auto vec4x64u_division_64f = vector_division_test_bench<vec4x64u, vec4x64u_division_64f_impl>;

    BENCHMARK(div_64u::vec4x64u_division_64f);

    #endif

    //=====================================================
    // vec8x64u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512F)

    vec8x64u vec8x64u_scalarization_impl(vec8x64u v, vec8x64u w) {
        std::uint64_t v0 = _mm_extract_epi64(_mm512_extracti32x4_epi32(decay(v), 0), 0);
        std::uint64_t v1 = _mm_extract_epi64(_mm512_extracti32x4_epi32(decay(v), 0), 1);
        std::uint64_t v2 = _mm_extract_epi64(_mm512_extracti32x4_epi32(decay(v), 1), 0);
        std::uint64_t v3 = _mm_extract_epi64(_mm512_extracti32x4_epi32(decay(v), 1), 1);
        std::uint64_t v4 = _mm_extract_epi64(_mm512_extracti32x4_epi32(decay(v), 2), 0);
        std::uint64_t v5 = _mm_extract_epi64(_mm512_extracti32x4_epi32(decay(v), 2), 1);
        std::uint64_t v6 = _mm_extract_epi64(_mm512_extracti32x4_epi32(decay(v), 3), 0);
        std::uint64_t v7 = _mm_extract_epi64(_mm512_extracti32x4_epi32(decay(v), 3), 1);

        std::uint64_t w0 = _mm_extract_epi64(_mm512_extracti32x4_epi32(decay(w), 0), 0);
        std::uint64_t w1 = _mm_extract_epi64(_mm512_extracti32x4_epi32(decay(w), 0), 1);
        std::uint64_t w2 = _mm_extract_epi64(_mm512_extracti32x4_epi32(decay(w), 1), 0);
        std::uint64_t w3 = _mm_extract_epi64(_mm512_extracti32x4_epi32(decay(w), 1), 1);
        std::uint64_t w4 = _mm_extract_epi64(_mm512_extracti32x4_epi32(decay(w), 2), 0);
        std::uint64_t w5 = _mm_extract_epi64(_mm512_extracti32x4_epi32(decay(w), 2), 1);
        std::uint64_t w6 = _mm_extract_epi64(_mm512_extracti32x4_epi32(decay(w), 3), 0);
        std::uint64_t w7 = _mm_extract_epi64(_mm512_extracti32x4_epi32(decay(w), 3), 1);

        std::uint64_t q0 = v0 / w0;
        std::uint64_t q1 = v1 / w1;
        std::uint64_t q2 = v2 / w2;
        std::uint64_t q3 = v3 / w3;
        std::uint64_t q4 = v4 / w4;
        std::uint64_t q5 = v5 / w5;
        std::uint64_t q6 = v6 / w6;
        std::uint64_t q7 = v7 / w7;

        return vec8x64u{_mm512_set_epi64(q7, q6, q5, q4, q3, q2, q1, q0)};
    }

    auto vec8x64u_scalarization = vector_division_test_bench<vec8x64u, vec8x64u_scalarization_impl>;

    BENCHMARK(div_64u::vec8x64u_scalarization);

    #endif



    #if defined(AVEL_AVX512DQ)

    vec8x64u vec8x64u_division_64f_avx512f_impl(vec8x64u n, vec8x64u d) {
        const __m512d hi_scale = _mm512_set1_pd(4294967296.0);
        const __m512i shuffle_indices = _mm512_set_epi32(
            0xf, 0xd, 0xb, 0x9, 0x7, 0x5, 0x3, 0x1,
            0xe, 0xc, 0xa, 0x8, 0x6, 0x4, 0x2, 0x0
        );

        __m512i a = decay(n);
        __m512i b = decay(d);

        __m512d a_f64;
        {
            // Pack lo 32-bit halves of i64 inputs into low half of vector
            // Pack hi 32-bit halves of i64 inputs into high half of vector
            __m512i shuffled = _mm512_permutexvar_epi32(shuffle_indices, decay(n));

            __m256i vec_lo_half = _mm512_extracti64x4_epi64(shuffled, 0x00);
            __m256i vec_hi_half = _mm512_extracti64x4_epi64(shuffled, 0x01);

            __m512d vec_lo_f64 = _mm512_cvtepu32_pd(vec_lo_half);
            __m512d vec_hi_f64 = _mm512_cvtepu32_pd(vec_hi_half);

            a_f64 = _mm512_fmadd_round_pd(hi_scale, vec_hi_f64, vec_lo_f64, _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC);
        }

        __m512d b_f64;
        {
            // Pack lo 32-bit halves of i64 inputs into low half of vector
            // Pack hi 32-bit halves of i64 inputs into high half of vector
            __m512i shuffled = _mm512_permutexvar_epi32(shuffle_indices, decay(n));

            __m256i vec_lo_half = _mm512_extracti64x4_epi64(shuffled, 0x00);
            __m256i vec_hi_half = _mm512_extracti64x4_epi64(shuffled, 0x01);

            __m512d vec_lo_f64 = _mm512_cvtepu32_pd(vec_lo_half);
            __m512d vec_hi_f64 = _mm512_cvtepu32_pd(vec_hi_half);

            b_f64 = _mm512_fmadd_round_pd(hi_scale, vec_hi_f64, vec_lo_f64, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
        }

        __m512d b_rcp = _mm512_div_round_pd(_mm512_set1_pd(1.0), b_f64, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);

        __m512i first; // = _mm512_cvt_roundpd_epu64(_mm512_mul_round_pd(a_f64, b_rcp, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
        {
            __m512d p = _mm512_mul_round_pd(a_f64, b_rcp, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);

            const __m512d recip = _mm512_set1_pd(2.32830643653869628906e-10);
            const __m512d multiplier = _mm512_set1_pd(4294967296.0);

            __m512d t0 = _mm512_roundscale_pd(_mm512_mul_pd(p, recip), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
            __m256i hi_half = _mm512_cvtpd_epu32(t0);
            __m256i lo_half = _mm512_cvtpd_epu32(_mm512_fnmadd_pd(t0, multiplier, p));

            __m512i hi_half_widened = _mm512_cvtepi32_epi64(hi_half);
            __m512i lo_half_widened = _mm512_cvtepi32_epi64(lo_half);

            first = _mm512_or_si512(_mm512_slli_epi64(hi_half_widened, 32), lo_half_widened);
        }

        a = _mm512_sub_epi64(a, _mm512_mullo_epi64(first, b));
        //a_f64 = _mm512_cvt_roundepu64_pd(a, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
        {
            // Pack lo 32-bit halves of i64 inputs into low half of vector
            // Pack hi 32-bit halves of i64 inputs into high half of vector
            __m512i shuffled = _mm512_permutexvar_epi32(shuffle_indices, a);

            __m256i vec_lo_half = _mm512_extracti64x4_epi64(shuffled, 0x00);
            __m256i vec_hi_half = _mm512_extracti64x4_epi64(shuffled, 0x01);

            __m512d vec_lo_f64 = _mm512_cvtepu32_pd(vec_lo_half);
            __m512d vec_hi_f64 = _mm512_cvtepu32_pd(vec_hi_half);

            a_f64 = _mm512_fmadd_round_pd(hi_scale, vec_hi_f64, vec_lo_f64, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
        }

        __m512i second; // = _mm512_cvt_roundpd_epu64(_mm512_mul_round_pd(a_f64, b_rcp, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
        {
            __m512d p = _mm512_mul_round_pd(a_f64, b_rcp, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);

            const __m512d recip = _mm512_set1_pd(2.32830643653869628906e-10);
            const __m512d multiplier = _mm512_set1_pd(4294967296.0);

            __m512d t0 = _mm512_roundscale_pd(_mm512_mul_pd(p, recip), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
            __m256i hi_half = _mm512_cvtpd_epu32(t0);
            __m256i lo_half = _mm512_cvtpd_epu32(_mm512_fnmadd_pd(t0, multiplier, p));

            __m512i hi_half_widened = _mm512_cvtepi32_epi64(hi_half);
            __m512i lo_half_widened = _mm512_cvtepi32_epi64(lo_half);

            second = _mm512_or_si512(_mm512_slli_epi64(hi_half_widened, 32), lo_half_widened);
        }

        __m512i result = _mm512_add_epi64(first, second);

        //a = _mm512_sub_epi64(a, _mm512_mullo_epi64(second, b));
        {
            auto t0 = _mm512_mul_epu32(second, b);
            auto t1 = _mm512_mul_epu32(_mm512_srli_epi64(b, 32), second);
            auto t2 = _mm512_mul_epu32(_mm512_srli_epi64(second, 32), b);

            auto t3 = _mm512_slli_epi64(_mm512_add_epi32(t1, t2), 32);
            a = _mm512_sub_epi64(a, _mm512_add_epi32(t0, t3));
        }

        result = _mm512_mask_add_epi64(result, _mm512_cmp_epu64_mask(a, b, _MM_CMPINT_NLT), result, _mm512_set1_epi64(1));

        return vec8x64u{result};
    }

    auto vec8x64u_division_64f_avx512f = vector_division_test_bench<vec8x64u, vec8x64u_division_64f_avx512f_impl>;

    BENCHMARK(div_64u::vec8x64u_division_64f_avx512f);

    #endif



    #if defined(AVEL_AVX512DQ)

    vec8x64u vec8x64u_division_64f_avx512dq_impl(vec8x64u n, vec8x64u d) {
        __m512i a = decay(n);
        __m512i b = decay(d);

        __m512d a_f64 = _mm512_cvt_roundepu64_pd(decay(n), _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC);
        __m512d b_f64 = _mm512_cvt_roundepu64_pd(decay(d), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
        __m512d b_rcp = _mm512_div_round_pd(_mm512_set1_pd(1.0), b_f64, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);

        __m512i first = _mm512_cvt_roundpd_epu64(_mm512_mul_round_pd(a_f64, b_rcp, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);

        a = _mm512_sub_epi64(a, _mm512_mullo_epi64(first, b));
        a_f64 = _mm512_cvt_roundepu64_pd(a, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);

        __m512i second = _mm512_cvt_roundpd_epu64(_mm512_mul_round_pd(a_f64, b_rcp, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);

        __m512i result = _mm512_add_epi64(first, second);

        a = _mm512_sub_epi64(a, _mm512_mullo_epi64(second, b));

        result = _mm512_mask_add_epi64(result, _mm512_cmp_epu64_mask(a, b, _MM_CMPINT_NLT), result, _mm512_set1_epi64(1));

        return vec8x64u{result};
    }

    auto vec8x64u_division_64f_avx512dq = vector_division_test_bench<vec8x64u, vec8x64u_division_64f_avx512dq_impl>;

    BENCHMARK(div_64u::vec8x64u_division_64f_avx512dq);

    #endif

}

namespace avel::benchmarks::div_53u {

    //=====================================================
    // scalar 53u benchmarks
    //=====================================================

    /*
    std::uint64_t scalar_f64_div_impl(std::uint64_t x, std::uint64_t y) {
        if ((x | y) < 0x20000000000000) [[likely]]  {
            return x / y;
        } else {
            return std::uint64_t(double(x) / double(y));
        }
    }

    auto scalar_f64_div = scalar_division_53_test_bench<std::uint64_t, scalar_f64_div_impl>;

    BENCHMARK(div_53u::scalar_f64_div);
    */

    //=====================================================
    // vec2x64u benchmarks
    //=====================================================

    /*
    #if defined(AVEL_AVX2)

    vec2x64u vec2x64u_53_bit_long_division_impl(vec2x64u n, vec2x64u d) {
        return vec2x64u{};
    }

    auto vec2x64u_53_bit_long_division = vector_division_53_test_bench<vec2x64u, vec2x64u_53_bit_long_division_impl>;

    BENCHMARK(div_53u::vec2x64u_53_bit_long_division);

    #endif



    #if defined(AVEL_AVX512DQ)

    vec2x64u vec2x64u_53_bit_special_handling_avx512dq_impl(vec2x64u n, vec2x64u d) {
        bool is_f64_division_sufficient = _mm_test_all_zeros(_mm_set1_epi64x(0xffe0000000000000ull), _mm_or_si128(decay(n), decay(d)));

        constexpr std::uint8_t mask = 0x3;
        __m128d a_f64 = _mm512_castpd512_pd128(_mm512_maskz_cvt_roundepu64_pd(mask, _mm512_castsi128_si512(decay(n)), _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC));
        __m128d b_f64 = _mm512_castpd512_pd128(_mm512_maskz_cvt_roundepu64_pd(mask, _mm512_castsi128_si512(decay(d)), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC));

        if (is_f64_division_sufficient) [[likely]]  {
            __m128d quotient_f64 = _mm_div_pd(a_f64, b_f64);
            __m128i quotient_u64 = _mm_cvttpd_epu64(quotient_f64);
            return vec2x64u{quotient_u64};
        } else {

            __m128i a = decay(n);
            __m128i b = decay(d);

            __m128d b_rcp = _mm512_castpd512_pd128(_mm512_maskz_div_round_pd(mask, _mm512_castpd128_pd512(_mm_set1_pd(1.0)), _mm512_castpd128_pd512(b_f64), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC));

            __m128i first = _mm512_castsi512_si128(_mm512_maskz_cvt_roundpd_epu64(mask, _mm512_maskz_mul_round_pd(mask, _mm512_castpd128_pd512(a_f64), _mm512_castpd128_pd512(b_rcp), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC));

            a = _mm_sub_epi64(a, _mm_mullo_epi64(first, b));
            a_f64 = _mm512_castpd512_pd128(_mm512_maskz_cvt_roundepu64_pd(mask, _mm512_castsi128_si512(a), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC));

            __m128i second = _mm512_castsi512_si128(_mm512_maskz_cvt_roundpd_epu64(mask, _mm512_maskz_mul_round_pd(mask, _mm512_castpd128_pd512(a_f64), _mm512_castpd128_pd512(b_rcp), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC));

            __m128i result = _mm_add_epi64(first, second);

            a = _mm_sub_epi64(a, _mm_mullo_epi64(second, b));

            result = _mm_mask_add_epi64(result, _mm_cmp_epu64_mask(a, b, _MM_CMPINT_NLT), result, _mm_set1_epi64x(1));

            return vec2x64u{result};
        }
    }

    auto vec2x64u_53_bit_special_handling_avx512dq = vector_division_53_test_bench<vec2x64u, vec2x64u_53_bit_special_handling_avx512dq_impl>;

    BENCHMARK(div_53u::vec2x64u_53_bit_special_handling_avx512dq);

    #endif
    */

    //=====================================================
    // vec4x64u benchmarks
    //=====================================================

    /*
    #if defined(AVEL_AVX2)

    vec4x64u vec4x64u_53_bit_long_division_impl(vec4x64u n, vec4x64u d) {
        return vec4x64u{};
    }

    auto vec4x64u_53_bit_long_division = vector_division_53_test_bench<vec4x64u, vec4x64u_53_bit_long_division_impl>;

    BENCHMARK(div_53u::vec4x64u_53_bit_long_division);

    #endif



    #if defined(AVEL_AVX512VL) & defined(AVEL_AVX512DQ)

    vec4x64u vec4x64u_53_bit_special_handling_avx512dq_impl(vec4x64u n, vec4x64u d) {
        bool is_f64_division_sufficient = _mm256_testz_si256(_mm256_set1_epi64x(0xffe0000000000000ull), _mm256_or_si256(decay(n), decay(d)));

        constexpr std::uint8_t mask = 0xf;
        __m256d a_f64 = _mm512_castpd512_pd256(_mm512_maskz_cvt_roundepu64_pd(mask, _mm512_castsi256_si512(decay(n)), _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC));
        __m256d b_f64 = _mm512_castpd512_pd256(_mm512_maskz_cvt_roundepu64_pd(mask, _mm512_castsi256_si512(decay(d)), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC));

        if (is_f64_division_sufficient) [[likely]] {
            __m256d quotient_f64 = _mm256_div_pd(a_f64, b_f64);
            __m256i quotient_u64 = _mm256_cvttpd_epu64(quotient_f64);
            return vec4x64u{quotient_u64};
        } else {
            __m256i a = decay(n);
            __m256i b = decay(d);
            __m256d b_rcp = _mm512_castpd512_pd256(_mm512_maskz_div_round_pd(mask, _mm512_castpd256_pd512(_mm256_set1_pd(1.0)), _mm512_castpd256_pd512(b_f64), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC));

            __m256i first = _mm512_castsi512_si256(_mm512_maskz_cvt_roundpd_epu64(mask, _mm512_maskz_mul_round_pd(mask, _mm512_castpd256_pd512(a_f64), _mm512_castpd256_pd512(b_rcp), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC));

            a = _mm256_sub_epi64(a, _mm256_mullo_epi64(first, b));
            a_f64 = _mm512_castpd512_pd256(_mm512_maskz_cvt_roundepu64_pd(mask, _mm512_castsi256_si512(a), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC));

            __m256i second = _mm512_castsi512_si256(_mm512_maskz_cvt_roundpd_epu64(mask, _mm512_maskz_mul_round_pd(mask, _mm512_castpd256_pd512(a_f64), _mm512_castpd256_pd512(b_rcp), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC));

            __m256i result = _mm256_add_epi64(first, second);

            a = _mm256_sub_epi64(a, _mm256_mullo_epi64(second, b));

            result = _mm256_mask_add_epi64(result, _mm256_cmp_epu64_mask(a, b, _MM_CMPINT_NLT), result, _mm256_set1_epi64x(1));

            return vec4x64u{result};
        }
    }

    auto vec4x64u_53_bit_special_handling_avx512dq = vector_division_53_test_bench<vec4x64u, vec4x64u_53_bit_special_handling_avx512dq_impl>;

    BENCHMARK(div_53u::vec4x64u_53_bit_special_handling_avx512dq);

    #endif
    */

    //=====================================================
    // vec8x64u benchmarks
    //=====================================================

    /*
    #if defined(AVEL_AVX512F)

    vec8x64u vec8x64u_53_bit_long_division_impl(vec8x64u n, vec8x64u d) {
        return vec8x64u{};
    }

    auto vec8x64u_53_bit_long_division = vector_division_53_test_bench<vec8x64u, vec8x64u_53_bit_long_division_impl>;

    BENCHMARK(div_53u::vec8x64u_53_bit_long_division);

    #endif



    #if defined(AVEL_AVX512DQ)

    vec8x64u vec8x64u_53_bit_special_handling_avx512dq_impl(vec8x64u n, vec8x64u d) {
        auto t0 = _mm512_srli_epi64(_mm512_or_si512(decay(n), decay(d)), 53);
        auto t1 = _mm512_test_epi64_mask(t0, t0);
        bool is_f64_division_sufficient = _mm512_kortestz(t1, t1);

        __m512d a_f64 = _mm512_cvt_roundepu64_pd(decay(n), _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC);
        __m512d b_f64 = _mm512_cvt_roundepu64_pd(decay(d), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);

        if (is_f64_division_sufficient) [[likely]] {
            __m512d quotient_f64 = _mm512_div_pd(a_f64, b_f64);
            __m512i quotient_u64 = _mm512_cvttpd_epu64(quotient_f64);
            return vec8x64u{quotient_u64};
        } else {
            __m512i a = decay(n);
            __m512i b = decay(d);

            __m512d b_rcp = _mm512_div_round_pd(_mm512_set1_pd(1.0), b_f64, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);

            __m512i first = _mm512_cvt_roundpd_epu64(_mm512_mul_round_pd(a_f64, b_rcp, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);

            a = _mm512_sub_epi64(a, _mm512_mullo_epi64(first, b));
            a_f64 = _mm512_cvt_roundepu64_pd(a, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);

            __m512i second = _mm512_cvt_roundpd_epu64(_mm512_mul_round_pd(a_f64, b_rcp, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);

            __m512i result = _mm512_add_epi64(first, second);

            a = _mm512_sub_epi64(a, _mm512_mullo_epi64(second, b));

            result = _mm512_mask_add_epi64(result, _mm512_cmp_epu64_mask(a, b, _MM_CMPINT_NLT), result, _mm512_set1_epi64(1));

            return vec8x64u{result};
        }
    }

    auto vec8x64u_53_bit_special_handling_avx512dq = vector_division_53_test_bench<vec8x64u, vec8x64u_53_bit_special_handling_avx512dq_impl>;

    BENCHMARK(div_53u::vec8x64u_53_bit_special_handling_avx512dq);

    #endif
    */

}

#endif //AVEL_DIVISION_64U_HPP
