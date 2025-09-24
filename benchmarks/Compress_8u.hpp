#ifndef AVEL_COMPRESS_8U_HPP
#define AVEL_COMPRESS_8U_HPP

namespace avel::benchmarks::compress_8u {

    //=====================================================
    // vec16x8u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW) && defined(AVEL_AVX512VBMI2)

    vec16x8u vec16x8u_vcompress_impl(vec16x8u x, mask16x8u m) {
        return vec16x8u{_mm_maskz_compress_epi8(decay(m), decay(x))};
    }

    auto vec16x8u_vcompress = vector_mask_test_bench<vec16x8u, vec16x8u_vcompress_impl>;

    BENCHMARK(compress_8u::vec16x8u_vcompress);

    #endif

    //=====================================================
    // vec32x8u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW) && defined(AVEL_AVX512VBMI2)

    vec32x8u vec32x8u_vcompress_impl(vec32x8u x, mask32x8u m) {
        return vec32x8u{_mm256_maskz_compress_epi8(decay(m), decay(x))};
    }

    auto vec32x8u_vcompress = vector_mask_test_bench<vec32x8u, vec32x8u_vcompress_impl>;

    BENCHMARK(compress_8u::vec32x8u_vcompress);

    #endif

    //=====================================================
    // vec64x8u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512BW) && defined(AVEL_AVX512VBMI2)

    vec64x8u vec64x8u_vcompress_impl(vec64x8u x, mask64x8u m) {
        return vec64x8u{_mm512_maskz_compress_epi8(decay(m), decay(x))};
    }

    auto vec64x8u_vcompress = vector_mask_test_bench<vec64x8u, vec64x8u_vcompress_impl>;

    BENCHMARK(compress_8u::vec64x8u_vcompress);

    #endif

}

#endif //AVEL_COMPRESS_8U_HPP
