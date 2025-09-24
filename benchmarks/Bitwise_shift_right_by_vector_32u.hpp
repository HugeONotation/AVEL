#ifndef AVEL_BITWISE_SHIFT_RIGHT_BY_VECTOR_32U_HPP
#define AVEL_BITWISE_SHIFT_RIGHT_BY_VECTOR_32U_HPP

namespace avel::benchmarks::bitwise_shift_right_by_vector_32u {

    //=====================================================
    // scalar 32u benchmarks
    //=====================================================

    std::uint32_t scalar_native_impl(std::uint32_t x, std::uint32_t y) {
        return x >> y;
    }

    auto scalar_native = scalar_test_bench<std::uint32_t, scalar_native_impl>;

    BENCHMARK(bitwise_shift_right_by_vector_32u::scalar_native);



    #if defined(AVEL_BMI2)

    std::uint32_t scalar_bmi2_impl(std::uint32_t x, std::uint32_t y) {
        return x >> y; //This version will use the shrx instruction
    }

    auto scalar_bmi2 = scalar_test_bench<std::uint32_t, scalar_bmi2_impl>;

    BENCHMARK(bitwise_shift_right_by_vector_32u::scalar_bmi2);

    #endif

    //=====================================================
    // vec4x32u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec4x32u vec4x32u_multiple_psrld_impl(vec4x32u v, vec4x32u w) {
        auto zeros = _mm_setzero_si128();

        auto x_half0 = _mm_unpacklo_epi32(decay(v), zeros);
        auto x_half1 = _mm_unpackhi_epi32(decay(v), zeros);

        auto y_half0 = _mm_unpacklo_epi32(decay(w), zeros);
        auto y_half1 = _mm_unpackhi_epi32(decay(w), zeros);

        auto X0 = _mm_srl_epi32(x_half0, y_half0);
        auto X1 = _mm_srl_epi32(x_half0, _mm_srli_si128(y_half0, 8));
        auto X2 = _mm_srl_epi32(x_half1, y_half1);
        auto X3 = _mm_srl_epi32(x_half1, _mm_srli_si128(decay(w), 12));

        auto t0 = _mm_unpacklo_epi64(X0, X2);
        auto t1 = _mm_unpackhi_epi64(X1, X3);

        auto t2 = _mm_slli_si128(t1, 0x4);

        auto ret = _mm_or_si128(t0, t2);

