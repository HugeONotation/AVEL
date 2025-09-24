#ifndef AVEL_COMPRESS_64U_HPP
#define AVEL_COMPRESS_64U_HPP

namespace avel::benchmarks::compress_64u {

    //=====================================================
    // vec2x64u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512BW) && defined(AVEL_AVX512VL)

    vec2x64u vec2x64u_vcompress_impl(vec2x64u x, mask2x64u m) {
        return vec2x64u{_mm_maskz_compress_epi64(decay(m), decay(x))};
    }

    auto vec2x64u_vcompress = vector_mask_test_bench<vec2x64u, vec2x64u_vcompress_impl>;

    BENCHMARK(compress_64u::vec2x64u_vcompress);

    #endif

    //=====================================================
    // vec4x64u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512BW) && defined(AVEL_AVX512VL)

    vec4x64u vec4x64u_vcompress_impl(vec4x64u x, mask4x64u m) {
        return vec4x64u{_mm256_maskz_compress_epi64(decay(m), decay(x))};
    }

    auto vec4x64u_vcompress = vector_mask_test_bench<vec4x64u, vec4x64u_vcompress_impl>;

    BENCHMARK(compress_64u::vec4x64u_vcompress);

    #endif

    //=====================================================
    // vec8x64u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512BW)

    vec8x64u vec8x64u_vcompress_impl(vec8x64u x, mask8x64u m) {
        return vec8x64u{_mm512_maskz_compress_epi64(decay(m), decay(x))};
    }

    auto vec8x64u_vcompress = vector_mask_test_bench<vec8x64u, vec8x64u_vcompress_impl>;

    BENCHMARK(compress_64u::vec8x64u_vcompress);

    #endif

}

#endif //AVEL_COMPRESS_64U_HPP
