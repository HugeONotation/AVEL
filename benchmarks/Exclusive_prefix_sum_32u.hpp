#ifndef AVEL_EXCLUSIVE_PREFIX_SUM_32U_HPP
#define AVEL_EXCLUSIVE_PREFIX_SUM_32U_HPP

namespace avel::benchmarks::exclusive_prefix_sum32u {

    //=====================================================
    // vec4x32u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512CD) && defined(AVX512VPOPCNTDQ) && defined(AVEL_AVX512VL)

    vec4x32u vec4x32u_broadcastm_popcnt_impl(mask4x32u m) {
        static constexpr alignas(16) std::uint32_t relevant_bits[4] {
            0x00000000, 0x00000001, 0x00000003, 0x00000007
        };

        auto mask_lanes = _mm5_broadcastmw_epi32(m);
        auto masked_mask = _mm_and_si128(mask_lanes, _mm_load_epi32(relevant_bits));
        auto result = _mm_popcnt_epi32(masked_mask);
        return vec4x32u{result};
    }

    auto vec4x32u_broadcastm_popcnt = mask_test_bench<vec4x32u, vec4x32u_broadcastm_popcnt_impl>;

    BENCHMARK(exclusive_prefix_sum32u::vec4x32u_broadcastm_popcnt);

    #endif

    //=====================================================
    // vec8x32u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512CD) && defined(AVX512VPOPCNTDQ) && defined(AVEL_AVX512VL)

    vec8x32u vec8x32u_broadcastm_popcnt_impl(mask8x32u m) {
        static constexpr alignas(32) std::uint32_t relevant_bits[8] {
            0x00000000, 0x00000001, 0x00000003, 0x00000007,
            0x0000000f, 0x0000001f, 0x0000003f, 0x0000007f
        };

        auto mask_lanes = _mm256_broadcastmw_epi32(m);
        auto masked_mask = _mm256_and_si256(mask_lanes, _mm256_load_epi32(relevant_bits));
        auto result = _mm256_popcnt_epi32(masked_mask);
        return vec8x32u{result};
    }

    auto vec8x32u_broadcastm_popcnt = mask_test_bench<vec16x32u, vec8x32u_broadcastm_popcnt_impl>;

    BENCHMARK(exclusive_prefix_sum32u::vec8x32u_broadcastm_popcnt);

    #endif

    //=====================================================
    // vec16x32u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512CD) && defined(AVX512VPOPCNTDQ)

    vec16x32u vec16x32u_broadcastm_popcnt_impl(mask16x32u m) {
        static constexpr alignas(64) std::uint32_t relevant_bits[16] {
            0x00000000, 0x00000001, 0x00000003, 0x00000007,
            0x0000000f, 0x0000001f, 0x0000003f, 0x0000007f,
            0x000000ff, 0x000001ff, 0x000003ff, 0x000007ff,
            0x00000fff, 0x00001fff, 0x00003fff, 0x00007fff
        };

        auto mask_lanes = _mm512_broadcastmw_epi32(m);
        auto masked_mask = _mm512_and_si512(mask_lanes, _mm512_load_epi32(relevant_bits));
        auto result = _mm512_popcnt_epi32(masked_mask);
        return vec16x32u{result};
    }

    auto vec16x32u_broadcastm_popcnt = mask_test_bench<vec16x32u, vec16x32u_broadcastm_popcnt_impl>;

    BENCHMARK(exclusive_prefix_sum32u::vec16x32u_broadcastm_popcnt);

    #endif

}

#endif //AVEL_EXCLUSIVE_PREFIX_SUM_32U_HPP
