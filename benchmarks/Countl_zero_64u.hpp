#ifndef AVEL_COUNTL_ZERO_64U_HPP
#define AVEL_COUNTL_ZERO_64U_HPP

namespace avel::benchmarks::countl_zero_64u {

    //=====================================================
    // scalar 64u benchmarks
    //=====================================================

    #if __cplusplus >= 202002L

    std::uint64_t scalar_native_impl(std::uint64_t x) {
        return std::countl_zero(x);
    }

    auto scalar_native = scalar_test_bench<std::uint64_t, scalar_native_impl>;

    BENCHMARK(countl_zero_64u::scalar_native);

    #endif



    std::uint64_t scalar_linear_scan_impl(std::uint64_t x) {
        // x86 assembly could be useful here since addition sets the sign bit
        // and a SETSF instruction could be used immediately after

        int cnt = 0;
        while (x & 0x8000000000000000) {
            cnt += 1;
            x += x;
        }

        return cnt;
    }

    auto scalar_linear_scan = scalar_test_bench<std::uint64_t, scalar_linear_scan_impl>;

    BENCHMARK(countl_zero_64u::scalar_linear_scan);



    std::uint64_t scalar_bitwise_divide_and_conquer_impl(std::uint64_t x) {
        x = ~x;
        std::uint64_t sum = (x == 0xFFFFFFFFFFFFFFFFul);

        bool b0 = (0xFFFFFFFF00000000u & x) == 0xFFFFFFFF00000000u;
        sum += (b0 * 32);
        x <<= (b0 * 32);

        bool b1 = (0xFFFF000000000000u & x) == 0xFFFF000000000000u;
        sum += (b1 * 16);
        x <<= (b1 * 16);

        bool b2 = (0xFF00000000000000u & x) == 0xFF00000000000000u;
        sum += (b2 * 8);
        x <<= (b2 * 8);

        bool b3 = (0xF000000000000000u & x) == 0xF000000000000000u;
        sum += (b3 * 4);
        x <<= (b3 * 4);

        bool b4 = (0xC000000000000000u & x) == 0xC000000000000000u;
        sum += (b4 * 2);
        x <<= (b4 * 2);

        bool b5 = (0x8000000000000000u & x) == 0x8000000000000000u;
        sum += (b5 * 1);

        return sum;
    }

    auto scalar_bitwise_divide_and_conquer = scalar_test_bench<std::uint64_t, scalar_bitwise_divide_and_conquer_impl>;

    BENCHMARK(countl_zero_64u::scalar_bitwise_divide_and_conquer);



    #if defined(AVEL_LZCNT) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))

    std::uint64_t scalar_lzcnt_op_impl(std::uint64_t x) {
        return _lzcnt_u64(x);
    }

    auto scalar_lzcnt_op = scalar_test_bench<std::uint64_t, scalar_lzcnt_op_impl>;

    BENCHMARK(countl_zero_64u::scalar_lzcnt_op);

    #endif



    #if defined(AVEL_X86) && (defined(AVEL_GCC) || defined(AVEL_CLANG))

    std::uint64_t scalar_bsr_op_impl(std::uint64_t x) {
        if (x) {
            return __builtin_clzl(x);
        } else {
            return 64;
        }
    }

    auto scalar_bsr_op = scalar_test_bench<std::uint64_t, scalar_bsr_op_impl>;

    BENCHMARK(countl_zero_64u::scalar_bsr_op);

    #endif



    #if defined(AVEL_MSVC)

    std::uint64_t scalar_bsr_op_impl(std::uint64_t x) {
        unsigned long result;
        if (_BitScanReverse64(&result, x)) {
            return 63 - result;
        } else {
            return 64;
        }
    }

    auto scalar_bsr_op = scalar_test_bench<std::uint64_t, scalar_bsr_op_impl>;

    BENCHMARK(countl_zero_64u::scalar_bsr_op);

    #endif

    //=====================================================
    // vec2x64u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))

