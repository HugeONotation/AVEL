#ifndef MODF_32F_HPP
#define MODF_32F_HPP

namespace avel::benchmarks::modf_32f {

    //=====================================================
    // Scalar 32f benchmarks
    //=====================================================

    float scalar_native_impl(float num, float* iptr) {
        return std::modf(num, iptr);
    }

    auto scalar_native = scalar_test_bench<float, float, scalar_native_impl>;

    BENCHMARK(modf_32f::scalar_native);



    float scalar_truncation_impl(float num, float* iptr) {
        float whole = avel::trunc(num);
        float frac = avel::copysign(clear(avel::isinf(num), num - whole), num);
        *iptr = whole;
        return frac;
    }

    auto scalar_truncation = scalar_test_bench<float, float, scalar_truncation_impl>;

    BENCHMARK(modf_32f::scalar_truncation);



    #if defined(AVEL_SSE4_1)

    float scalar_sse4_1_impl(float num, float* iptr) {
        __m128 num_reg = _mm_set_ss(num);
        __m128 whole_reg = _mm_round_ss(num_reg, num_reg, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);

        __m128 sign_bit_mask = _mm_castsi128_ps(_mm_set1_epi32(0x80000000));
        __m128 abs_num = _mm_andnot_ps(sign_bit_mask, num_reg);

        __m128 is_inf = _mm_cmpeq_ss(abs_num, _mm_set_ss(INFINITY));
        __m128 diff = _mm_sub_ss(num_reg, whole_reg);
        __m128 masked_diff = _mm_andnot_ps(is_inf, diff);

        __m128 sign_bit = _mm_and_ps(sign_bit_mask, num_reg);
        __m128 magnitude = _mm_andnot_ps(sign_bit_mask, masked_diff);
        __m128 frac = _mm_or_ps(sign_bit, magnitude);

        *iptr = _mm_cvtss_f32(whole_reg);

        return _mm_cvtss_f32(frac);
    }

    auto scalar_sse4_1 = scalar_test_bench<float, float, scalar_sse4_1_impl>;

    BENCHMARK(modf_32f::scalar_sse4_1);

    #endif



    #if defined(AVEL_AVX512F)

    float scalar_avx512f_impl(float num, float* iptr) {
        __m128 num_reg = _mm_set_ss(num);
        __m128 whole_reg = _mm_round_ss(num_reg, num_reg, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);

        __m128 sign_bit_mask = _mm_castsi128_ps(_mm_set1_epi32(0x80000000));
        __m128 abs_num = _mm_andnot_ps(sign_bit_mask, num_reg);

        __m128 is_inf = _mm_cmpeq_ss(abs_num, _mm_set_ss(INFINITY));
        __m128 diff = _mm_sub_ss(num_reg, whole_reg);
        __m128 masked_diff = _mm_andnot_ps(is_inf, diff);

        // Gets optimized away to use ternlog
        __m128 sign_bit = _mm_and_ps(sign_bit_mask, num_reg);
        __m128 magnitude = _mm_andnot_ps(sign_bit_mask, masked_diff);
        __m128 frac = _mm_or_ps(sign_bit, magnitude);

        *iptr = _mm_cvtss_f32(whole_reg);

        return _mm_cvtss_f32(frac);
    }

    auto scalar_avx512f = scalar_test_bench<float, float, scalar_avx512f_impl>;

    BENCHMARK(modf_32f::scalar_avx512f);

    #endif



    #if defined(AVEL_AVX512DQ)

    float scalar_roundscale_reduce_impl(float num, float* iptr) {
        __m128 num_reg = _mm_set_ss(num);

        __m128 x = _mm_roundscale_ss(num_reg, num_reg, _MM_FROUND_TO_ZERO);
        __m128 y = _mm_reduce_ss(num_reg, num_reg, _MM_FROUND_TO_ZERO | (0 << 4));

        *iptr = _mm_cvtss_f32(x);
        return avel::copysign(_mm_cvtss_f32(y), num);
    }

    auto scalar_roundscale_reduce = scalar_test_bench<float, float, scalar_roundscale_reduce_impl>;

    BENCHMARK(modf_32f::scalar_roundscale_reduce);

    #endif

    //=====================================================
    // vec4x32f benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec4x32f vec4x32f_truncate_impl(vec4x32f num, vec4x32f* iptr) {
        __m128 num_reg = decay(num);
        __m128 whole_reg = decay(avel::trunc(num));

        __m128 sign_bit_mask = _mm_castsi128_ps(_mm_set1_epi32(0x80000000));
        __m128 abs_num = _mm_andnot_ps(sign_bit_mask, num_reg);

        __m128 is_inf = _mm_cmpeq_ps(abs_num, _mm_set1_ps(INFINITY));
        __m128 masked_diff = _mm_andnot_ps(is_inf, _mm_sub_ps(num_reg, whole_reg));

