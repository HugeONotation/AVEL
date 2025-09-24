#ifndef AVEL_SHIFT_LEFT_VECTOR_8I_HPP
#define AVEL_SHIFT_LEFT_VECTOR_8I_HPP

namespace avel::benchmarks::shift_left_8u {

    //=====================================================
    // scalar f32 benchmarks
    //=====================================================

    std::uint8_t scalar_native_impl(std::uint8_t x, std::uint8_t s) {
        return std::uint16_t(x) << s;
    }

    auto scalar_native = scalar_test_bench<std::uint8_t, scalar_native_impl>;

    BENCHMARK(shift_left_8u::scalar_native);

    //=====================================================
    // vec16x8u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec16x8u vec16x8u_divide_and_conquer_impl(vec16x8u v, vec16x8u rhs) {
        auto content = decay(v);
        auto zeros = _mm_setzero_si128();

        auto a = _mm_cmpeq_epi8(_mm_and_si128(decay(rhs), _mm_set1_epi8(0x01)), zeros);
        content = _mm_add_epi8(content, _mm_andnot_si128(a, content));

        auto b = _mm_cmpeq_epi8(_mm_and_si128(decay(rhs), _mm_set1_epi8(0x02)), zeros);
        content = _mm_add_epi8(content, _mm_andnot_si128(b, content));
        content = _mm_add_epi8(content, _mm_andnot_si128(b, content));

        auto c = _mm_cmpeq_epi8(_mm_and_si128(decay(rhs), _mm_set1_epi8(0x04)), zeros);
        auto tmp = _mm_and_si128(_mm_slli_epi16(content, 4), _mm_set1_epi16(0xF0FF));
        content = _mm_or_si128(_mm_andnot_si128(c, tmp), _mm_and_si128(c, content));

        content = _mm_andnot_si128(_mm_cmpgt_epi8(decay(rhs), _mm_set1_epi8(0x7)), content);
        return vec16x8u{content};
    }

    auto vec16x8u_divide_and_conquer = vector_test_bench<vec16x8u, vec16x8u, vec16x8u_divide_and_conquer_impl>;

    BENCHMARK(shift_left_8u::vec16x8u_divide_and_conquer);

    #endif



    #if defined(AVEL_SSSE3)

    vec16x8u vec16x8u_pshufb_impl(vec16x8u v, vec16x8u rhs) {
        alignas(16) static constexpr std::uint8_t table_data[16] {
            0x01, 0x02, 0x04, 0x08,
            0x10, 0x20, 0x40, 0x80,
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00
        };

        auto table = _mm_load_si128(reinterpret_cast<const vec16x8u::primitive*>(table_data));
        auto shifts = _mm_shuffle_epi8(table, decay(rhs));

        v *= vec16x8u{shifts};
        return v;
    }

    auto vec16x8u_pshufb = vector_test_bench<vec16x8u, vec16x8u, vec16x8u_pshufb_impl>;

    BENCHMARK(shift_left_8u::vec16x8u_pshufb);

    #endif



    #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)

    vec16x8u vec16x8u_widening_impl(vec16x8u v, vec16x8u rhs) {
        auto content = decay(v);
        auto whole = _mm256_cvtepu8_epi16(content);
        auto shifts = _mm256_cvtepu8_epi16(decay(rhs));
        content = _mm256_cvtepi16_epi8(_mm256_sllv_epi16(whole, shifts));
        return vec16x8u{content};
    }

    auto vec16x8u_widening = vector_test_bench<vec16x8u, vec16x8u, vec16x8u_widening_impl>;

    BENCHMARK(shift_left_8u::vec16x8u_widening);

    #endif

    //=====================================================
    // vec32x8u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec32x8u vec32x8u_pshufb_impl(vec32x8u v, vec32x8u rhs) {
        alignas(32) static constexpr std::uint8_t table_data[32] {
            0x01, 0x02, 0x04, 0x08,
            0x10, 0x20, 0x40, 0x80,
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00,
            0x01, 0x02, 0x04, 0x08,
            0x10, 0x20, 0x40, 0x80,
            0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00
        };

        auto table = _mm256_load_si256(reinterpret_cast<const vec32x8u::primitive*>(table_data));
        auto shifts = _mm256_shuffle_epi8(table, decay(rhs));

        v *= vec32x8u{shifts};
        return v;
    }

    auto vec32x8u_pshufb = vector_test_bench<vec32x8u, vec32x8u, vec32x8u_pshufb_impl>;

    BENCHMARK(shift_left_8u::vec32x8u_pshufb);

    #endif



    #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)

    vec32x8u vec32x8u_widening_impl(vec32x8u v, vec32x8u rhs) {
        auto content = decay(v);
        auto whole = _mm512_cvtepu8_epi16(content);
        auto shifts = _mm512_cvtepu8_epi16(decay(rhs));
        content = _mm512_cvtepi16_epi8(_mm512_sllv_epi16(whole, shifts));
        return vec32x8u{content};
    }

    auto vec32x8u_widening = vector_test_bench<vec32x8u, vec32x8u, vec32x8u_widening_impl>;

    BENCHMARK(shift_left_8u::vec32x8u_widening);

    #endif

    //=====================================================
    // vec64x8u benchmarks
    //=====================================================

    #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)



    #endif

}

#endif //AVEL_SHIFT_LEFT_VECTOR_8I_HPP
