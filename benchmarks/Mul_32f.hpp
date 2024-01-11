#ifndef AVEL_MUL_32F_HPP
#define AVEL_MUL_32F_HPP

namespace avel::benchmarks::mul_32f {

    //=====================================================
    // scalar 8u benchmarks
    //=====================================================

    float scalar_native_impl(float x, float y) {
        return x * y;
    }

    auto scalar_native = scalar_test_bench<float, scalar_native_impl>;

    BENCHMARK(mul_32f::scalar_native);

    //=====================================================
    // vec4x32f benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec4x32f vec4x32f_mulps_impl(vec4x32f v, vec4x32f w) {
        return vec4x32f{_mm_mul_ps(decay(v), decay(w))};
    }

    auto vec4x32f_mulps = vector_test_bench<vec4x32f, vec4x32f_mulps_impl>;

    BENCHMARK(mul_32f::vec4x32f_mulps);

    #endif

    //=====================================================
    // vec8x32f benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec8x32f vec8x32f_mulps_impl(vec8x32f v, vec8x32f w) {
        return vec8x32f{_mm256_mul_ps(decay(v), decay(w))};
    }

    auto vec8x32f_mulps = vector_test_bench<vec8x32f, vec8x32f_mulps_impl>;

    BENCHMARK(mul_32f::vec8x32f_mulps);

    #endif

    //=====================================================
    // vec16x32f benchmarks
    //=====================================================

    #if defined(AVEL_AVX512F)

    vec16x32f vec16x32f_mulps_impl(vec16x32f v, vec16x32f w) {
        return vec16x32f{_mm512_mul_ps(decay(v), decay(w))};
    }

    auto vec16x32f_mulps = vector_test_bench<vec16x32f, vec16x32f_mulps_impl>;

    BENCHMARK(mul_32f::vec16x32f_mulps);

    #endif

}

#endif //AVEL_MUL_32F_HPP
