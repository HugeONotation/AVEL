#ifndef AVEL_COUNTL_ZERO_32U_HPP
#define AVEL_COUNTL_ZERO_32U_HPP

namespace avel::benchmarks::countl_zero_32u {

    //=====================================================
    // scalar 32u benchmarks
    //=====================================================

    #if __cplusplus >= 202002L

    std::uint32_t scalar_native_impl(std::uint32_t x) {
        return std::countl_zero(x);
    }

    auto scalar_native = scalar_test_bench<std::uint32_t, scalar_native_impl>;

    BENCHMARK(countl_zero_32u::scalar_native);

    #endif



    std::uint32_t scalar_linear_scan_impl(std::uint32_t x) {
        // x86 assembly could be useful here since addition sets the sign bit
        // and a SETSF instruction could be used immediately after

        int cnt = 0;
        while (x & 0x80000000) {
            cnt += 1;
            x += x;
        }

        return cnt;
    }

    auto scalar_linear_scan = scalar_test_bench<std::uint32_t, scalar_linear_scan_impl>;

    BENCHMARK(countl_zero_32u::scalar_linear_scan);



    std::uint32_t scalar_bitwise_divide_and_conquer_impl(std::uint32_t x) {
        x = ~x;
        std::uint32_t sum = (x == 0xFFFFFFFFul);

        bool b0 = (0xFFFF0000u & x) == 0xFFFF0000u;
        sum += (b0 * 16);
        x <<= (b0 * 16);

        bool b1 = (0xFF000000u & x) == 0xFF000000u;
        sum += (b1 * 8);
        x <<= (b1 * 8);

        bool b2 = (0xF0000000u & x) == 0xF0000000u;
        sum += (b2 * 4);
        x <<= (b2 * 4);

        bool b3 = (0xC0000000u & x) == 0xC0000000u;
        sum += (b3 * 2);
        x <<= (b3 * 2);

        bool b4 = (0x80000000u & x) == 0x80000000u;
        sum += (b4 * 1);
        x <<= (b4 * 1);

        return sum;
    }

    auto scalar_bitwise_divide_and_conquer = scalar_test_bench<std::uint32_t, scalar_bitwise_divide_and_conquer_impl>;

    BENCHMARK(countl_zero_32u::scalar_bitwise_divide_and_conquer);



    #if defined(AVEL_LZCNT) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))

    std::uint32_t scalar_lzcnt_op_impl(std::uint32_t x) {
        return _lzcnt_u32(x);
    }

    auto scalar_lzcnt_op = scalar_test_bench<std::uint32_t, scalar_lzcnt_op_impl>;

    BENCHMARK(countl_zero_32u::scalar_lzcnt_op);

    #endif



    #if defined(AVEL_X86) && (defined(AVEL_GCC) || defined(AVEL_CLANG))

    std::uint32_t scalar_bsr_op_impl(std::uint32_t x) {
        if (x) {
            return __builtin_clz(x);
        } else {
            return 32;
        }
    }

    auto scalar_bsr_op = scalar_test_bench<std::uint32_t, scalar_bsr_op_impl>;

    BENCHMARK(countl_zero_32u::scalar_bsr_op);

    #endif



    #if defined(AVEL_MSVC)

    std::uint32_t scalar_bsr_op_impl(std::uint32_t x) {
        unsigned long result;
        if (_BitScanReverse(&result, x)) {
            return 31 - result;
        } else {
            return 32;
        }
    }

    auto scalar_bsr_op = scalar_test_bench<std::uint32_t, scalar_bsr_op_impl>;

    BENCHMARK(countl_zero_32u::scalar_bsr_op);

    #endif

    //=====================================================
    // vec4x32u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec4x32u vec4x32u_32i_to_32f_conv_impl(vec4x32u v) {
        //http://www.icodeguru.com/Embedded/Hacker%27s-Delight/040.htm

        // Block rounding carry
        v = _mm_andnot_si128(_mm_srli_epi32(decay(v), 1), decay(v));

        // Convert to floats
        auto floats = _mm_add_ps(_mm_cvtepi32_ps(decay(v)), _mm_set1_ps(0.5f));

        // Extract exponent
        auto biased_exponents = _mm_srli_epi32(_mm_castps_si128(floats), 23);

        // Compute lzcnt from exponent
        auto lzcnt = _mm_subs_epu16(_mm_set1_epi32(158), biased_exponents);

        // If v's leading bit is set then the exponent
        // extraction will include it. The saturated
        // subtraction ensures the result is 0.

