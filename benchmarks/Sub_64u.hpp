#ifndef AVEL_SUB_64U_HPP
#define AVEL_SUB_64U_HPP

namespace avel::benchmarks::sub_64u {

    //=====================================================
    // scalar 64u benchmarks
    //=====================================================

    std::uint64_t scalar_native_impl(std::uint64_t x, std::uint64_t y) {
        return x + y;
    }

    auto scalar_native = scalar_test_bench<std::uint64_t, scalar_native_impl>;

    BENCHMARK(sub_64u::scalar_native);

    //=====================================================
    // vec2x64u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec2x64u vec2x64u_subq_impl(vec2x64u v, vec2x64u w) {
        return vec2x64u{_mm_sub_epi64(decay(v), decay(w))};
    }

    auto vec2x64u_subq = vector_test_bench<vec2x64u, vec2x64u_subq_impl>;

    BENCHMARK(sub_64u::vec2x64u_subq);

    #endif

    //=====================================================
    // vec4x64u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec4x64u vec4x64u_subq_impl(vec4x64u v, vec4x64u w) {
        return vec4x64u{_mm256_sub_epi64(decay(v), decay(w))};
    }

    auto vec4x64u_subq = vector_test_bench<vec4x64u, vec4x64u_subq_impl>;

    BENCHMARK(sub_64u::vec4x64u_subq);

    #endif

    //=====================================================
    // vec8x64u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512F)

    vec8x64u vec8x64u_subq_impl(vec8x64u v, vec8x64u w) {
        return vec8x64u{_mm512_sub_epi64(decay(v), decay(w))};
    }

    auto vec8x64u_subq = vector_test_bench<vec8x64u, vec8x64u_subq_impl>;

    BENCHMARK(sub_64u::vec8x64u_subq);

    #endif

}

#endif //AVEL_SUB_64U_HPP
