#ifndef AVEL_COMPRESS_32U_HPP
#define AVEL_COMPRESS_32U_HPP

namespace avel::benchmarks::compress_32u {

    //=====================================================
    // vec4x32u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512BW) && defined(AVEL_AVX512VL)

    vec4x32u vec4x32u_vcompress_impl(vec4x32u x, mask4x32u m) {
        return vec4x32u{_mm_maskz_compress_epi32(decay(m), decay(x))};
    }

    auto vec4x32u_vcompress = vector_mask_test_bench<vec4x32u, vec4x32u_vcompress_impl>;

    BENCHMARK(compress_32u::vec4x32u_vcompress);

    #endif

    //=====================================================
    // vec8x32u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512BW) && defined(AVEL_AVX512VL)

    vec8x32u vec8x32u_vcompress_impl(vec8x32u x, mask8x32u m) {
        return vec8x32u{_mm256_maskz_compress_epi32(decay(m), decay(x))};
    }

    auto vec8x32u_vcompress = vector_mask_test_bench<vec8x32u, vec8x32u_vcompress_impl>;

    BENCHMARK(compress_32u::vec8x32u_vcompress);

    #endif

    //=====================================================
    // vec16x32u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512BW)

    vec16x32u vec16x32u_vcompress_impl(vec16x32u x, mask16x32u m) {
        return vec16x32u{_mm512_maskz_compress_epi32(decay(m), decay(x))};
    }

    auto vec16x32u_vcompress = vector_mask_test_bench<vec16x32u, vec16x32u_vcompress_impl>;

    BENCHMARK(compress_32u::vec16x32u_vcompress);

    #endif

}

#endif //AVEL_COMPRESS_32U_HPP