        return vec4x32u{lzcnt};
    }

    auto vec4x32u_32i_to_32f_conv = vector_test_bench<vec4x32u, vec4x32u_32i_to_32f_conv_impl>;

    BENCHMARK(countl_zero_32u::vec4x32u_32i_to_32f_conv);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)

    vec4x32u vec4x32u_32u_to_32f_conv_impl(vec4x32u v) {
        __m512 as_floats = _mm512_maskz_cvt_roundepu32_ps(0x000f, _mm512_castsi128_si512(decay(v)), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
        __m512 as_floats_adjusted = _mm512_maskz_add_round_ps(0x000f, as_floats, _mm512_set1_ps(0.5f), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);

        __m128i float_bits = _mm_castps_si128(_mm512_castps512_ps128(as_floats_adjusted));
        __m128i exponent = _mm_srli_epi32(float_bits, 23);

        __m128i result = _mm_sub_epi32(_mm_set1_epi32(126 + 32), exponent);
        return vec4x32u{result};
    }

    auto vec4x32u_32u_to_32f_conv = vector_test_bench<vec4x32u, vec4x32u_32u_to_32f_conv_impl>;

    BENCHMARK(countl_zero_32u::vec4x32u_32u_to_32f_conv);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)

    vec4x32u vec4x32u_lzcnt_op_impl(vec4x32u v) {
        return vec4x32u{_mm_lzcnt_epi32(decay(v))};
    }

    auto vec4x32u_lzcnt_op = vector_test_bench<vec4x32u, vec4x32u_lzcnt_op_impl>;

    BENCHMARK(countl_zero_32u::vec4x32u_lzcnt_op);

    #endif

    //=====================================================
    // vec8x32u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec8x32u vec8x32u_32i_to_32f_conv_impl(vec8x32u v) {
        //http://www.icodeguru.com/Embedded/Hacker%27s-Delight/040.htm

        // Isolate leading bit
        v = _mm256_andnot_si256(_mm256_srli_epi32(decay(v), 1), decay(v));

        // Convert to floats
        auto floats = _mm256_add_ps(_mm256_cvtepi32_ps(decay(v)), _mm256_set1_ps(0.5f));

        // Extract exponent
        auto biased_exponents = _mm256_srli_epi32(_mm256_castps_si256(floats), 23);

        // Compute lzcnt from exponent
        auto lzcnt = _mm256_subs_epu16(_mm256_set1_epi32(158), biased_exponents);

        // If v's leading bit is set then the exponent
        // extraction will include it. The saturated
        // subtraction ensures the result is 0.

        return vec8x32u{lzcnt};
    }

    auto vec8x32u_32i_to_32f_conv = vector_test_bench<vec8x32u, vec8x32u_32i_to_32f_conv_impl>;

    BENCHMARK(countl_zero_32u::vec8x32u_32i_to_32f_conv);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)

    vec8x32u vec8x32u_32u_to_32f_conv_impl(vec8x32u v) {
        __m512 as_floats = _mm512_maskz_cvt_roundepu32_ps(0x00ff, _mm512_castsi256_si512(decay(v)), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
        __m512 as_floats_adjusted = _mm512_maskz_add_round_ps(0x00ff, as_floats, _mm512_set1_ps(0.5f), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);

        __m256i float_bits = _mm256_castps_si256(_mm512_castps512_ps256(as_floats_adjusted));
        __m256i exponent = _mm256_srli_epi32(float_bits, 23);

        __m256i result = _mm256_sub_epi32(_mm256_set1_epi32(126 + 32), exponent);
        return vec8x32u{result};
    }

    auto vec8x32u_32u_to_32f_conv = vector_test_bench<vec8x32u, vec8x32u_32u_to_32f_conv_impl>;

    BENCHMARK(countl_zero_32u::vec8x32u_32u_to_32f_conv);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512CD)

    vec8x32u vec8x32u_lzcnt_op_impl(vec8x32u v) {
        return vec8x32u{_mm256_lzcnt_epi32(decay(v))};
    }

    auto vec8x32u_lzcnt_op = vector_test_bench<vec8x32u, vec8x32u_lzcnt_op_impl>;

    BENCHMARK(countl_zero_32u::vec8x32u_lzcnt_op);

    #endif

    //=====================================================
    // vec16x32u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512F)

    vec16x32u vec16x32u_32u_to_32f_conv_impl(vec16x32u v) {
        //TODO: Check correctness
        __mmask16 is_non_zero = _mm512_test_epi32_mask(decay(v), decay(v));
        __m512 as_floats = _mm512_cvt_roundepu32_ps(decay(v), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);

        __m512i float_bits = _mm512_castps_si512(as_floats);
        __m512i exponent = _mm512_srli_epi32(float_bits, 23);

        __m512i result = _mm512_sub_epi32(_mm512_set1_epi32(126 + 32), exponent);
        result = _mm512_mask_mov_epi32(_mm512_set1_epi32(32), is_non_zero, result);
        return vec16x32u{result};
    }

    auto vec16x32u_32u_to_32f_conv = vector_test_bench<vec16x32u, vec16x32u_32u_to_32f_conv_impl>;

    BENCHMARK(countl_zero_32u::vec16x32u_32u_to_32f_conv);

    #endif



    #if defined(AVEL_AVX512CD)

    vec16x32u vec16x32u_lzcnt_op_impl(vec16x32u v) {
        return vec16x32u{_mm512_lzcnt_epi32(decay(v))};
    }

    auto vec16x32u_lzcnt_op = vector_test_bench<vec16x32u, vec16x32u_lzcnt_op_impl>;

    BENCHMARK(countl_zero_32u::vec16x32u_lzcnt_op);

    #endif
    
}

#endif //AVEL_COUNTL_ZERO_32U_HPP
