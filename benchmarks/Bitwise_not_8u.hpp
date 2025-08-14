#ifndef AVEL_BITWISE_NOT_8U_HPP
#define AVEL_BITWISE_NOT_8U_HPP

namespace avel::benchmarks::bitwise_not_8u {

    //=====================================================
    // scalar 8u benchmarks
    //=====================================================

    std::uint8_t scalar_native_impl(std::uint8_t x) {
        return ~x;
    }

    auto scalar_native = scalar_test_bench<std::uint8_t, scalar_native_impl>;

    BENCHMARK(bitwise_not_8u::scalar_native);

    //=====================================================
    // vec16x8u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec16x8u vec16x8u_andnot_impl(vec16x8u x) {
        return vec16x8u{_mm_andnot_si128(decay(x), _mm_set1_epi32(-1))};
    }

    auto vec16x8u_andnot = vector_test_bench<vec16x8u, vec16x8u_andnot_impl>;

    BENCHMARK(bitwise_not_8u::vec16x8u_andnot);

    #endif



    #if defined(AVEL_AVX512VL)

    vec16x8u vec16x8u_ternlog_impl(vec16x8u x) {
        return vec16x8u{_mm_ternarylogic_epi32(decay(x), decay(x), decay(x), 0x01)};
    }

    auto vec16x8u_ternlog = vector_test_bench<vec16x8u, vec16x8u_ternlog_impl>;

    BENCHMARK(bitwise_not_8u::vec16x8u_ternlog);

    #endif

    //=====================================================
    // vec32x8u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec32x8u vec32x8u_andnot_impl(vec32x8u x) {
        return vec32x8u{_mm256_andnot_si256(decay(x), _mm256_set1_epi32(-1))};
    }

    auto vec32x8u_andnot = vector_test_bench<vec32x8u, vec32x8u_andnot_impl>;

    BENCHMARK(bitwise_not_8u::vec32x8u_andnot);

    #endif



    #if defined(AVEL_AVX512VL)

    vec32x8u vec32x8u_ternlog_impl(vec32x8u x) {
        return vec32x8u{_mm256_ternarylogic_epi32(decay(x), decay(x), decay(x), 0x01)};
    }

    auto vec32x8u_ternlog = vector_test_bench<vec32x8u, vec32x8u_ternlog_impl>;

    BENCHMARK(bitwise_not_8u::vec32x8u_ternlog);

    #endif

    //=====================================================
    // vec64x8u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512BW)

    vec64x8u vec64x8u_ternlog_impl(vec64x8u x) {
        return vec64x8u{_mm512_ternarylogic_epi32(decay(x), decay(x), decay(x), 0x01)};
    }

    auto vec64x8u_ternlog = vector_test_bench<vec64x8u, vec64x8u_ternlog_impl>;

    BENCHMARK(bitwise_not_8u::vec64x8u_ternlog);

    #endif

}

#endif //AVEL_BITWISE_NOT_8U_HPP
