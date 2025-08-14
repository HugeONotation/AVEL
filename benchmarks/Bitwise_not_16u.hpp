#ifndef AVEL_BITWISE_NOT_16U_HPP
#define AVEL_BITWISE_NOT_16U_HPP

namespace avel::benchmarks::bitwise_not_16u {

    //=====================================================
    // scalar 16u benchmarks
    //=====================================================

    std::uint16_t scalar_native_impl(std::uint16_t x) {
        return ~x;
    }

    auto scalar_native = scalar_test_bench<std::uint16_t, scalar_native_impl>;

    BENCHMARK(bitwise_not_16u::scalar_native);

    //=====================================================
    // vec8x16u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec8x16u vec8x16u_andnot_impl(vec8x16u x) {
        return vec8x16u{_mm_andnot_si128(decay(x), _mm_set1_epi32(-1))};
    }

    auto vec8x16u_andnot = vector_test_bench<vec8x16u, vec8x16u_andnot_impl>;

    BENCHMARK(bitwise_not_16u::vec8x16u_andnot);

    #endif



    #if defined(AVEL_AVX512VL)

    vec8x16u vec8x16u_ternlog_impl(vec8x16u x) {
        return vec8x16u{_mm_ternarylogic_epi32(decay(x), decay(x), decay(x), 0x01)};
    }

    auto vec8x16u_ternlog = vector_test_bench<vec8x16u, vec8x16u_ternlog_impl>;

    BENCHMARK(bitwise_not_16u::vec8x16u_ternlog);

    #endif

    //=====================================================
    // vec16x16u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec16x16u vec16x16u_andnot_impl(vec16x16u x) {
        return vec16x16u{_mm256_andnot_si256(decay(x), _mm256_set1_epi32(-1))};
    }

    auto vec16x16u_andnot = vector_test_bench<vec16x16u, vec16x16u_andnot_impl>;

    BENCHMARK(bitwise_not_16u::vec16x16u_andnot);

    #endif



    #if defined(AVEL_AVX512VL)

    vec16x16u vec16x16u_ternlog_impl(vec16x16u x) {
        return vec16x16u{_mm256_ternarylogic_epi32(decay(x), decay(x), decay(x), 0x01)};
    }

    auto vec16x16u_ternlog = vector_test_bench<vec16x16u, vec16x16u_ternlog_impl>;

    BENCHMARK(bitwise_not_16u::vec16x16u_ternlog);

    #endif

    //=====================================================
    // vec32x16u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512BW)

    vec32x16u vec32x16u_ternlog_impl(vec32x16u x) {
        return vec32x16u{_mm512_ternarylogic_epi32(decay(x), decay(x), decay(x), 0x01)};
    }

    auto vec32x16u_ternlog = vector_test_bench<vec32x16u, vec32x16u_ternlog_impl>;

    BENCHMARK(bitwise_not_16u::vec32x16u_ternlog);

    #endif

}

#endif //AVEL_BITWISE_NOT_16U_HPP
