#ifndef AVEL_DIV_64F_HPP
#define AVEL_DIV_64F_HPP

namespace avel::benchmarks::div_64f {

    //=====================================================
    // scalar 32f benchmarks
    //=====================================================

    double scalar_native_impl(double x, double y) {
        return x / y;
    }

    auto scalar_native = scalar_test_bench<double, scalar_native_impl>;

    BENCHMARK(div_64f::scalar_native);

    //=====================================================
    // vec2x64f benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec2x64f vec2x64f_div_impl(vec2x64f v, vec2x64f w) {
        return vec2x64f{_mm_div_pd(decay(v), decay(w))};
    }

    auto vec2x64f_div = vector_test_bench<vec2x64f, vec2x64f_div_impl>;

    BENCHMARK(div_64f::vec2x64f_div);

    #endif

    //=====================================================
    // vec4x64f benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec4x64f vec4x64f_div_impl(vec4x64f v, vec4x64f w) {
        return vec4x64f{_mm256_div_pd(decay(v), decay(w))};
    }

    auto vec4x64f_div = vector_test_bench<vec4x64f, vec4x64f_div_impl>;

    BENCHMARK(div_64f::vec4x64f_div);

    #endif

    //=====================================================
    // vec8x64f benchmarks
    //=====================================================

    #if defined(AVEL_AVX512F)

    vec8x64f vec8x64f_div_impl(vec8x64f v, vec8x64f w) {
        return vec8x64f{_mm512_div_pd(decay(v), decay(w))};
    }

    auto vec8x64f_div = vector_test_bench<vec8x64f, vec8x64f_div_impl>;

    BENCHMARK(div_64f::vec8x64f_div);

    #endif

}

#endif //AVEL_DIV_64F_HPP