    vec2x64u vec2x64u_scalarized_bsr_impl(vec2x64u v) {
        return vec2x64u{_mm_set_epi64x(
            extract<0>(v) ? __builtin_clzl(extract<0>(v)) : 64,
            extract<1>(v) ? __builtin_clzl(extract<1>(v)) : 64
        )};
    }

    auto vec2x64u_scalarized_bsr = vector_test_bench<vec2x64u, vec2x64u_scalarized_bsr_impl>;

    BENCHMARK(countl_zero_64u::vec2x64u_scalarized_bsr);

    #endif



    #if defined(AVEL_SSE2) && defined(AVEL_LZCNT) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))

    vec2x64u vec2x64u_scalarized_lzcnt_impl(vec2x64u v) {
        return vec2x64u{_mm_set_epi64x(
            _lzcnt_u64(extract<0>(v)),
            _lzcnt_u64(extract<1>(v))
        )};
    }

    auto vec2x64u_scalarized_lzcnt = vector_test_bench<vec2x64u, vec2x64u_scalarized_lzcnt_impl>;

    BENCHMARK(countl_zero_64u::vec2x64u_scalarized_lzcnt);

    #endif



    #if defined(AVEL_SSE2)

    vec2x64u vec2x64u_32i_to_32f_conv_no_cmp64_impl(vec2x64u v) {
        // Compute leading zero counts for 32-bit ints
        //http://www.icodeguru.com/Embedded/Hacker%27s-Delight/040.htm

        // Isolate leading bit
        v = _mm_andnot_si128(_mm_srli_epi32(decay(v), 1), decay(v));

        // Convert to floats
        auto floats = _mm_add_ps(_mm_cvtepi32_ps(decay(v)), _mm_set1_ps(0.5f));

        // Extract exponent
        auto biased_exponents = _mm_srli_epi32(_mm_castps_si128(floats), 23);

        // Compute lzcnt from exponent
        auto partial_results = _mm_subs_epu16(_mm_set1_epi32(158), biased_exponents);

        // If v's leading bit is set then the exponent
        // extraction will include it. The saturated
        // subtraction ensures the result is 0.

        // Combine results to compute 64-bit result
        __m128i even_results = _mm_and_si128(_mm_set1_epi64x(0x00000000ffffffff), partial_results);
        __m128i  odd_results = _mm_srli_epi64(partial_results, 32);

        __m128i threshold = _mm_set1_epi64x(std::uint64_t(32ull) << 32);
        __m128i mask = _mm_bsrli_si128(_mm_srai_epi32(_mm_sub_epi64(partial_results, threshold), 32), 4);
        __m128i results = _mm_add_epi64(odd_results, _mm_and_si128(mask, even_results));
        return vec2x64u{results};
    }

    auto vec2x64u_32i_to_32f_conv_no_cmp64 = vector_test_bench<vec2x64u, vec2x64u_32i_to_32f_conv_no_cmp64_impl>;

    BENCHMARK(countl_zero_64u::vec2x64u_32i_to_32f_conv_no_cmp64);

    #endif



    #if defined(AVEL_SSE4_2)

    vec2x64u vec2x64u_32i_to_32f_conv_impl(vec2x64u v) {
        // Compute leading zero counts for 32-bit ints
        //http://www.icodeguru.com/Embedded/Hacker%27s-Delight/040.htm

        // Isolate leading bit
        v = _mm_andnot_si128(_mm_srli_epi32(decay(v), 1), decay(v));

        // Convert to floats
        auto floats = _mm_add_ps(_mm_cvtepi32_ps(decay(v)), _mm_set1_ps(0.5f));

        // Extract exponent
        auto biased_exponents = _mm_srli_epi32(_mm_castps_si128(floats), 23);

        // Compute lzcnt from exponent
        auto partial_results = _mm_subs_epu16(_mm_set1_epi32(158), biased_exponents);

        // If v's leading bit is set then the exponent
        // extraction will include it. The saturated
        // subtraction ensures the result is 0.

        // Combine results to compute 64-bit result
        __m128i even_results = _mm_and_si128(_mm_set1_epi64x(0x00000000ffffffff), partial_results);
        __m128i  odd_results = _mm_srli_epi64(partial_results, 32);

        __m128i threshold = _mm_set1_epi64x(std::uint64_t(32ull) << 32);
        __m128i mask = _mm_cmpgt_epi64(threshold, partial_results);
        __m128i results = _mm_add_epi64(odd_results, _mm_and_si128(mask, even_results));
        return vec2x64u{results};
    }