        // Gets optimized away to use ternlog
        __m128 sign_bit = _mm_and_ps(sign_bit_mask, num_reg);
        __m128 magnitude = _mm_andnot_ps(sign_bit_mask, masked_diff);
        __m128 frac = _mm_or_ps(sign_bit, magnitude);

        *iptr = vec4x32f(whole_reg);
        return vec4x32f(frac);
    }

    auto vec4x32f_truncate = vector_test_bench<vec4x32f, vec4x32f, vec4x32f_truncate_impl>;

    BENCHMARK(modf_32f::vec4x32f_truncate);

    #endif



    #if defined(AVEL_SSE4_1)

    vec4x32f vec4x32f_truncate2_impl(vec4x32f num, vec4x32f* iptr) {
        __m128 num_reg = decay(num);
        __m128 whole_reg = _mm_round_ps(num_reg, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);

        __m128 sign_bit_mask = _mm_castsi128_ps(_mm_set1_epi32(0x80000000));
        __m128 abs_num = _mm_andnot_ps(sign_bit_mask, num_reg);

        __m128 is_inf = _mm_cmpeq_ps(abs_num, _mm_set1_ps(INFINITY));
        __m128 masked_diff = _mm_andnot_ps(is_inf, _mm_sub_ps(num_reg, whole_reg));

        // Gets optimized away to use ternlog
        __m128 sign_bit = _mm_and_ps(sign_bit_mask, num_reg);
        __m128 magnitude = _mm_andnot_ps(sign_bit_mask, masked_diff);
        __m128 frac = _mm_or_ps(sign_bit, magnitude);

        *iptr = vec4x32f(whole_reg);
        return vec4x32f(frac);
    }

    auto vec4x32f_truncate2 = vector_test_bench<vec4x32f, vec4x32f, vec4x32f_truncate2_impl>;

    BENCHMARK(modf_32f::vec4x32f_truncate2);

    #endif



    #if defined(AVEL_AVX512VL)

    vec4x32f vec4x32f_truncate_k_impl(vec4x32f num, vec4x32f* iptr) {
        __m128 num_reg = decay(num);
        __m128 whole_reg = _mm_round_ps(num_reg, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);

        __m128 sign_bit_mask = _mm_castsi128_ps(_mm_set1_epi32(0x80000000));
        __m128 abs_num = _mm_andnot_ps(sign_bit_mask, num_reg);

        __mmask8 is_inf = _mm_cmp_ps_mask(abs_num, _mm_set1_ps(INFINITY), _CMP_EQ_OQ);
        __m128 masked_diff = _mm_maskz_sub_ps(is_inf, num_reg, whole_reg);

        // Gets optimized away to use ternlog
        __m128 sign_bit = _mm_and_ps(sign_bit_mask, num_reg);
        __m128 magnitude = _mm_andnot_ps(sign_bit_mask, masked_diff);
        __m128 frac = _mm_or_ps(sign_bit, magnitude);

        *iptr = vec4x32f(whole_reg);
        return vec4x32f(frac);
    }

    auto vec4x32f_truncate_k = vector_test_bench<vec4x32f, vec4x32f, vec4x32f_truncate_k_impl>;

    BENCHMARK(modf_32f::vec4x32f_truncate_k);

    #endif



    #if defined(AVEL_AVX512DQ) && defined(AVEL_AVX512VL)

    vec4x32f vec4x32f_roundscale_reduce_impl(vec4x32f num, vec4x32f* iptr) {
        __m128 num_reg = decay(num);

        __m128 x = _mm_roundscale_ps(num_reg, _MM_FROUND_TO_ZERO);
        __m128 y = _mm_reduce_ps(num_reg, _MM_FROUND_TO_ZERO | (0 << 4));

        *iptr = vec4x32f(x);
        return avel::copysign(vec4x32f{y}, num);
    }

    auto vec4x32f_roundscale_reduce = vector_test_bench<vec4x32f, vec4x32f, vec4x32f_roundscale_reduce_impl>;

    BENCHMARK(modf_32f::vec4x32f_roundscale_reduce);

    #endif

    //=====================================================
    // vec8x32f benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec8x32f vec8x32f_truncate_impl(vec8x32f num, vec8x32f* iptr) {
        __m256 num_reg = decay(num);
        __m256 whole_reg = _mm256_round_ps(num_reg, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);

        __m256 sign_bit_mask = _mm256_castsi256_ps(_mm256_set1_epi32(0x80000000));
        __m256 abs_num = _mm256_andnot_ps(sign_bit_mask, num_reg);

        __m256 is_inf = _mm256_cmp_ps(abs_num, _mm256_set1_ps(INFINITY), _CMP_EQ_OQ);
        __m256 masked_diff = _mm256_andnot_ps(is_inf, _mm256_sub_ps(num_reg, whole_reg));

        // Gets optimized away to use ternlog
        __m256 sign_bit = _mm256_and_ps(sign_bit_mask, num_reg);
        __m256 magnitude = _mm256_andnot_ps(sign_bit_mask, masked_diff);
        __m256 frac = _mm256_or_ps(sign_bit, magnitude);

        *iptr = vec8x32f(whole_reg);
        return vec8x32f(frac);
    }

    #endif



    #if defined(AVEL_AVX512VL)

    vec8x32f vec8x32f_truncate_k_impl(vec8x32f num, vec8x32f* iptr) {
        __m256 num_reg = decay(num);
        __m256 whole_reg = _mm256_round_ps(num_reg, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);

        __m256 sign_bit_mask = _mm256_castsi256_ps(_mm256_set1_epi32(0x80000000));
        __m256 abs_num = _mm256_andnot_ps(sign_bit_mask, num_reg);

        __mmask8 is_inf = _mm256_cmp_ps_mask(abs_num, _mm256_set1_ps(INFINITY), _CMP_EQ_OQ);
        __m256 masked_diff = _mm256_maskz_sub_ps(is_inf, num_reg, whole_reg);

        // Gets optimized away to use ternlog
        __m256 sign_bit = _mm256_and_ps(sign_bit_mask, num_reg);
        __m256 magnitude = _mm256_andnot_ps(sign_bit_mask, masked_diff);
        __m256 frac = _mm256_or_ps(sign_bit, magnitude);

        *iptr = vec8x32f(whole_reg);
        return vec8x32f(frac);
    }

    auto vec8x32f_truncate_k = vector_test_bench<vec8x32f, vec8x32f, vec8x32f_truncate_k_impl>;

    BENCHMARK(modf_32f::vec8x32f_truncate_k);

    #endif



    #if defined(AVEL_AVX512DQ) && defined(AVEL_AVX512VL)

    vec8x32f vec8x32f_roundscale_reduce_impl(vec8x32f num, vec8x32f* iptr) {
        __m256 num_reg = decay(num);

        __m256 x = _mm256_roundscale_ps(num_reg, _MM_FROUND_TO_ZERO);
        __m256 y = _mm256_reduce_ps(num_reg, _MM_FROUND_TO_ZERO | (0 << 4));

        *iptr = vec8x32f(x);
        return avel::copysign(vec8x32f{y}, num);
    }

    auto vec8x32f_roundscale_reduce = vector_test_bench<vec8x32f, vec8x32f, vec8x32f_roundscale_reduce_impl>;

    BENCHMARK(modf_32f::vec8x32f_roundscale_reduce);

    #endif

    //=====================================================
    // vec16x32f benchmarks
    //=====================================================

    #if defined(AVEL_AVX512F)

    vec16x32f vec16x32f_truncate_k_impl(vec16x32f num, vec16x32f* iptr) {
        __m512 num_reg = decay(num);
        __m512 whole_reg = _mm512_roundscale_ps(num_reg, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);

        __m512 sign_bit_mask = _mm512_castsi512_ps(_mm512_set1_epi32(0x80000000));
        __m512 abs_num = _mm512_castsi512_ps(_mm512_andnot_si512(_mm512_castps_si512(sign_bit_mask), _mm512_castps_si512(num_reg)));

        __mmask16 is_inf = _mm512_cmp_ps_mask(abs_num, _mm512_set1_ps(INFINITY), _CMP_EQ_OQ);
        __m512 masked_diff = _mm512_maskz_sub_ps(is_inf, num_reg, whole_reg);

        // Gets optimized away to use ternlog
        __m512 sign_bit = _mm512_castsi512_ps(_mm512_and_si512(_mm512_castps_si512(sign_bit_mask), _mm512_castps_si512(num_reg)));
        __m512 magnitude = _mm512_castsi512_ps(_mm512_andnot_si512(_mm512_castps_si512(sign_bit_mask), _mm512_castps_si512(masked_diff)));
        __m512 frac = _mm512_castsi512_ps(_mm512_or_si512(_mm512_castps_si512(sign_bit), _mm512_castps_si512(magnitude)));

        *iptr = vec16x32f(whole_reg);

        return vec16x32f(frac);
    }

    auto vec16x32f_truncate_k = vector_test_bench<vec16x32f, vec16x32f, vec16x32f_truncate_k_impl>;

    BENCHMARK(modf_32f::vec16x32f_truncate_k);

    #endif



    #if defined(AVEL_AVX512DQ)

    vec16x32f vec16x32f_roundscale_reduce_impl(vec16x32f num, vec16x32f* iptr) {
        __m512 num_reg = decay(num);

        __m512 x = _mm512_roundscale_ps(num_reg, _MM_FROUND_TO_ZERO);
        __m512 y = _mm512_reduce_ps(num_reg, _MM_FROUND_TO_ZERO | (0 << 4));

        *iptr = vec16x32f(x);
        return avel::copysign(vec16x32f{y}, num);
    }

    auto vec16x32f_roundscale_reduce = vector_test_bench<vec16x32f, vec16x32f, vec16x32f_roundscale_reduce_impl>;

    BENCHMARK(modf_32f::vec16x32f_roundscale_reduce);

    #endif


}

#endif //MODF_32F_HPP
