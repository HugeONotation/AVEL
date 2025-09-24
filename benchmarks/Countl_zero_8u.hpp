#ifndef AVEL_COUNTL_ZERO_8U_HPP
#define AVEL_COUNTL_ZERO_8U_HPP

namespace avel::benchmarks::countl_zero_8u {

    //=====================================================
    // scalar 8u benchmarks
    //=====================================================

    #if __cplusplus >= 202002L

    std::uint8_t scalar_native_impl(std::uint8_t x) {
        return std::countl_zero(x);
    }

    auto scalar_native = scalar_test_bench<std::uint8_t, scalar_native_impl>;

    BENCHMARK(countl_zero_8u::scalar_native);

    #endif



    std::uint8_t scalar_bitwise_accumulate_impl(std::uint8_t x) {
        std::uint8_t ret = 0;

        std::int8_t tmp = x;
        for (int i = 0; i < 8; ++i) {
            ret += (tmp < 0);
            tmp <<= 1;
        }

        return ret;
    }

    auto scalar_bitwise_accumulate = scalar_test_bench<std::uint8_t, scalar_bitwise_accumulate_impl>;

    BENCHMARK(countl_zero_8u::scalar_bitwise_accumulate);



    std::uint8_t scalar_nibble_lookup_impl(std::uint8_t x) {
        alignas(16) static constexpr std::uint8_t table0[16] {
            8, 3, 2, 2,
            1, 1, 1, 1,
            0, 0, 0, 0,
            0, 0, 0, 0
        };

        alignas(16) static constexpr std::uint8_t table1[16] {
            8, 7, 6, 6,
            5, 5, 5, 5,
            4, 4, 4, 4,
            4, 4, 4, 4
        };

        auto lo_nibble = x & 0x0F;
        auto hi_nibble = x >> 4;

        auto lo = table0[hi_nibble];
        auto hi = table1[lo_nibble];

        if (lo < hi) {
            return lo;
        } else {
            return hi;
        }
    }

    auto scalar_nibble_lookup = scalar_test_bench<std::uint8_t, scalar_nibble_lookup_impl>;

    BENCHMARK(countl_zero_8u::scalar_nibble_lookup);



    std::uint8_t scalar_byte_lookup_impl(std::uint8_t x) {
        alignas(256) static constexpr std::uint8_t table[256] {
            32, 31, 30, 30, 29, 29, 29, 29, 28, 28, 28, 28, 28, 28, 28, 28,
            27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27,
            26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
            26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
            25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
            25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
            25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
            25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
            24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
            24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
            24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
            24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
            24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
            24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
            24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
            24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24
        };

        return table[x];
    }

    auto scalar_byte_lookup = scalar_test_bench<std::uint8_t, scalar_byte_lookup_impl>;

    BENCHMARK(countl_zero_8u::scalar_byte_lookup);



    std::uint8_t scalar_float_conversion_impl(std::uint8_t x) {
        float y = x + 0.5f;
        std::uint32_t bits = avel::bit_cast<std::uint32_t>(y);
        return 134 - (bits >> 23);
    }

    auto scalar_float_conversion = scalar_test_bench<std::uint8_t, scalar_float_conversion_impl>;

    BENCHMARK(countl_zero_8u::scalar_float_conversion);



    #if defined(AVEL_X86) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))

    std::uint8_t scalar_bsr_impl(std::uint8_t x) {
        if (x) {
            return 7 - _bit_scan_reverse(x);
        } else {
            return 8;
        }
    }

    auto scalar_bsr = scalar_test_bench<std::uint8_t, scalar_bsr_impl>;

    BENCHMARK(countl_zero_8u::scalar_bsr);

    #endif



    #if defined(AVEL_LZCNT) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))

    std::uint8_t scalar_lzcnt_impl(std::uint8_t x) {
        return static_cast<uint8_t>(_lzcnt_u32(x) - 24);
    }

    auto scalar_lzcnt = scalar_test_bench<std::uint8_t, scalar_lzcnt_impl>;

    BENCHMARK(countl_zero_8u::scalar_lzcnt);

    #endif

    //=====================================================
    // vec16x8u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec16x8u vec16x8u_bitwise_divide_and_conquer_impl(vec16x8u v) {
        auto ret = _mm_setzero_si128();

