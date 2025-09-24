#ifndef AVEL_EXCLUSIVE_PREFIX_SUM_8U_HPP
#define AVEL_EXCLUSIVE_PREFIX_SUM_8U_HPP

namespace avel::benchmarks::exclusive_prefix_sum8u {

    /*
    //=====================================================
    // vec16x8u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2) && !defined(AVEL_AVX512BW) && !defined(AVEL_AVX512VL)

    vec16x8u vec16x8u_exclusive_prefix_sum_movemask_impl(mask16x8u m) {
        std::uint32_t mask = _mm_movemask_epi8(decay(m));
        std::uint8_t mask_lo = mask;
        std::uint8_t mask_hi = (mask >> 8);

        __m128i mask_lo_vector = _mm_set1_epi8(mask_lo);
        __m128i mask_hi_vector = _mm_set1_epi8(mask_hi);

        __m128i ret = _mm_setzero_si128();
        __m182i a;

        //TODO: Complete implementation

        return vec16x8u{ret};
    }

    auto vec16x8u_vcompress = mask_test_bench<mask16x8u, vec16x8u_exclusive_prefix_sum_movemask_impl>;

    BENCHMARK(exclusive_prefix_sum8u::vec16x8u_vcompress);

    #endif



    #if defined(AVEL_SSE2) && !defined(AVEL_AVX512BW) && !defined(AVEL_AVX512VL)

    vec16x8u vec16x8u_exclusive_prefix_sum_divide_and_conquer_impl(mask16x8u m) {
        __m128i a = ;
        __m128k b = ;

        __m128i ret = _mm_setzero_si128();
        return vec16x8u{ret};
    }

    auto vec16x8u_exclusive_prefix_sum_divide_and_conquer = mask_test_bench<mask16x8u, vec16x8u_exclusive_prefix_sum_divide_and_conquer_impl>;

    BENCHMARK(exclusive_prefix_sum8u::vec16x8u_exclusive_prefix_sum_divide_and_conquer);

    #endif

    //=====================================================
    // vec32x8u benchmarks
    //=====================================================

    //=====================================================
    // vec64x8u benchmarks
    //=====================================================
    */

}

#endif //AVEL_EXCLUSIVE_PREFIX_SUM_8U_HPP
