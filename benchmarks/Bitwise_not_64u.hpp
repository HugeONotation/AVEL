#ifndef AVEL_BITWISE_NOT_64U_HPP
#define AVEL_BITWISE_NOT_64U_HPP

namespace avel::benchmarks::bitwise_not_64u {

    //=====================================================
    // scalar 64u benchmarks
    //=====================================================

    std::uint64_t scalar_native_impl(std::uint64_t x) {
        return ~x;
    }

    auto scalar_native = scalar_test_bench<std::uint64_t, scalar_native_impl>;

    BENCHMARK(bitwise_not_64u::scalar_native);

    //=====================================================
    // vec2x64u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec2x64u vec2x64u_andnot_impl(vec2x64u x) {
        return vec2x64u{_mm_andnot_si128(decay(x), _mm_set1_epi32(-1))};
    }

    auto vec2x64u_andnot = vector_test_bench<vec2x64u, vec2x64u_andnot_impl>;

    BENCHMARK(bitwise_not_64u::vec2x64u_andnot);

    #endif



    #if defined(AVEL_AVX512VL)

    vec2x64u vec2x64u_ternlog_impl(vec2x64u x) {
        return vec2x64u{_mm_ternarylogic_epi64(decay(x), decay(x), decay(x), 0x01)};
    }

    auto vec2x64u_ternlog = vector_test_bench<vec2x64u, vec2x64u_ternlog_impl>;

    BENCHMARK(bitwise_not_64u::vec2x64u_ternlog);

    #endif

    //=====================================================
    // vec4x64u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec4x64u vec4x64u_andnot_impl(vec4x64u x) {
        return vec4x64u{_mm256_andnot_si256(decay(x), _mm256_set1_epi32(-1))};
    }

    auto vec4x64u_andnot = vector_test_bench<vec4x64u, vec4x64u_andnot_impl>;

    BENCHMARK(bitwise_not_64u::vec4x64u_andnot);

    #endif



    #if defined(AVEL_AVX512VL)

    vec4x64u vec4x64u_ternlog_impl(vec4x64u x) {
        return vec4x64u{_mm256_ternarylogic_epi64(decay(x), decay(x), decay(x), 0x01)};
    }

    auto vec4x64u_ternlog = vector_test_bench<vec4x64u, vec4x64u_ternlog_impl>;

    BENCHMARK(bitwise_not_64u::vec4x64u_ternlog);

    #endif

    //=====================================================
    // vec8x64u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512BW)

    vec8x64u vec8x64u_ternlog_impl(vec8x64u x) {
        return vec8x64u{_mm512_ternarylogic_epi64(decay(x), decay(x), decay(x), 0x01)};
    }

    auto vec8x64u_ternlog = vector_test_bench<vec8x64u, vec8x64u_ternlog_impl>;

    BENCHMARK(bitwise_not_64u::vec8x64u_ternlog);

    #endif

}

#endif //AVEL_BITWISE_NOT_64U_HPP
