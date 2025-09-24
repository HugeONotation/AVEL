#ifndef AVEL_BITWISE_AND_8U_HPP
#define AVEL_BITWISE_AND_8U_HPP

namespace avel::benchmarks::bitwise_and_8u {

    //=====================================================
    // scalar 16u benchmarks
    //=====================================================

    std::uint8_t scalar_native_impl(std::uint8_t x, std::uint8_t y) {
        return x & y;
    }

    auto scalar_native = scalar_test_bench<std::uint8_t, scalar_native_impl>;

    BENCHMARK(bitwise_and_8u::scalar_native);

    //=====================================================
    // vec2x64u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec16x8u vec16x8u_pand_impl(vec16x8u x, vec16x8u y) {
        return vec16x8u{_mm_and_si128(decay(x), decay(y))};
    }

    auto vec16x8u_pand = vector_test_bench<vec16x8u, vec16x8u_pand_impl>;

    BENCHMARK(bitwise_and_8u::vec16x8u_pand);

    #endif

    //=====================================================
    // vec4x64u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec32x8u vec32x8u_vpand_impl(vec32x8u x, vec32x8u y) {
        return vec32x8u{_mm256_and_si256(decay(x), decay(y))};
    }

    auto vec32x8u_vpand = vector_test_bench<vec32x8u, vec32x8u_vpand_impl>;

    BENCHMARK(bitwise_and_8u::vec32x8u_vpand);

    #endif

    //=====================================================
    // vec8x64u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512BW)

    vec64x8u vec64x8u_vpandd_impl(vec64x8u x, vec64x8u y) {
        return vec64x8u{_mm512_and_si512(decay(x), decay(y))};
    }

    auto vec64x8u_vpandd = vector_test_bench<vec64x8u, vec64x8u_vpandd_impl>;

    BENCHMARK(bitwise_and_8u::vec64x8u_vpandd);

    #endif

}

#endif //AVEL_BITWISE_AND_8U_HPP
