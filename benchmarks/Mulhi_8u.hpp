#ifndef AVEL_MULHI_UINT8_HPP
#define AVEL_MULHI_UINT8_HPP

namespace avel::benchmarks::mulhi_8u {

    //=====================================================
    // scalar 8u benchmarks
    //=====================================================

    std::uint8_t native_implementation(std::uint8_t x, std::uint8_t y) {
        return (x * y) >> 8;
    }

    auto native = scalar_test_bench<std::uint8_t, native_implementation>;

    BENCHMARK(mulhi_8u::native);

    //=====================================================
    // vec16x8u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec16x8u vec16x8u_shuffle_impl(vec16x8u x, vec16x8u y) {
        auto zeros = _mm_setzero_si128();

        auto x_lo = _mm_unpacklo_epi8(decay(x), zeros);
        auto x_hi = _mm_unpackhi_epi8(decay(x), zeros);

        auto y_lo = _mm_unpacklo_epi8(decay(y), zeros);
        auto y_hi = _mm_unpackhi_epi8(decay(y), zeros);

        auto p_lo = _mm_mullo_epi16(x_lo, y_lo);
        auto p_hi = _mm_mullo_epi16(x_hi, y_hi);

        auto ret_lo = _mm_srli_epi16(p_lo, 8);
        auto ret_hi = _mm_srli_epi16(p_hi, 8);

        auto ret = _mm_packus_epi16(ret_lo, ret_hi);
        return vec16x8u{ret};
    }

    auto vec16x8u_shuffle = vector_test_bench<vec16x8u, vec16x8u_shuffle_impl>;

    BENCHMARK(mulhi_8u::vec16x8u_shuffle);

    #endif



    #if defined(AVEL_SSE2)

    vec16x8u vec16x8u_masking_impl(vec16x8u x, vec16x8u y) {
        auto byte_mask = _mm_set1_epi16(0x00FF);

        auto x_even = _mm_and_si128(byte_mask, decay(x));
        auto x_odd  = _mm_srli_epi16(decay(x), 8);

        auto y_even = _mm_and_si128(byte_mask, decay(y));
        auto y_odd  = _mm_srli_epi16(decay(y), 8);

        auto p_even = _mm_mullo_epi16(x_even, y_even);
        auto p_odd  = _mm_mullo_epi16(x_odd,  y_odd );

        auto ret_even = _mm_srli_epi16(p_even, 8);
        auto ret_odd  = _mm_andnot_si128(byte_mask, p_odd);

        auto ret = _mm_or_si128(ret_even, ret_odd);
        return vec16x8u{ret};
    }

    auto vec16x8u_masking = vector_test_bench<vec16x8u, vec16x8u_masking_impl>;

    BENCHMARK(mulhi_8u::vec16x8u_masking);

    #endif

    //=====================================================
    // vec32x8u benchmarks
    //=====================================================



    //=====================================================
    // vec64x8u benchmarks
    //=====================================================



}





#if defined(AVEL_AVX2)

namespace avel::benchmarks::mulhi_vec32x8u {

    vec32x8u shuffle_implementation(vec32x8u x, vec32x8u y) {
        auto zeros = _mm256_setzero_si256();

        auto x_lo = _mm256_unpacklo_epi8(decay(x), zeros);
        auto x_hi = _mm256_unpackhi_epi8(decay(x), zeros);

        auto y_lo = _mm256_unpacklo_epi8(decay(y), zeros);
        auto y_hi = _mm256_unpackhi_epi8(decay(y), zeros);

        auto p_lo = _mm256_mullo_epi16(x_lo, y_lo);
        auto p_hi = _mm256_mullo_epi16(x_hi, y_hi);

        auto ret_lo = _mm256_srli_epi16(p_lo, 8);
        auto ret_hi = _mm256_srli_epi16(p_hi, 8);

        auto ret = _mm256_packus_epi16(ret_lo, ret_hi);
        return vec32x8u{ret};
    }

    vec32x8u masking_implementation(vec32x8u x, vec32x8u y) {
        auto byte_mask = _mm256_set1_epi16(0x00FF);

        auto x_even = _mm256_and_si256(byte_mask, decay(x));
        auto x_odd  = _mm256_srli_epi16(decay(x), 8);

        auto y_even = _mm256_and_si256(byte_mask, decay(y));
        auto y_odd  = _mm256_srli_epi16(decay(y), 8);

        auto p_even = _mm256_mullo_epi16(x_even, y_even);
        auto p_odd  = _mm256_mullo_epi16(x_odd,  y_odd );

        auto ret_even = _mm256_srli_epi16(p_even, 8);
        auto ret_odd  = _mm256_andnot_si256(byte_mask, p_odd);

        auto ret = _mm256_or_si256(ret_even, ret_odd);
        return vec32x8u{ret};
    }

    auto shuffling = vector_test_bench<vec32x8u, shuffle_implementation>;
    auto masking = vector_test_bench<vec32x8u, masking_implementation>;

    BENCHMARK(mulhi_vec32x8u::shuffling);
    BENCHMARK(mulhi_vec32x8u::masking);

}

#endif





#if defined(AVEL_AVX512BW)

namespace avel::benchmarks::mulhi_vec64x8u {

    vec64x8u shuffle_implementation(vec64x8u x, vec64x8u y) {
        auto zeros = _mm512_setzero_si512();

        auto x_lo = _mm512_unpacklo_epi8(decay(x), zeros);
        auto x_hi = _mm512_unpackhi_epi8(decay(x), zeros);

        auto y_lo = _mm512_unpacklo_epi8(decay(y), zeros);
        auto y_hi = _mm512_unpackhi_epi8(decay(y), zeros);

        auto p_lo = _mm512_mullo_epi16(x_lo, y_lo);
        auto p_hi = _mm512_mullo_epi16(x_hi, y_hi);

        auto ret_lo = _mm512_srli_epi16(p_lo, 8);
        auto ret_hi = _mm512_srli_epi16(p_hi, 8);

        auto ret = _mm512_packus_epi16(ret_lo, ret_hi);
        return vec64x8u{ret};
    }

    vec64x8u masking_implementation(vec64x8u x, vec64x8u y) {
        auto byte_mask = _mm512_set1_epi16(0x00FF);

        auto x_even = _mm512_and_si512(byte_mask, decay(x));
        auto x_odd  = _mm512_srli_epi16(decay(x), 8);

        auto y_even = _mm512_and_si512(byte_mask, decay(y));
        auto y_odd  = _mm512_srli_epi16(decay(y), 8);

        auto p_even = _mm512_mullo_epi16(x_even, y_even);
        auto p_odd  = _mm512_mullo_epi16(x_odd,  y_odd );

        auto ret_even = _mm512_srli_epi16(p_even, 8);
        auto ret_odd  = _mm512_andnot_si512(byte_mask, p_odd);

        auto ret = _mm512_or_si512(ret_even, ret_odd);
        return vec64x8u{ret};
    }

    auto shuffling = vector_test_bench<vec64x8u, shuffle_implementation>;
    auto masking = vector_test_bench<vec64x8u, masking_implementation>;

    BENCHMARK(mulhi_vec64x8u::shuffling);
    BENCHMARK(mulhi_vec64x8u::masking);

}

#endif

#endif //AVEL_MULHI_UINT8_HPP