        auto threshold0 = _mm_set1_epi8(0x08);
        auto mask0 = _mm_cmplt_epi8(decay(v), threshold0);
        ret = _mm_sub_epi8(ret, mask0);
        ret = _mm_add_epi8(ret, ret);

        auto threshold1_0 = _mm_set1_epi8(0x02);
        auto threshold1_1 = _mm_set1_epi8(0x20);

        auto threshold1 = _mm_max_epu8(threshold1_0, _mm_andnot_si128(mask0, threshold1_1));
        auto mask1 = _mm_cmplt_epi8(decay(v), threshold1);
        ret = _mm_sub_epi8(ret, mask1);
        ret = _mm_add_epi8(ret, ret);

        auto threshold2_0 = _mm_srli_epi16(threshold1, 1);
        auto threshold2_1 = _mm_add_epi8(threshold1, threshold1);
        auto threshold2 = _mm_max_epu8(threshold2_0, _mm_andnot_si128(mask1, threshold2_1));
        auto mask2 = _mm_cmplt_epi8(decay(v), threshold2);
        ret = _mm_sub_epi8(ret, mask2);

        ret = _mm_sub_epi8(ret, _mm_set1_epi8(-1));
        ret = _mm_andnot_si128(_mm_cmplt_epi8(decay(v), _mm_setzero_si128()), ret);
        return vec16x8u{ret};
    }

    auto vec16x8u_bitwise_divide_and_conquer = vector_test_bench<vec16x8u, vec16x8u_bitwise_divide_and_conquer_impl>;

    BENCHMARK(countl_zero_8u::vec16x8u_bitwise_divide_and_conquer);

    #endif



    #if defined(AVEL_SSSE3)

    vec16x8u vec16x8u_pshufb_impl(vec16x8u v) {
        alignas(16) static constexpr std::uint8_t table_data0[16] {
            8, 3, 2, 2,
            1, 1, 1, 1,
            0, 0, 0, 0,
            0, 0, 0, 0
        };

        alignas(16) static constexpr std::uint8_t table_data1[16] {
            8, 7, 6, 6,
            5, 5, 5, 5,
            4, 4, 4, 4,
            4, 4, 4, 4
        };

        auto nibble_mask = _mm_set1_epi8(0x0F);
        auto lo_nibble = _mm_and_si128(nibble_mask, decay(v));
        auto hi_nibble = _mm_and_si128(nibble_mask, _mm_srli_epi16(decay(v), 0x4));

        auto table0 = _mm_load_si128(reinterpret_cast<const __m128i*>(table_data0));
        auto table1 = _mm_load_si128(reinterpret_cast<const __m128i*>(table_data1));

        auto partial0 = _mm_shuffle_epi8(table1, lo_nibble);
        auto partial1 = _mm_shuffle_epi8(table0, hi_nibble);

        auto ret = _mm_min_epu8(partial0, partial1);
        return vec16x8u{ret};
    }

    auto vec16x8u_pshufb = vector_test_bench<vec16x8u, vec16x8u_pshufb_impl>;

    BENCHMARK(countl_zero_8u::vec16x8u_pshufb);

    #endif

    //=====================================================
    // vec32x8u benchmarks
    //=====================================================



    //=====================================================
    // vec64x8u benchmarks
    //=====================================================

    #if defined(AVEL_AVX512BW) && defined(AVEL_AVX512VBMI)

    vec64x8u vec64x8u_vpermi2b_impl(vec64x8u v) {
        alignas(128) static constexpr std::uint8_t table[128] {
            8, 7, 6, 6, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4,
            3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
            2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
            2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
        };

        auto table0 = _mm512_load_si512(table +  0);
        auto table1 = _mm512_load_si512(table + 64);

        auto partial_result = _mm512_permutex2var_epi8(table0, decay(v), table1);

        auto is_high_bit_set_mask = _knot_mask64(_mm512_movepi8_mask(decay(v)));
        auto ret = _mm512_maskz_mov_epi8(is_high_bit_set_mask, partial_result);

        return vec64x8u{ret};
    }

    auto vec64x8u_vpermi2b = vector_test_bench<vec64x8u, vec64x8u_vpermi2b_impl>;

    BENCHMARK(countl_zero_8u::vec64x8u_vpermi2b);

    #endif

}

#endif //AVEL_COUNTL_ZERO_8U_HPP
