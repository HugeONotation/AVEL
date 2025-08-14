#ifndef AVEL_SUB_8U_HPP
#define AVEL_SUB_8U_HPP

namespace avel::benchmarks::sub_8u {

    //=====================================================
    // scalar 8u benchmarks
    //=====================================================

    std::uint8_t scalar_native_impl(std::uint8_t x, std::uint8_t y) {
        return x - y;
    }

    auto scalar_native = scalar_test_bench<std::uint8_t, scalar_native_impl>;

    BENCHMARK(sub_8u::scalar_native);

    //=====================================================
    // vec16x8u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec16x8u vec16x8u_subb_impl(vec16x8u v, vec16x8u w) {
        return vec16x8u{_mm_sub_epi8(decay(v), decay(w))};
    }

    auto vec16x8u_subb = vector_test_bench<vec16x8u, vec16x8u_subb_impl>;

    BENCHMARK(sub_8u::vec16x8u_subb);

    #endif

    //=====================================================
    // vec32x8u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec32x8u vec32x8u_subb_impl(vec32x8u v, vec32x8u w) {
        return vec32x8u{_mm256_sub_epi8(decay(v), decay(w))};
    }

    auto vec32x8u_subb = vector_test_bench<vec32x8u, vec32x8u_subb_impl>;

    BENCHMARK(sub_8u::vec32x8u_subb);

    #endif

    //=====================================================
    // vec64x8u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512BW)

    vec64x8u vec64x8u_subb_impl(vec64x8u v, vec64x8u w) {
        return vec64x8u{_mm512_sub_epi8(decay(v), decay(w))};
    }

    auto vec64x8u_subb = vector_test_bench<vec64x8u, vec64x8u_subb_impl>;

    BENCHMARK(sub_8u::vec64x8u_subb);

    #endif

}

#endif //AVEL_SUB_8U_HPP
