#ifndef AVEL_SHIFT_LEFT_32U_HPP
#define AVEL_SHIFT_LEFT_32U_HPP

namespace avel::benchmarks::shift_left_32u {

    //=====================================================
    // scalar 32u benchmarks
    //=====================================================

    std::uint32_t scalar_native_impl(std::uint32_t x, std::uint32_t y) {
        return x << y;
    }

    auto scalar_native = scalar_test_bench<std::uint32_t, scalar_native_impl>;

    BENCHMARK(shift_left_32u::scalar_native);

    //=====================================================
    // vec4x32u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec4x32u vec4x32u_mul_pow2_impl(vec4x32u v, vec4x32u w) {
        __m128i exp = _mm_slli_epi32(_mm_add_epi32(decay(w), _mm_set1_epi32(127)), 23);
        __m128i pow2 = _mm_cvtps_epi32(_mm_castsi128_ps(exp));

        auto lhs_lo = decay(v);
        auto lhs_hi = _mm_srli_epi64(decay(v), 32);

        auto rhs_lo = pow2;
        auto rhs_hi = _mm_srli_epi64(pow2, 32);

        auto lo = _mm_mul_epu32(lhs_lo, rhs_lo);
        auto hi = _mm_mul_epu32(lhs_hi, rhs_hi);

        auto half0 = _mm_unpacklo_epi32(lo, hi);
        auto half1 = _mm_unpackhi_epi32(lo, hi);

