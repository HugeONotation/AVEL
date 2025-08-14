#ifndef AVEL_BITWISE_OR_16U_HPP
#define AVEL_BITWISE_OR_16U_HPP

namespace avel::benchmarks::bitwise_or_16u {

    //=====================================================
    // scalar 16u benchmarks
    //=====================================================

    std::uint16_t scalar_native_impl(std::uint16_t x, std::uint16_t y) {
        return x | y;
    }

    auto scalar_native = scalar_test_bench<std::uint16_t, scalar_native_impl>;

    BENCHMARK(bitwise_or_16u::scalar_native);

    //=====================================================
    // vec2x64u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec8x16u vec8x16u_por_impl(vec8x16u x, vec8x16u y) {
        return vec8x16u{_mm_or_si128(decay(x), decay(y))};
    }

    auto vec8x16u_por = vector_test_bench<vec8x16u, vec8x16u_por_impl>;

    BENCHMARK(bitwise_or_16u::vec8x16u_por);

    #endif

    //=====================================================
    // vec4x64u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec16x16u vec16x16u_vpor_impl(vec16x16u x, vec16x16u y) {
        return vec16x16u{_mm256_or_si256(decay(x), decay(y))};
    }

    auto vec16x16u_vpor = vector_test_bench<vec16x16u, vec16x16u_vpor_impl>;

    BENCHMARK(bitwise_or_16u::vec16x16u_vpor);

    #endif

    //=====================================================
    // vec8x64u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512F)

    vec32x16u vec32x16u_vpord_impl(vec32x16u x, vec32x16u y) {
        return vec32x16u{_mm512_or_si512(decay(x), decay(y))};
    }

    auto vec32x16u_vpord = vector_test_bench<vec32x16u, vec32x16u_vpord_impl>;

    BENCHMARK(bitwise_or_16u::vec32x16u_vpord);

    #endif

}

#endif //AVEL_BITWISE_OR_16U_HPP
