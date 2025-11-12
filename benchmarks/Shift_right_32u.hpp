#ifndef AVEL_SHIFT_RIGHT_32U_HPP
#define AVEL_SHIFT_RIGHT_32U_HPP

namespace avel::benchmarks::shift_right_32u {

    //=====================================================
    // scalar 32u benchmarks
    //=====================================================

    std::uint32_t scalar_native_impl(std::uint32_t x, std::uint32_t y) {
        return x >> y;
    }

    auto scalar_native = scalar_test_bench<std::uint32_t, scalar_native_impl>;

    BENCHMARK(shift_right_32u::scalar_native);

    //=====================================================
    // vec4x32u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec4x32u vec4x32u_mul_pow2_impl(vec4x32u v, vec4x32u w) {
        __m128i exp = _mm_sub_epi32(_mm_set1_epi32(158), decay(w));
        __m128i pow2 = _mm_cvtps_epi32(_mm_castsi128_ps(_mm_slli_epi32(exp, 23)));

        auto lhs_lo = decay(v);
        auto lhs_hi = _mm_srli_epi64(decay(v), 32);

        auto rhs_lo = pow2;
        auto rhs_hi = _mm_srli_epi64(pow2, 32);

        auto lo = _mm_mul_epu32(lhs_lo, rhs_lo);
        auto hi = _mm_mul_epu32(lhs_hi, rhs_hi);

        auto lo_shifted = _mm_srli_epi64(lo, 31);
        auto hi_shifted = _mm_srli_epi64(hi, 31);

        auto half0 = _mm_unpacklo_epi32(lo_shifted, hi_shifted);
        auto half1 = _mm_unpackhi_epi32(lo_shifted, hi_shifted);

        auto result = _mm_unpacklo_epi64(half0, half1);
        return vec4x32u{result};
    }

    auto vec4x32u_mul_pow2 = vector_test_bench<vec4x32u, vec4x32u_mul_pow2_impl>;

    BENCHMARK(shift_right_32u::vec4x32u_mul_pow2);

    #endif



    /*
    #if defined(AVEL_SSE2)

    vec4x32u vec4x32u_64f_conversion_impl(vec4x32u v, vec4x32u w) {
        //TODO: Handle case where v has 32-bit wide values. Current solution only works with 31-bit wide values

        __m128d even_doubles = _mm_cvtepi32_pd(decay(v));
        __m128d odd_doubles  = _mm_cvtepi32_pd(decay(w));

        __m128i even_exp_offsets = _mm_slli_epi64(decay(w), 52);
        __m128i odd_exp_offsets  = _mm_slli_epi64(_mm_srli_epi64(decay(w), 32), 52);

        __m128i even_doubles_adjusted = _mm_sub_epi64(_mm_castpd_si128(even_doubles), even_exp_offsets);
        __m128i odd_doubles_adjusted  = _mm_sub_epi64(_mm_castpd_si128(odd_doubles),  odd_exp_offsets );

        __m128i even_results = _mm_cvttpd_epi32(_mm_castsi128_pd(even_doubles_adjusted));
        __m128i odd_results  = _mm_cvttpd_epi32(_mm_castsi128_pd(odd_doubles_adjusted ));

        __m128i results = _mm_or_si128(even_results, _mm_slli_epi64(odd_results, 32));

        return vec4x32u{results};
    }

    auto vec4x32u_64f_conversion = vector_test_bench<vec4x32u, vec4x32u_64f_conversion_impl>;

    BENCHMARK(shift_right_32u::vec4x32u_64f_conversion);

    #endif
    */



    #if defined(AVEL_SSE2)

    vec4x32u vec4x32u_psrld_scalarized_impl(vec4x32u v, vec4x32u w) {
        auto zeros = _mm_setzero_si128();

        auto x_half0 = _mm_unpacklo_epi32(decay(v), zeros);
        auto x_half1 = _mm_unpackhi_epi32(decay(v), zeros);

        auto y_half0 = _mm_unpacklo_epi32(decay(w), zeros);
        auto y_half1 = _mm_unpackhi_epi32(decay(w), zeros);

        auto X0 = _mm_srl_epi32(x_half0, y_half0);
        auto X1 = _mm_srl_epi32(x_half0, _mm_srli_si128(y_half0, 8));
        auto X2 = _mm_srl_epi32(x_half1, y_half1);
        auto X3 = _mm_srl_epi32(x_half1, _mm_srli_si128(y_half1, 8));

        auto t0 = _mm_unpacklo_epi64(X0, X2);
        auto t1 = _mm_unpackhi_epi64(X1, X3);

        auto t2 = _mm_slli_si128(t1, 0x4);

        return vec4x32u{_mm_or_si128(t0, t2)};
    }

    auto vec4x32u_psrld_scalarized = vector_test_bench<vec4x32u, vec4x32u_psrld_scalarized_impl>;

    BENCHMARK(shift_right_32u::vec4x32u_psrld_scalarized);

    #endif



    #if defined(AVEL_AVX2)

    vec4x32u vec4x32u_vpsrlvd_impl(vec4x32u v, vec4x32u w) {
        return vec4x32u{_mm_srlv_epi32(decay(v), decay(w))};
    }

    auto vec4x32u_vpsrlvd = vector_test_bench<vec4x32u, vec4x32u_vpsrlvd_impl>;

    BENCHMARK(shift_right_32u::vec4x32u_vpsrlvd);

    #endif

    //=====================================================
    // vec8x32u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec8x32u vec8x32u_vpsrlvd_impl(vec8x32u v, vec8x32u w) {
        return vec8x32u{_mm256_srlv_epi32(decay(v), decay(w))};
    }

    auto vec8x32u_vpsrlvd = vector_test_bench<vec8x32u, vec8x32u_vpsrlvd_impl>;

    BENCHMARK(shift_right_32u::vec8x32u_vpsrlvd);

    #endif

    //=====================================================
    // vec16x32u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512F)

    vec16x32u vec16x32u_vpsrlvd_impl(vec16x32u v, vec16x32u w) {
        return vec16x32u{_mm512_srlv_epi32(decay(v), decay(w))};
    }

    auto vec16x32u_vpsrlvd = vector_test_bench<vec16x32u, vec16x32u_vpsrlvd_impl>;

    BENCHMARK(shift_right_32u::vec16x32u_vpsrlvd);

    #endif

}

#endif //AVEL_SHIFT_RIGHT_32U_HPP