#ifndef AVEL_MASK_FROM_BOOL_8I_HPP
#define AVEL_MASK_FROM_BOOL_8I_HPP



namespace avel::benchmarks::mask_from_bool_8i {

    //=====================================================
    // scalar 8i benchmarks
    //=====================================================

    std::int8_t scalar_negate_bool_impl(bool b) {
        return -std::int8_t(b);
    }

    auto scalar_negate_bool = scalar_mask_bool_test_bench<std::int8_t, scalar_negate_bool_impl>;

    BENCHMARK(mask_from_bool_8i::scalar_negate_bool);



    std::int8_t scalar_ternary_impl(bool b) {
        return b ? 0xff : 0;
    }

    auto scalar_ternary = scalar_mask_bool_test_bench<std::int8_t, scalar_ternary_impl>;

    BENCHMARK(mask_from_bool_8i::scalar_ternary);

    //=====================================================
    // mask16x8i benchmarks
    //=====================================================

    #if defined(AVEL_SSE2) && !defined(AVEL_AVX512VL)

    mask16x8i mask16x8i_ternary_impl(bool b) {
        return mask16x8i{b ? _mm_set1_epi64x(-1ull) : _mm_setzero_si128()};
    }

    auto mask16x8i_ternary = vector_mask_bool_test_bench<mask16x8i, mask16x8i_ternary_impl>;

    BENCHMARK(mask_from_bool_8i::mask16x8i_ternary);



    mask16x8i mask16x8i_set1_impl(bool b) {
        return mask16x8i{_mm_set1_epi64x(-b)};
    }

    auto mask16x8i_set1 = vector_mask_bool_test_bench<mask16x8i, mask16x8i_set1_impl>;

    BENCHMARK(mask_from_bool_8i::mask16x8i_set1);

    #endif



    #if defined(AVEL_AVX512BW) && defined(AVEL_AVX512VL)

    mask16x8i mask16x8i_negate_impl(bool b) {
        // This almost certainly doesn't move the negated result into a mask
        // register so this should really be reworked
        return mask16x8i{__mmask16(-std::int16_t(b))};
    }

    auto mask16x8i_negate = vector_mask_bool_test_bench<mask16x8i, mask16x8i_negate_impl>;

    BENCHMARK(mask_from_bool_8i::mask16x8i_negate);

    #endif

    //=====================================================
    // mask32x8i benchmarks
    //=====================================================

    #if defined(AVEL_AVX2) && !defined(AVEL_AVX512VL)

    mask32x8i mask32x8i_ternary_impl(bool b) {
        return mask32x8i{b ? _mm256_set1_epi64x(-1ull) : _mm256_setzero_si256()};
    }

    auto mask32x8i_ternary = vector_mask_bool_test_bench<mask32x8i, mask32x8i_ternary_impl>;

    BENCHMARK(mask_from_bool_8i::mask32x8i_ternary);



    mask32x8i mask32x8i_set1_impl(bool b) {
        return mask32x8i{_mm256_set1_epi64x(-b)};
    }

    auto mask32x8i_set1 = vector_mask_bool_test_bench<mask32x8i, mask32x8i_set1_impl>;

    BENCHMARK(mask_from_bool_8i::mask32x8i_set1);

    #endif



    #if defined(AVEL_AVX512BW) && defined(AVEL_AVX512VL)

    mask32x8i mask32x8i_negate_impl(bool b) {
        // This almost certainly doesn't move the negated result into a mask
        // register so this should really be reworked
        return mask32x8i{_cvtu32_mask32(-std::int32_t(b))};
    }

    auto mask32x8i_negate = vector_mask_bool_test_bench<mask32x8i, mask32x8i_negate_impl>;

    BENCHMARK(mask_from_bool_8i::mask32x8i_negate);

    #endif

    //=====================================================
    // mask64x8i benchmarks
    //=====================================================

    #if defined(AVEL_AVX512BW)

    mask64x8i mask64x8i_negate_impl(bool b) {
        // This almost certainly doesn't move the negated result into a mask
        // register so this should really be reworked
        return mask64x8i{_cvtu64_mask64(-std::int64_t(b))};
    }

    auto mask64x8i_negate = vector_mask_bool_test_bench<mask64x8i, mask64x8i_negate_impl>;

    BENCHMARK(mask_from_bool_8i::mask64x8i_negate);

    #endif

}

#endif //AVEL_MASK_FROM_BOOL_8I_HPP
