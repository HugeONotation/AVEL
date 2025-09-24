#ifndef AVEL_EXCLUSIVE_PREFIX_SUM_64U_HPP
#define AVEL_EXCLUSIVE_PREFIX_SUM_64U_HPP

namespace avel::benchmarks::exclusive_prefix_sum64u {

    /*
    //=====================================================
    // vec2x64u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2) && !defined(AVEL_AVX512VL)

    vec2x64u vec2x64u_vector_shift_impl(mask2x64u m) {
        return vec2x64u{_mm_bslli_si128(decay(m))};
    }

    auto vec2x64u_vector_shift = mask_test_bench<vec2x64u, vec2x64u_vector_shift_impl>;

    BENCHMARK(exclusive_prefix_sum64u::vec2x64u_vector_shift);

    #endif



    #if defined(AVEL_AVX512CD) && defined(AVX512VPOPCNTDQ) && defined(AVEL_AVX512VL)

    vec2x64u vec2x64u_mask_shift_impl(mask2x64u m) {
        return vec2x64u{_mm_maskz_sub_epi64(decay(m) << 1, _mm_setzero_si128(), _mm_set1_epi32(-1))};
    }

    auto vec2x64u_mask_shift = mask_test_bench<vec2x64u, vec2x64u_mask_shift_impl>;

    BENCHMARK(exclusive_prefix_sum64u::vec2x64u_mask_shift);

    #endif

    //=====================================================
    // vec4x64u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2) && !defined(AVEL_AVX512VL)

    vec4x64u vec4x64u_movemask_impl(mask4x64u m) {
        constexpr std::uint32_t mask_mask = 0x01010101;
        std::uint32_t mask = _mm256_movemask_epi8(decay(m));
        std::uint32_t masked_mask = mask & mask_mask;

        __m256i t0 = _mm256_set1_epi64x(masked_mask << 32);
        t0 = _mm256_and_si256(t0, _mm256_set_epi64(0x00000000, 0x000000ff, 0x0000ffff, 0x00ffffff));
        __m256i t1 = _mm256_add_epi64(t0, _mm256_slli_epi32(t0, 16));
        __m256i t2 = _mm256_add_epi64(t1, _mm256_slli_epi32(t1, 8));
        __m256i t3 = _mm256_srli_epi64(t2, 56);
        return vec4x64u{t3};
    }

    auto vec4x64u_movemask = mask_test_bench<vec4x64u, vec4x64u_movemask_impl>;

    BENCHMARK(exclusive_prefix_sum64u::vec4x64u_movemask);

    #endif



    #if defined(AVEL_AVX2) && !defined(AVEL_AVX512VL)

    vec4x64u vec4x64u_linear_sum_impl(mask4x64u m) {
        __m256i ret= _mm256_setzero_si256();

        __m256i t0 = _mm256_andnot_si256(_mm256_set_epi64x(0x0000000000000000, 0xffffffff00000000), decay(m));
        __m256i t1 = _mm256_permute4x64_epi64(t0, 0x93);
        __m256i t2 = _mm256_permute4x64_epi64(t0, 0x4f);
        __m256i t3 = _mm256_permute4x64_epi64(t0, 0x3f);

        ret = _mm256_sub_epi64(ret, t1);
        ret = _mm256_sub_epi64(ret, t2);
        ret = _mm256_sub_epi64(ret, t3);

        return vec4x64u{t3};
    }

    auto vec4x64u_linear_sum = mask_test_bench<vec4x64u, vec4x64u_linear_sum_impl>;

    BENCHMARK(exclusive_prefix_sum64u::vec4x64u_linear_sum);
    #endif


    #if defined(AVEL_AVX512CD) && defined(AVX512VPOPCNTDQ) && defined(AVEL_AVX512VL)

    vec4x64u vec4x64u_broadcastm_popcnt_impl(mask4x64u m) {
        static constexpr alignas(32) std::uint64_t relevant_bits[4] {
            0x00000000, 0x00000001, 0x00000003, 0x00000007
        };

        auto mask_lanes = _mm256_broadcastmw_epi64(m);
        auto masked_mask = _mm256_and_si256(mask_lanes, _mm256_load_epi64(relevant_bits));
        auto result = _mm256_popcnt_epi64(masked_mask);
        return vec4x64u{result};
    }

    auto vec4x64u_broadcastm_popcnt = mask_test_bench<vec4x64u, vec4x64u_broadcastm_popcnt_impl>;

    BENCHMARK(exclusive_prefix_sum64u::vec4x64u_broadcastm_popcnt);

    #endif

    //=====================================================
    // vec8x64u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512F)

    vec8x64u vec8x64u_broadcast_software_popcnt_impl(mask8x64u m) {
		__m512i t = _mm512_set1_epi64(decay(m));

        const auto m0 = _mm512_set1_epi8(0x55);
        const auto m1 = _mm512_set1_epi8(0x33);
        const auto m2 = _mm512_set1_epi8(0x0f);

        t = _mm512_sub_epi8(t, _mm512_and_si512(_mm512_srli_epi16(t,  1), m0));
        t = _mm512_add_epi8(_mm512_and_si512(t, m1), _mm512_and_si512(_mm512_srli_epi16(t, 2), m1));
        t = _mm512_and_si512(_mm512_add_epi8(t, _mm512_srli_epi16(t, 4)), m2);

        return vec8x64u{t};
    }

    auto vec8x64u_broadcast_software_popcnt = mask_test_bench<vec8x64u, vec8x64u_broadcast_software_popcnt_impl>;

    BENCHMARK(exclusive_prefix_sum64u::vec8x64u_broadcast_software_popcnt);

    #endif



    #if defined(AVX512VPOPCNTDQ)

    vec8x64u vec8x64u_broadcast_popcnt_impl(mask m) {
        alignas(64) static constexpr std::uint64_t relevant_bits[8] {
            0x00000000, 0x00000001, 0x00000003, 0x00000007,
            0x0000000f, 0x0000001f, 0x0000003f, 0x0000007f
        };

        auto mask_lanes = _mm512_broadcast_epi64(decay(m));
        auto masked_mask = _mm512_and_si512(mask_lanes, _mm512_load_epi64(relevant_bits));
        auto result = _mm512_popcnt_epi64(masked_mask);
        return vec8x64u{result};
    }

    auto vec8x64u_broadcastm_popcnt = mask_test_bench<vec8x64u, vec8x64u_broadcastm_popcnt_impl>;

    BENCHMARK(exclusive_prefix_sum64u::vec8x64u_broadcastm_popcnt);

    #endif



    #if defined(AVEL_AVX512CD) && defined(AVX512VPOPCNTDQ)

    vec8x64u vec8x64u_broadcastm_popcnt_impl(mask m) {
        alignas(64) static constexpr std::uint64_t relevant_bits[8] {
            0x00000000, 0x00000001, 0x00000003, 0x00000007,
            0x0000000f, 0x0000001f, 0x0000003f, 0x0000007f
        };

        auto mask_lanes = _mm512_broadcastmw_epi64(m);
        auto masked_mask = _mm512_and_si512(mask_lanes, _mm512_load_epi64(relevant_bits));
        auto result = _mm512_popcnt_epi64(masked_mask);
        return vec8x64u{result};
    }

    auto vec8x64u_broadcastm_popcnt = mask_test_bench<vec8x64u, vec8x64u_broadcastm_popcnt_impl>;

    BENCHMARK(exclusive_prefix_sum64u::vec8x64u_broadcastm_popcnt);

    #endif
    */

}

#endif //AVEL_EXCLUSIVE_PREFIX_SUM_64U_HPP
