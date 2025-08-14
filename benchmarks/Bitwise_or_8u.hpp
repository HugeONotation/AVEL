#ifndef AVEL_BITWISE_OR_8U_HPP
#define AVEL_BITWISE_OR_8U_HPP

namespace avel::benchmarks::bitwise_or_8u {

    //=====================================================
    // scalar 16u benchmarks
    //=====================================================

    std::uint8_t scalar_native_impl(std::uint8_t x, std::uint8_t y) {
        return x | y;
    }

    auto scalar_native = scalar_test_bench<std::uint8_t, scalar_native_impl>;

    BENCHMARK(bitwise_or_8u::scalar_native);

    //=====================================================
    // vec2x64u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec16x8u vec16x8u_por_impl(vec16x8u x, vec16x8u y) {
        return vec16x8u{_mm_or_si128(decay(x), decay(y))};
    }

    auto vec16x8u_por = vector_test_bench<vec16x8u, vec16x8u_por_impl>;

    BENCHMARK(bitwise_or_8u::vec16x8u_por);

    #endif

    //=====================================================
    // vec4x64u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec32x8u vec32x8u_vpor_impl(vec32x8u x, vec32x8u y) {
        return vec32x8u{_mm256_or_si256(decay(x), decay(y))};
    }

    auto vec32x8u_vpor = vector_test_bench<vec32x8u, vec32x8u_vpor_impl>;

    BENCHMARK(bitwise_or_8u::vec32x8u_vpor);

    #endif

    //=====================================================
    // vec8x64u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512F)

    vec64x8u vec64x8u_vpord_impl(vec64x8u x, vec64x8u y) {
        return vec64x8u{_mm512_or_si512(decay(x), decay(y))};
    }

    auto vec64x8u_vpord = vector_test_bench<vec64x8u, vec64x8u_vpord_impl>;

    BENCHMARK(bitwise_or_8u::vec64x8u_vpord);

    #endif

}

#endif //AVEL_BITWISE_OR_8U_HPP
