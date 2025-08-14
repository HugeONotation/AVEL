#ifndef AVEL_BITWISE_OR_32U_HPP
#define AVEL_BITWISE_OR_32U_HPP

namespace avel::benchmarks::bitwise_or_32u {

    //=====================================================
    // scalar 32u benchmarks
    //=====================================================

    std::uint32_t scalar_native_impl(std::uint32_t x, std::uint32_t y) {
        return x | y;
    }

    auto scalar_native = scalar_test_bench<std::uint32_t, scalar_native_impl>;

    BENCHMARK(bitwise_or_32u::scalar_native);

    //=====================================================
    // vec4x32u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec4x32u vec4x32u_por_impl(vec4x32u x, vec4x32u y) {
        return vec4x32u{_mm_or_si128(decay(x), decay(y))};
    }

    auto vec4x32u_por = vector_test_bench<vec4x32u, vec4x32u_por_impl>;

    BENCHMARK(bitwise_or_32u::vec4x32u_por);

    #endif

    //=====================================================
    // vec8x32u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec8x32u vec8x32u_vpor_impl(vec8x32u x, vec8x32u y) {
        return vec8x32u{_mm256_or_si256(decay(x), decay(y))};
    }

    auto vec8x32u_vpor = vector_test_bench<vec8x32u, vec8x32u_vpor_impl>;

    BENCHMARK(bitwise_or_32u::vec8x32u_vpor);

    #endif

    //=====================================================
    // vec16x32u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512F)

    vec16x32u vec16x32u_vpord_impl(vec16x32u x, vec16x32u y) {
        return vec16x32u{_mm512_or_si512(decay(x), decay(y))};
    }

    auto vec16x32u_vpord = vector_test_bench<vec16x32u, vec16x32u_vpord_impl>;

    BENCHMARK(bitwise_or_32u::vec16x32u_vpord);

    #endif

}

#endif //AVEL_BITWISE_OR_32U_HPP