    auto vec2x64u_32i_to_32f_conv = vector_test_bench<vec2x64u, vec2x64u_32i_to_32f_conv_impl>;

    BENCHMARK(countl_zero_64u::vec2x64u_32i_to_32f_conv);

    #endif



    #if defined(AVEL_AVX512VL)

    vec2x64u vec2x64u_conversion_32f_impl(vec2x64u v) {
        //TODo: Test correctness
        __m128 as_floats = _mm_cvtepu32_ps(decay(v));
        __m128 as_floats_adjusted = _mm_add_ps(as_floats, _mm_set1_ps(0.5));

        __m128i exponent_mask = _mm_set1_epi32(0x7f800000);
        __m128i float_bits = _mm_castps_si128(as_floats_adjusted);
        __m128i exponent = _mm_srli_epi32(_mm_and_si128(exponent_mask, float_bits), 23);

        __m128i partial_results = _mm_sub_epi32(_mm_set1_epi32(158), exponent);

        __m128i even_value = _mm_maskz_mov_epi32(0x5, partial_results);
        __m128i odd_value = _mm_srli_epi64(partial_results, 32);

        __mmask16 addition_mask = _mm_cmpeq_epi32_mask(odd_value, _mm_set1_epi64x(0x20));
        __m128i result = _mm_mask_add_epi32(odd_value, addition_mask, even_value, odd_value);

        return vec2x64u{result};
    }

    auto vec2x64u_conversion_32f = vector_test_bench<vec2x64u, vec2x64u_conversion_32f_impl>;

