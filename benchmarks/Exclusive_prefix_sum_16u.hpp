#ifndef AVEL_EXCLUSIVE_PREFIX_SUM_16U_HPP
#define AVEL_EXCLUSIVE_PREFIX_SUM_16U_HPP

namespace avel::benchmarks::exclusive_prefix_sum16u {

    //=====================================================
    // vec8x16u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512BW) && defined(AVEL_AVX512BITALG) && defined(AVEL_AVX512VL)

    vec8x16u vec8x16u_broadcast_popcnt_impl(mask8x16u m) {
        alignas(16) static constexpr std::uint16_t relevant_bits[8] {
            0x0000, 0x0001, 0x0003, 0x0007,
            0x000f, 0x001f, 0x003f, 0x007f
        };

        auto mask_lanes = _mm_set1_epi16(decay(m));
        auto masked_mask = _mm_and_si128(mask_lanes, _mm_load_si128(reinterpret_cast<const __m128i*>(relevant_bits)));
        auto result = _mm_popcnt_epi16(masked_mask);
        return vec8x16u{result};
    }

    auto vec8x16u_broadcast_popcnt = mask_test_bench<vec8x16u, vec8x16u_broadcast_popcnt_impl>;

    BENCHMARK(exclusive_prefix_sum16u::vec8x16u_broadcast_popcnt);

    #endif

    //=====================================================
    // vec16x16u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512BW) && defined(AVEL_AVX512BITALG) && defined(AVEL_AVX512VL)

    vec16x16u vec16x16u_broadcast_popcnt_impl(mask16x16u m) {
        alignas(32) static constexpr std::uint16_t relevant_bits[16] {
            0x00000000, 0x00000001, 0x00000003, 0x00000007,
            0x0000000f, 0x0000001f, 0x0000003f, 0x0000007f,
            0x000000ff, 0x000001ff, 0x000003ff, 0x000007ff,
            0x00000fff, 0x00001fff, 0x00003fff, 0x00007fff
        };

        auto mask_lanes = _mm256_set1_epi16(decay(m));
        auto masked_mask = _mm256_and_si256(mask_lanes, _mm256_load_si256(reinterpret_cast<const __m256i*>(relevant_bits)));
        auto result = _mm256_popcnt_epi16(masked_mask);
        return vec16x16u{result};
    }

    auto vec16x16u_broadcast_popcnt = mask_test_bench<vec16x16u, vec16x16u_broadcast_popcnt_impl>;

    BENCHMARK(exclusive_prefix_sum16u::vec16x16u_broadcast_popcnt);

    #endif

    //=====================================================
    // vec32x16u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512BW) && defined(AVEL_AVX512BITALG) && defined(AVEL_AVX512VL)

    vec32x16u vec32x16u_exclusive_prefix_sum_impl(mask32x16u m) {
        alignas(32) static constexpr std::uint16_t relevant_bits[16] {
            0x00000000, 0x00000001, 0x00000003, 0x00000007,
            0x0000000f, 0x0000001f, 0x0000003f, 0x0000007f,
            0x000000ff, 0x000001ff, 0x000003ff, 0x000007ff,
            0x00000fff, 0x00001fff, 0x00003fff, 0x00007fff
        };

        //TODO: Implement
        return vec32x16u{};
    }

    auto vec32x16u_exclusive_prefix_sum = mask_test_bench<vec32x16u, vec32x16u_exclusive_prefix_sum_impl>;

    BENCHMARK(exclusive_prefix_sum16u::vec32x16u_exclusive_prefix_sum);

    #endif

}

#endif //AVEL_EXCLUSIVE_PREFIX_SUM_16U_HPP
