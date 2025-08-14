#ifndef AVEL_BITWISE_OR_64U_HPP
#define AVEL_BITWISE_OR_64U_HPP

namespace avel::benchmarks::bitwise_or_64u {

    //=====================================================
    // scalar 64u benchmarks
    //=====================================================

    std::uint64_t scalar_native_impl(std::uint64_t x, std::uint64_t y) {
        return x | y;
    }

    auto scalar_native = scalar_test_bench<std::uint64_t, scalar_native_impl>;

    BENCHMARK(bitwise_or_64u::scalar_native);

    //=====================================================
    // vec2x64u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec2x64u vec2x64u_por_impl(vec2x64u x, vec2x64u y) {
        return vec2x64u{_mm_or_si128(decay(x), decay(y))};
    }

    auto vec2x64u_por = vector_test_bench<vec2x64u, vec2x64u_por_impl>;

    BENCHMARK(bitwise_or_64u::vec2x64u_por);

    #endif

    //=====================================================
    // vec4x64u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec4x64u vec4x64u_vpor_impl(vec4x64u x, vec4x64u y) {
        return vec4x64u{_mm256_or_si256(decay(x), decay(y))};
    }

    auto vec4x64u_vpor = vector_test_bench<vec4x64u, vec4x64u_vpor_impl>;

    BENCHMARK(bitwise_or_64u::vec4x64u_vpor);

    #endif

    //=====================================================
    // vec8x64u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512F)

    vec8x64u vec8x64u_vpord_impl(vec8x64u x, vec8x64u y) {
        return vec8x64u{_mm512_or_si512(decay(x), decay(y))};
    }

    auto vec8x64u_vpord = vector_test_bench<vec8x64u, vec8x64u_vpord_impl>;

    BENCHMARK(bitwise_or_64u::vec8x64u_vpord);

    #endif

}

#endif //AVEL_BITWISE_OR_64U_HPP