    BENCHMARK(countl_zero_64u::vec2x64u_conversion_32f);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)

    vec2x64u vec2x64u_64u_to_64f_conv_impl(vec2x64u v) {
        // Compute leading zero counts within 32-bit integers
        __m512 as_floats = _mm512_maskz_cvt_roundepu32_ps(0x000f, _mm512_castsi128_si512(decay(v)), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
        __m512 as_floats_adjusted = _mm512_maskz_add_round_ps(0x000f, as_floats, _mm512_set1_ps(0.5f), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);

        __m128i float_bits = _mm_castps_si128(_mm512_castps512_ps128(as_floats_adjusted));
        __m128i exponent = _mm_srli_epi32(float_bits, 23);

        __m128i partial_results = _mm_sub_epi32(_mm_set1_epi32(126 + 32), exponent);

        // Combine 32-bit results into 64-bit results
        __m128i even_results = _mm_maskz_mov_epi32(0x55, partial_results);
        __m128i  odd_results = _mm_srli_epi64(partial_results, 32);

        __m128i threshold = _mm_set1_epi64x(std::uint64_t(32ull) << 32);
        __mmask8 mask = _mm_cmpge_epu64_mask(partial_results, threshold);
        __m128i results = _mm_mask_add_epi64(odd_results, mask, odd_results, even_results);
        return vec2x64u{results};
    }

    auto vec2x64u_64u_to_64f_conv = vector_test_bench<vec2x64u, vec2x64u_64u_to_64f_conv_impl>;

    BENCHMARK(countl_zero_64u::vec2x64u_64u_to_64f_conv);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)

    vec2x64u vec2x64u_lzcnt_op_impl(vec2x64u v) {
        return vec2x64u{_mm_lzcnt_epi64(decay(v))};
    }

    auto vec2x64u_lzcnt_op = vector_test_bench<vec2x64u, vec2x64u_lzcnt_op_impl>;

    BENCHMARK(countl_zero_64u::vec2x64u_lzcnt_op);

    #endif

    //=====================================================
    // vec4x64u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec4x64u vec4x64u_32i_to_32f_conv_impl(vec4x64u v) {
        // Compute leading zero counts for 32-bit ints
        //http://www.icodeguru.com/Embedded/Hacker%27s-Delight/040.htm

        // Isolate leading bit
        v = _mm256_andnot_si256(_mm256_srli_epi32(decay(v), 1), decay(v));

        // Convert to floats
        auto floats = _mm256_add_ps(_mm256_cvtepi32_ps(decay(v)), _mm256_set1_ps(0.5f));

        // Extract exponent
        auto biased_exponents = _mm256_srli_epi32(_mm256_castps_si256(floats), 23);

        // Compute lzcnt from exponent
        auto partial_results = _mm256_subs_epu16(_mm256_set1_epi32(158), biased_exponents);

        // If v's leading bit is set then the exponent
        // extraction will include it. The saturated
        // subtraction ensures the result is 0.

        // Combine results to compute 64-bit result
        __m256i even_results = _mm256_blend_epi32(_mm256_setzero_si256(), partial_results, 0x55);
        __m256i  odd_results = _mm256_srli_epi64(partial_results, 32);

        __m256i threshold = _mm256_set1_epi64x(std::uint64_t(32ull) << 32);
        __m256i mask = _mm256_cmpgt_epi64(threshold, partial_results);
        __m256i results = _mm256_add_epi64(odd_results, _mm256_and_si256(mask, even_results));
        return vec4x64u{results};
    }

    auto vec4x64u_32i_to_32f_conv = vector_test_bench<vec4x64u, vec4x64u_32i_to_32f_conv_impl>;

    BENCHMARK(countl_zero_64u::vec4x64u_32i_to_32f_conv);

    #endif


    #if defined(AVEL_AVX512VL)

    vec4x64u vec4x64u_32u_to_32f_conv_impl(vec4x64u v) {
        // Compute leading zero counts within 32-bit integers
        __m512 as_floats = _mm512_maskz_cvt_roundepu32_ps(0x00ff, _mm512_castsi256_si512(decay(v)), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
        __m512 as_floats_adjusted = _mm512_maskz_add_round_ps(0x00ff, as_floats, _mm512_set1_ps(0.5f), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);

        __m256i float_bits = _mm256_castps_si256(_mm512_castps512_ps256(as_floats_adjusted));
        __m256i exponent = _mm256_srli_epi32(float_bits, 23);

        __m256i partial_results = _mm256_sub_epi32(_mm256_set1_epi32(126 + 32), exponent);

        // Combine 32-bit results into 64-bit results
        __m256i even_results = _mm256_maskz_mov_epi32(0x55, partial_results);
        __m256i  odd_results = _mm256_srli_epi64(partial_results, 32);

        __m256i threshold = _mm256_set1_epi64x(std::uint64_t(32ull) << 32);
        __mmask8 mask = _mm256_cmpge_epu64_mask(partial_results, threshold);
        __m256i results = _mm256_mask_add_epi64(odd_results, mask, odd_results, even_results);
        return vec4x64u{results};
    }

    auto vec4x64u_32u_to_32f_conv = vector_test_bench<vec4x64u, vec4x64u_32u_to_32f_conv_impl>;

    BENCHMARK(countl_zero_64u::vec4x64u_32u_to_32f_conv);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)

    vec4x64u vec4x64u_64u_to_64f_conv_impl(vec4x64u v) {
        __m512d as_floats = _mm512_cvt_roundepu64_pd(_mm512_castsi256_si512(decay(v)), _MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC);
        __m512d as_floats_adjusted = _mm512_add_round_pd(as_floats, _mm512_set1_pd(0.5), _MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC);

        __m256i float_bits = _mm256_castpd_si256(_mm512_castpd512_pd256(as_floats_adjusted));
        __m256i exponent = _mm256_srli_epi64(float_bits, 52);

        __m256i result = _mm256_sub_epi64(_mm256_set1_epi64x(1022 + 64), exponent);
        return vec4x64u{result};
    }

    auto vec4x64u_64u_to_64f_conv = vector_test_bench<vec4x64u, vec4x64u_64u_to_64f_conv_impl>;

    BENCHMARK(countl_zero_64u::vec4x64u_64u_to_64f_conv);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)

    vec4x64u vec4x64u_lzcnt_op_impl(vec4x64u v) {
        return vec4x64u{_mm256_lzcnt_epi64(decay(v))};
    }

    auto vec4x64u_lzcnt_op = vector_test_bench<vec4x64u, vec4x64u_lzcnt_op_impl>;

    BENCHMARK(countl_zero_64u::vec4x64u_lzcnt_op);

    #endif

    //=====================================================
    // vec8x64u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512F)

    vec8x64u vec8x64u_32u_to_32f_conv_impl(vec8x64u v) {
        // Compute leading zero counts within 32-bit integers
        __m512 as_floats = _mm512_cvt_roundepu32_ps(decay(v), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
        __m512 as_floats_adjusted = _mm512_add_round_ps(as_floats, _mm512_set1_ps(0.5f), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);

        __m512i float_bits = _mm512_castps_si512(as_floats_adjusted);
        __m512i exponent = _mm512_srli_epi32(float_bits, 23);

        __m512i partial_results = _mm512_sub_epi32(_mm512_set1_epi32(126 + 32), exponent);

        // Combine 32-bit results into 64-bit results
        __m512i even_results = _mm512_maskz_mov_epi32(0x5555, partial_results);
        __m512i  odd_results = _mm512_srli_epi64(partial_results, 32);

        __m512i threshold = _mm512_set1_epi64(std::uint64_t(32ull) << 32);
        __mmask16 mask = _mm512_cmpge_epu64_mask(partial_results, threshold);
        __m512i results = _mm512_mask_add_epi64(odd_results, mask, odd_results, even_results);
        return vec8x64u{results};
    }

    auto vec8x64u_32u_to_32f_conv = vector_test_bench<vec8x64u, vec8x64u_32u_to_32f_conv_impl>;

    BENCHMARK(countl_zero_64u::vec8x64u_32u_to_32f_conv);

    #endif



    #if defined(AVEL_AVX512DQ)

    vec8x64u vec8x64u_64u_to_64f_conv_impl(vec8x64u v) {
        __m512d as_floats = _mm512_cvt_roundepu64_pd(decay(v), _MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC);
        __m512d as_floats_adjusted = _mm512_add_round_pd(as_floats, _mm512_set1_pd(0.5), _MM_FROUND_TO_ZERO |_MM_FROUND_NO_EXC);

        __m512i float_bits = _mm512_castpd_si512(as_floats_adjusted);
        __m512i exponent = _mm512_srli_epi64(float_bits, 52);

        __m512i result = _mm512_sub_epi64(_mm512_set1_epi64(1022 + 64), exponent);
        return vec8x64u{result};
    }

    auto vec8x64u_64u_to_64f_conv = vector_test_bench<vec8x64u, vec8x64u_64u_to_64f_conv_impl>;

    BENCHMARK(countl_zero_64u::vec8x64u_64u_to_64f_conv);

    #endif



    #if defined(AVEL_AVX512CD)

    vec8x64u vec8x64u_lzcnt_op_impl(vec8x64u v) {
        return vec8x64u{_mm512_lzcnt_epi64(decay(v))};
    }

    auto vec8x64u_lzcnt_op = vector_test_bench<vec8x64u, vec8x64u_lzcnt_op_impl>;

    BENCHMARK(countl_zero_64u::vec8x64u_lzcnt_op);

    #endif

}

#endif //AVEL_COUNTL_ZERO_64U_HPP
