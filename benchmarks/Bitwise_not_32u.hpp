#ifndef AVEL_BITWISE_NOT_32U_HPP
#define AVEL_BITWISE_NOT_32U_HPP

namespace avel::benchmarks::bitwise_not_32u {

    //=====================================================
    // scalar 32u benchmarks
    //=====================================================

    std::uint32_t scalar_native_impl(std::uint32_t x) {
        return ~x;
    }

    auto scalar_native = scalar_test_bench<std::uint32_t, scalar_native_impl>;

    BENCHMARK(bitwise_not_32u::scalar_native);

    //=====================================================
    // vec4x32u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec4x32u vec4x32u_andnot_impl(vec4x32u x) {
        return vec4x32u{_mm_andnot_si128(decay(x), _mm_set1_epi32(-1))};
    }

    auto vec4x32u_andnot = vector_test_bench<vec4x32u, vec4x32u_andnot_impl>;

    BENCHMARK(bitwise_not_32u::vec4x32u_andnot);

    #endif



    #if defined(AVEL_AVX512VL)

    vec4x32u vec4x32u_ternlog_impl(vec4x32u x) {
        return vec4x32u{_mm_ternarylogic_epi32(decay(x), decay(x), decay(x), 0x01)};
    }

    auto vec4x32u_ternlog = vector_test_bench<vec4x32u, vec4x32u_ternlog_impl>;

    BENCHMARK(bitwise_not_32u::vec4x32u_ternlog);

    #endif

    //=====================================================
    // vec8x32u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec8x32u vec8x32u_andnot_impl(vec8x32u x) {
        return vec8x32u{_mm256_andnot_si256(decay(x), _mm256_set1_epi32(-1))};
    }

    auto vec8x32u_andnot = vector_test_bench<vec8x32u, vec8x32u_andnot_impl>;

    BENCHMARK(bitwise_not_32u::vec8x32u_andnot);

    #endif



    #if defined(AVEL_AVX512VL)

    vec8x32u vec8x32u_ternlog_impl(vec8x32u x) {
        return vec8x32u{_mm256_ternarylogic_epi32(decay(x), decay(x), decay(x), 0x01)};
    }

    auto vec8x32u_ternlog = vector_test_bench<vec8x32u, vec8x32u_ternlog_impl>;

    BENCHMARK(bitwise_not_32u::vec8x32u_ternlog);

    #endif

    //=====================================================
    // vec16x32u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512BW)

    vec16x32u vec16x32u_ternlog_impl(vec16x32u x) {
        return vec16x32u{_mm512_ternarylogic_epi32(decay(x), decay(x), decay(x), 0x01)};
    }

    auto vec16x32u_ternlog = vector_test_bench<vec16x32u, vec16x32u_ternlog_impl>;

    BENCHMARK(bitwise_not_32u::vec16x32u_ternlog);

    #endif

}

#endif //AVEL_BITWISE_NOT_32U_HPP
