#ifndef AVEL_COMPRESS_16U_HPP
#define AVEL_COMPRESS_16U_HPP

namespace avel::benchmarks::compress_16u {

    //=====================================================
    // vec8x16u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW) && defined(AVEL_AVX512VBMI2)

    vec8x16u vec8x16u_vcompress_impl(vec8x16u x, mask8x16u m) {
        return vec8x16u{_mm_maskz_compress_epi16(decay(m), decay(x))};
    }

    auto vec8x16u_vcompress = vector_mask_test_bench<vec8x16u, vec8x16u_vcompress_impl>;

    BENCHMARK(compress_16u::vec8x16u_vcompress);

    #endif

    //=====================================================
    // vec16x16u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW) && defined(AVEL_AVX512VBMI2)

    vec16x16u vec16x16u_vcompress_impl(vec16x16u x, mask16x16u m) {
        return vec16x16u{_mm256_maskz_compress_epi16(decay(m), decay(x))};
    }

    auto vec16x16u_vcompress = vector_mask_test_bench<vec16x16u, vec16x16u_vcompress_impl>;

    BENCHMARK(compress_16u::vec16x16u_vcompress);

    #endif

    //=====================================================
    // vec32x16u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512BW) && defined(AVEL_AVX512VBMI2)

    vec32x16u vec32x16u_vcompress_impl(vec32x16u x, mask32x16u m) {
        return vec32x16u{_mm512_maskz_compress_epi16(decay(m), decay(x))};
    }

    auto vec32x16u_vcompress = vector_mask_test_bench<vec32x16u, vec32x16u_vcompress_impl>;

    BENCHMARK(compress_16u::vec32x16u_vcompress);

    #endif

}

#endif //AVEL_COMPRESS_16U_HPP
