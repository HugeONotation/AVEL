#ifndef AVEL_REDUCE_ADD_8U_HPP
#define AVEL_REDUCE_ADD_8U_HPP

namespace avel::benchmarks::reduce_add_8u {

    /*
    //=====================================================
    // vec16x8u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    std::uint16_t vec16x8u_fully_vectorized_horizontal_fold_impl(vec16x8u v) {
        __m128i even_mask = _mm_set1_epi16(0x00fff);
        __m128i t0 = decay(v);
        __m128i t1 = _mm_and_si128(t0, even_mask);
        __m128i t2 = _mm_srli_epi16(t0, 8);
        __m128i t2 = _mm_add_epi16(t1, t2);
        __m128i t3 = _mm_srli_epi32(t2, 16);


        std::uint16_t result = _mm_extract_epi16();
        return ;
    }

    auto vec16x8u_horizontal_fold = vector_test_bench<vec16x8u, vec16x8u_horizontal_fold_impl>;

    BENCHMARK(reduce_add_8u::vec16x8u_horizontal_fold);

    #endif
    */

}

#endif //AVEL_REDUCE_ADD_8U_HPP