        return vec4x32u{ret};
    }

    auto vec4x32u_multiple_psrld = vector_test_bench<vec4x32u, vec4x32u_multiple_psrld_impl>;

    BENCHMARK(bitwise_shift_right_by_vector_32u::vec4x32u_multiple_psrld);

    #endif



    #if defined(AVEL_SSE4_1)

    vec4x32u vec4x32u_bitwise_divide_and_conquer_impl(vec4x32u v, vec4x32u w) {
        alignas(16) constexpr std::uint8_t shuffle_indices_data[16] {
            0, 0, 0, 0,
            1, 1, 1, 1,
            2, 2, 2, 2,
            3, 3, 3, 3
        };

        __m128i ret = decay(v);

        // Move count into each byte since blend is done with byte granularity
        __m128i counts = _mm_shuffle_epi8(decay(w), _mm_load_si128(reinterpret_cast<const __m128i*>(shuffle_indices_data)));

        ret = _mm_blendv_epi8(_mm_srli_epi32(ret, 16), ret, counts);
        counts = _mm_add_epi32(counts, counts);
        ret = _mm_blendv_epi8(_mm_srli_epi32(ret, 8), ret, counts);
        counts = _mm_add_epi32(counts, counts);
        ret = _mm_blendv_epi8(_mm_srli_epi32(ret, 4), ret, counts);
        counts = _mm_add_epi32(counts, counts);
        ret = _mm_blendv_epi8(_mm_srli_epi32(ret, 2), ret, counts);
        counts = _mm_add_epi32(counts, counts);
        ret = _mm_blendv_epi8(_mm_srli_epi32(ret, 1), ret, counts);

        ret = _mm_and_si128(_mm_cmplt_epi32(_mm_set1_epi32(31), decay(w)), ret);

        return vec4x32u{ret};
    }

    auto vec4x32u_bitwise_divide_and_conquer = vector_test_bench<vec4x32u, vec4x32u_bitwise_divide_and_conquer_impl>;

    BENCHMARK(bitwise_shift_right_by_vector_32u::vec4x32u_bitwise_divide_and_conquer);

    #endif



    #if defined(AVEL_SSE4_1)

    vec4x32u vec4x32u_scalarization_impl(vec4x32u v, vec4x32u w) {
        std::uint32_t t0 = _mm_cvtsi128_si32(decay(v));
        std::uint32_t t1 = _mm_cvtsi128_si32(_mm_bsrli_si128(decay(v), 0x4));
        std::uint32_t t2 = _mm_cvtsi128_si32(_mm_bsrli_si128(decay(v), 0x8));
        std::uint32_t t3 = _mm_cvtsi128_si32(_mm_bsrli_si128(decay(v), 0xC));
        
        std::uint32_t s0 = _mm_cvtsi128_si32(decay(w));
        std::uint32_t s1 = _mm_cvtsi128_si32(_mm_bsrli_si128(decay(w), 0x4));
        std::uint32_t s2 = _mm_cvtsi128_si32(_mm_bsrli_si128(decay(w), 0x8));
        std::uint32_t s3 = _mm_cvtsi128_si32(_mm_bsrli_si128(decay(w), 0xC));

        std::uint32_t r0 = t0 >> s0;
        std::uint32_t r1 = t1 >> s1;
        std::uint32_t r2 = t2 >> s2;
        std::uint32_t r3 = t3 >> s3;

        __m128i ret = _mm_setzero_si128();
        ret = _mm_insert_epi32(ret, r0, 0);
        ret = _mm_insert_epi32(ret, r1, 1);
        ret = _mm_insert_epi32(ret, r2, 2);
        ret = _mm_insert_epi32(ret, r3, 3);

        return vec4x32u{ret};
    }

    auto vec4x32u_scalarization = vector_test_bench<vec4x32u, vec4x32u_scalarization_impl>;

    BENCHMARK(bitwise_shift_right_by_vector_32u::vec4x32u_scalarization);

    #endif



    #if defined(AVEL_SSE4_1)

    vec4x32u vec4x32u_scalarization_with_insert_extract_impl(vec4x32u v, vec4x32u w) {
        std::uint32_t t0 = _mm_extract_epi32(decay(v), 0);
        std::uint32_t t1 = _mm_extract_epi32(decay(v), 1);
        std::uint32_t t2 = _mm_extract_epi32(decay(v), 2);
        std::uint32_t t3 = _mm_extract_epi32(decay(v), 3);
        
        std::uint32_t s0 = _mm_extract_epi32(decay(w), 0);
        std::uint32_t s1 = _mm_extract_epi32(decay(w), 1);
        std::uint32_t s2 = _mm_extract_epi32(decay(w), 2);
        std::uint32_t s3 = _mm_extract_epi32(decay(w), 3);

        std::uint32_t r0 = t0 >> s0;
        std::uint32_t r1 = t1 >> s1;
        std::uint32_t r2 = t2 >> s2;
        std::uint32_t r3 = t3 >> s3;

        __m128i ret = _mm_setzero_si128();
        ret = _mm_insert_epi32(ret, r0, 0);
        ret = _mm_insert_epi32(ret, r1, 1);
        ret = _mm_insert_epi32(ret, r2, 2);
        ret = _mm_insert_epi32(ret, r3, 3);

        return vec4x32u{ret};
    }

    auto vec4x32u_scalarization_with_insert_extract = vector_test_bench<vec4x32u, vec4x32u_scalarization_with_insert_extract_impl>;

    BENCHMARK(bitwise_shift_right_by_vector_32u::vec4x32u_scalarization_with_insert_extract);

    #endif



    #if defined(AVEL_AVX2)

    vec4x32u vec4x32u_srlv_impl(vec4x32u v, vec4x32u w) {
        return vec4x32u{_mm_srlv_epi32(decay(v), decay(w))};
    }

    auto vec4x32u_srlv = vector_test_bench<vec4x32u, vec4x32u_srlv_impl>;

    BENCHMARK(bitwise_shift_right_by_vector_32u::vec4x32u_srlv);

    #endif

    //=====================================================
    // vec8x32u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec8x32u vec8x32u_srlv_impl(vec8x32u v, vec8x32u w) {
        return vec8x32u{_mm256_srlv_epi32(decay(v), decay(w))};
    }

    auto vec8x32u_srlv = vector_test_bench<vec8x32u, vec8x32u_srlv_impl>;

    BENCHMARK(bitwise_shift_right_by_vector_32u::vec8x32u_srlv);

    #endif

    //=====================================================
    // vec16x32u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512F)

    vec16x32u vec16x32u_srlv_impl(vec16x32u v, vec16x32u w) {
        return vec16x32u{_mm512_srlv_epi32(decay(v), decay(w))};
    }

    auto vec16x32u_srlv = vector_test_bench<vec16x32u, vec16x32u_srlv_impl>;

    BENCHMARK(bitwise_shift_right_by_vector_32u::vec16x32u_srlv);

    #endif

}

#endif //AVEL_BITWISE_SHIFT_RIGHT_BY_VECTOR_32U_HPP
