#ifndef AVEL_BITWISE_AND_64U_HPP
#define AVEL_BITWISE_AND_64U_HPP

namespace avel::benchmarks::bitwise_and_64u {

    //=====================================================
    // scalar 64u benchmarks
    //=====================================================

    std::uint64_t scalar_native_impl(std::uint64_t x, std::uint64_t y) {
        return x & y;
    }

    auto scalar_native = scalar_test_bench<std::uint64_t, scalar_native_impl>;

    BENCHMARK(bitwise_and_64u::scalar_native);

    //=====================================================
    // vec2x64u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec2x64u vec2x64u_pand_impl(vec2x64u x, vec2x64u y) {
        return vec2x64u{_mm_and_si128(decay(x), decay(y))};
    }

    auto vec2x64u_pand = vector_test_bench<vec2x64u, vec2x64u_pand_impl>;

    BENCHMARK(bitwise_and_64u::vec2x64u_pand);

    #endif

    //=====================================================
    // vec4x64u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec4x64u vec4x64u_vpand_impl(vec4x64u x, vec4x64u y) {
        return vec4x64u{_mm256_and_si256(decay(x), decay(y))};
    }

    auto vec4x64u_vpand = vector_test_bench<vec4x64u, vec4x64u_vpand_impl>;

    BENCHMARK(bitwise_and_64u::vec4x64u_vpand);

    #endif

    //=====================================================
    // vec8x64u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512F)

    vec8x64u vec8x64u_vpandd_impl(vec8x64u x, vec8x64u y) {
        return vec8x64u{_mm512_and_si512(decay(x), decay(y))};
    }

    auto vec8x64u_vpandd = vector_test_bench<vec8x64u, vec8x64u_vpandd_impl>;

    BENCHMARK(bitwise_and_64u::vec8x64u_vpandd);

    #endif

}

#endif //AVEL_BITWISE_AND_64U_HPP