        return vec4x32u{_mm_unpacklo_epi64(half0, half1)};
    }

    auto vec4x32u_mul_pow2 = vector_test_bench<vec4x32u, vec4x32u_mul_pow2_impl>;

    BENCHMARK(shift_left_32u::vec4x32u_mul_pow2);

    #endif



    #if defined(AVEL_SSE4_1)

    vec4x32u vec4x32u_mullo_pow2_impl(vec4x32u v, vec4x32u w) {
        __m128i exp = _mm_add_epi32(decay(w), _mm_set1_epi32(127));
        __m128i pow2 = _mm_cvtps_epi32(_mm_castsi128_ps(_mm_slli_epi32(exp, 23)));

        return vec4x32u{_mm_mullo_epi32(decay(v), pow2)};
    }

    auto vec4x32u_mullo_pow2 = vector_test_bench<vec4x32u, vec4x32u_mullo_pow2_impl>;

    BENCHMARK(shift_left_32u::vec4x32u_mullo_pow2);

    #endif



    /* These handle the case where w >= 32 by clearing out the result.
    #if defined(AVEL_SSE4_1)

    vec4x32u vec4x32u_mullo_pow2_N0_impl(vec4x32u v, vec4x32u w) {
        __m128i exp = _mm_add_epi32(decay(w), _mm_set1_epi32(127));
        __m128i pow2 = _mm_cvtps_epi32(_mm_castsi128_ps(_mm_slli_epi32(exp, 23)));

        __m128i mask = _mm_srai_epi32(_mm_slli_epi32(decay(w), 26), 32);
        __m128i mul_result = _mm_mullo_epi32(decay(v), pow2);
        __m128i result = _mm_andnot_si128(mask, mul_result);
        return vec4x32u{result};
    }

    auto vec4x32u_mullo_pow2_N0 = vector_test_bench<vec4x32u, vec4x32u_mullo_pow2_N0_impl>;

    BENCHMARK(shift_left_32u::vec4x32u_mullo_pow2_N0);

    #endif



    #if defined(AVEL_SSE4_1)

    vec4x32u vec4x32u_mullo_pow2_N1_impl(vec4x32u v, vec4x32u w) {
        __m128i exp = _mm_add_epi32(decay(w), _mm_set1_epi32(127));
        __m128i pow2 = _mm_cvtps_epi32(_mm_castsi128_ps(_mm_slli_epi32(exp, 23)));

        __m128i mask = _mm_srai_epi32(_mm_slli_epi32(decay(w), 26), 32);
        __m128i pow2_masked = _mm_andnot_si128(mask, pow2);

        return vec4x32u{_mm_mullo_epi32(decay(v), pow2_masked)};
    }

    auto vec4x32u_mullo_pow2_N1 = vector_test_bench<vec4x32u, vec4x32u_mullo_pow2_N1_impl>;

    BENCHMARK(shift_left_32u::vec4x32u_mullo_pow2_N1);

    #endif



    #if defined(AVEL_SSE4_1)

    vec4x32u vec4x32u_mullo_pow2_N2_impl(vec4x32u v, vec4x32u w) {
        __m128i mask = _mm_srai_epi32(_mm_slli_epi32(decay(w), 26), 32);

        __m128i exp = _mm_add_epi32(decay(w), _mm_set1_epi32(127));
        __m128i exp_field = _mm_slli_epi32(exp, 23);
        __m128i exp_field_masked = _mm_andnot_si128(mask, exp_field);
        __m128i pow2 = _mm_cvtps_epi32(_mm_castsi128_ps(exp_field_masked));

        return vec4x32u{_mm_mullo_epi32(decay(v), pow2)};
    }

    auto vec4x32u_mullo_pow2_N2 = vector_test_bench<vec4x32u, vec4x32u_mullo_pow2_N2_impl>;

    BENCHMARK(shift_left_32u::vec4x32u_mullo_pow2_N2);

    #endif



    #if defined(AVEL_SSE4_1)

    vec4x32u vec4x32u_mullo_pow2_N3_impl(vec4x32u v, vec4x32u w) {
        __m128i mask = _mm_srai_epi32(_mm_slli_epi32(decay(w), 26), 32);

        __m128i exp = _mm_add_epi32(decay(w), _mm_set1_epi32(127));
        __m128i exp_masked = _mm_andnot_si128(mask, exp);

        __m128i exp_field = _mm_slli_epi32(exp_masked, 23);
        __m128i pow2 = _mm_cvtps_epi32(_mm_castsi128_ps(exp_field));

        return vec4x32u{_mm_mullo_epi32(decay(v), pow2)};
    }

    auto vec4x32u_mullo_pow2_N3 = vector_test_bench<vec4x32u, vec4x32u_mullo_pow2_N3_impl>;

    BENCHMARK(shift_left_32u::vec4x32u_mullo_pow2_N3);

    #endif



    #if defined(AVEL_SSE4_1)

    vec4x32u vec4x32u_mullo_pow2_N4_impl(vec4x32u v, vec4x32u w) {
        __m128i mask = _mm_srai_epi32(_mm_slli_epi32(decay(w), 26), 32);

        __m128i exp = _mm_add_epi32(decay(w), _mm_andnot_si128(mask, _mm_set1_epi32(127)));
        __m128i exp_field = _mm_slli_epi32(exp, 23);
        __m128i pow2 = _mm_cvtps_epi32(_mm_castsi128_ps(exp_field));

        return vec4x32u{_mm_mullo_epi32(decay(v), pow2)};
    }

    auto vec4x32u_mullo_pow2_N4 = vector_test_bench<vec4x32u, vec4x32u_mullo_pow2_N4_impl>;

    BENCHMARK(shift_left_32u::vec4x32u_mullo_pow2_N4);

    #endif



    #if defined(AVEL_SSE4_1)

    vec4x32u vec4x32u_mullo_pow2_N5_impl(vec4x32u v, vec4x32u w) {
        __m128i exp = _mm_add_epi32(decay(w), _mm_set1_epi32(127));
        __m128i exp_field = _mm_slli_epi32(exp, 23);
        __m128i pow2 = _mm_cvtps_epi32(_mm_castsi128_ps(exp_field));

        __m128i mask = _mm_srai_epi32(_mm_slli_epi32(decay(w), 26), 32);
        __m128i content_masked = _mm_andnot_si128(mask, decay(v));

        return vec4x32u{_mm_mullo_epi32(content_masked, pow2)};
    }

    auto vec4x32u_mullo_pow2_N5 = vector_test_bench<vec4x32u, vec4x32u_mullo_pow2_N5_impl>;

    BENCHMARK(shift_left_32u::vec4x32u_mullo_pow2_N5);

    #endif
    */



    #if defined(AVEL_SSE2)

    vec4x32u vec4x32u_psrld_scalarized_impl(vec4x32u v, vec4x32u w) {
        //TODO: Test
        auto zeros = _mm_setzero_si128();

        auto x_half0 = _mm_unpacklo_epi32(decay(v), zeros);
        auto x_half1 = _mm_unpackhi_epi32(decay(v), zeros);

        auto y_half0 = _mm_unpacklo_epi32(decay(w), zeros);
        auto y_half1 = _mm_unpackhi_epi32(decay(w), zeros);

        auto X0 = _mm_sll_epi32(x_half0, y_half0);
        auto X1 = _mm_sll_epi32(x_half0, _mm_srli_si128(y_half0, 8));
        auto X2 = _mm_sll_epi32(x_half1, y_half1);
        auto X3 = _mm_sll_epi32(x_half1, _mm_srli_si128(y_half1, 8));

        auto t0 = _mm_unpacklo_epi64(X0, X2);
        auto t1 = _mm_unpackhi_epi64(X1, X3);

        auto t2 = _mm_slli_si128(t1, 0x4);

        return vec4x32u{_mm_or_si128(t0, t2)};
    }

    auto vec4x32u_psrld_scalarized = vector_test_bench<vec4x32u, vec4x32u_psrld_scalarized_impl>;

    BENCHMARK(shift_left_32u::vec4x32u_psrld_scalarized);

    #endif



    #if defined(AVEL_AVX2)

    vec4x32u vec4x32u_vpsllvd_impl(vec4x32u v, vec4x32u w) {
        return vec4x32u{_mm_sllv_epi32(decay(v), decay(w))};
    }

    auto vec4x32u_vpsllvd = vector_test_bench<vec4x32u, vec4x32u_vpsllvd_impl>;

    BENCHMARK(shift_left_32u::vec4x32u_vpsllvd);

    #endif

    //=====================================================
    // vec8x32u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec8x32u vec8x32u_vpsllvd_impl(vec8x32u v, vec8x32u w) {
        return vec8x32u{_mm256_sllv_epi32(decay(v), decay(w))};
    }

    auto vec8x32u_vpsllvd = vector_test_bench<vec8x32u, vec8x32u_vpsllvd_impl>;

    BENCHMARK(shift_left_32u::vec8x32u_vpsllvd);

    #endif

    //=====================================================
    // vec16x32u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512F)

    vec16x32u vec16x32u_vpsllvd_impl(vec16x32u v, vec16x32u w) {
        return vec16x32u{_mm512_sllv_epi32(decay(v), decay(w))};
    }

    auto vec16x32u_vpsllvd = vector_test_bench<vec16x32u, vec16x32u_vpsllvd_impl>;

    BENCHMARK(shift_left_32u::vec16x32u_vpsllvd);

    #endif

}

#endif //AVEL_SHIFT_LEFT_32U_HPP
