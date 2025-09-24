#ifndef AVEL_DIVISION_UINT8_HPP
#define AVEL_DIVISION_UINT8_HPP

namespace avel::benchmarks::div_8u {

    //=====================================================
    // scalar 8u benchmarks
    //=====================================================

    std::uint8_t scalar_native_impl(std::uint8_t x, std::uint8_t y) {
        return x / y;
    }

    auto scalar_native = scalar_division_test_bench<std::uint8_t, scalar_native_impl>;

    BENCHMARK(div_8u::scalar_native);



    std::uint8_t scalar_recip_lut_impl(std::uint8_t x, std::uint8_t y) {
        alignas(256) static constexpr std::uint32_t reciprocals[256] {
            0x0001, 0x10000, 0x8000, 0x5556, 0x4000, 0x3334, 0x2aab, 0x2493, 0x2000, 0x1c72, 0x199a, 0x1746, 0x1556, 0x13b2, 0x124a, 0x1112,
            0x1000, 0x00f10, 0x0e39, 0x0d7a, 0x0ccd, 0x0c31, 0x0ba3, 0x0b22, 0x0aab, 0x0a3e, 0x09d9, 0x097c, 0x0925, 0x08d4, 0x0889, 0x0843,
            0x0800, 0x007c2, 0x0788, 0x0751, 0x071d, 0x06ec, 0x06bd, 0x0691, 0x0667, 0x063f, 0x0619, 0x05f5, 0x05d2, 0x05b1, 0x0591, 0x0573,
            0x0556, 0x0053a, 0x051f, 0x0506, 0x04ed, 0x04d5, 0x04be, 0x04a8, 0x0493, 0x047e, 0x046a, 0x0457, 0x0445, 0x0433, 0x0422, 0x0411,
            0x0400, 0x003f1, 0x03e1, 0x03d3, 0x03c4, 0x03b6, 0x03a9, 0x039c, 0x038f, 0x0382, 0x0376, 0x036a, 0x035f, 0x0354, 0x0349, 0x033e,
            0x0334, 0x0032a, 0x0320, 0x0316, 0x030d, 0x0304, 0x02fb, 0x02f2, 0x02e9, 0x02e1, 0x02d9, 0x02d1, 0x02c9, 0x02c1, 0x02ba, 0x02b2,
            0x02ab, 0x002a4, 0x029d, 0x0296, 0x0290, 0x0289, 0x0283, 0x027d, 0x0277, 0x0271, 0x026b, 0x0265, 0x025f, 0x025a, 0x0254, 0x024f,
            0x024a, 0x00244, 0x023f, 0x023a, 0x0235, 0x0231, 0x022c, 0x0227, 0x0223, 0x021e, 0x021a, 0x0215, 0x0211, 0x020d, 0x0209, 0x0205,
            0x0200, 0x001fd, 0x01f9, 0x01f5, 0x01f1, 0x01ed, 0x01ea, 0x01e6, 0x01e2, 0x01df, 0x01db, 0x01d8, 0x01d5, 0x01d1, 0x01ce, 0x01cb,
            0x01c8, 0x001c4, 0x01c1, 0x01be, 0x01bb, 0x01b8, 0x01b5, 0x01b3, 0x01b0, 0x01ad, 0x01aa, 0x01a7, 0x01a5, 0x01a2, 0x019f, 0x019d,
            0x019a, 0x00198, 0x0195, 0x0193, 0x0190, 0x018e, 0x018b, 0x0189, 0x0187, 0x0184, 0x0182, 0x0180, 0x017e, 0x017b, 0x0179, 0x0177,
            0x0175, 0x00173, 0x0171, 0x016f, 0x016d, 0x016b, 0x0169, 0x0167, 0x0165, 0x0163, 0x0161, 0x015f, 0x015d, 0x015b, 0x0159, 0x0158,
            0x0156, 0x00154, 0x0152, 0x0151, 0x014f, 0x014d, 0x014b, 0x014a, 0x0148, 0x0147, 0x0145, 0x0143, 0x0142, 0x0140, 0x013f, 0x013d,
            0x013c, 0x0013a, 0x0139, 0x0137, 0x0136, 0x0134, 0x0133, 0x0131, 0x0130, 0x012f, 0x012d, 0x012c, 0x012a, 0x0129, 0x0128, 0x0126,
            0x0125, 0x00124, 0x0122, 0x0121, 0x0120, 0x011f, 0x011d, 0x011c, 0x011b, 0x011a, 0x0119, 0x0117, 0x0116, 0x0115, 0x0114, 0x0113,
            0x0112, 0x00110, 0x010f, 0x010e, 0x010d, 0x010c, 0x010b, 0x010a, 0x0109, 0x0108, 0x0107, 0x0106, 0x0105, 0x0104, 0x0103, 0x0102
        };

        return (x * reciprocals[y]) >> 16;
    }

    auto scalar_recip_lut = scalar_division_test_bench<std::uint8_t, scalar_recip_lut_impl>;

    BENCHMARK(div_8u::scalar_recip_lut);



    std::uint8_t scalar_f32_div_impl(std::uint8_t x, std::uint8_t y) {
        return std::uint8_t(float(x) / float(y));
    }

    auto scalar_f32_div = scalar_division_test_bench<std::uint8_t, scalar_f32_div_impl>;

    BENCHMARK(div_8u::scalar_f32_div);

    //=====================================================
    // vec16x8u benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec16x8u vec16x8u_generic_long_division_loop_impl(vec16x8u x, vec16x8u y) {
        // The lack of 8-bit shift instructions and native comparison operators means this can theoretically be made
        // better with careful consideration
        vec16x8u quotient{};

        for (std::int32_t i = 8; (i-- > 0) && any(x);) {
            mask16x8u b = ((x >> i) >= y);
            x -= keep(b, y << i);
            quotient |= (vec16x8u{b} << i);
        }

        return quotient;
    }

    auto vec16x8u_generic_long_division_loop = vector_division_test_bench<vec16x8u, vec16x8u_generic_long_division_loop_impl>;

    BENCHMARK(div_8u::vec16x8u_generic_long_division_loop);

    #endif



    #if defined(AVEL_SSE2)

    vec16x8u vec16x8u_long_division_unrolled_impl(vec16x8u x, vec16x8u y) {
        __m128i x_reg = decay(x);
        __m128i y_reg = decay(y);

        __m128i undef = _mm_undefined_si128();
        __m128i bit_mask = _mm_cmpeq_epi8(undef, undef);
        __m128i q_reg = _mm_setzero_si128();

        {   // Iteration 0
            bit_mask = _mm_add_epi8(bit_mask, bit_mask);

            __m128i x_shifted = _mm_andnot_si128(bit_mask, _mm_srli_epi16(x_reg, 7));
            __m128i y_shifted = _mm_slli_epi16(_mm_andnot_si128(bit_mask, y_reg), 7);

            __m128i mask = _mm_cmpeq_epi8(_mm_max_epu8(x_shifted, y_reg), x_shifted);
            x_reg = _mm_sub_epi8(x_reg, _mm_and_si128(mask, y_shifted));
            q_reg = _mm_add_epi8(q_reg, q_reg);
            q_reg = _mm_sub_epi8(q_reg, mask);
        }

        {   // Iteration 1
            bit_mask = _mm_add_epi8(bit_mask, bit_mask);

            __m128i x_shifted = _mm_andnot_si128(bit_mask, _mm_srli_epi16(x_reg, 6));
            __m128i y_shifted = _mm_slli_epi16(_mm_andnot_si128(bit_mask, y_reg), 6);

            __m128i mask = _mm_cmpeq_epi8(_mm_max_epu8(x_shifted, y_reg), x_shifted);
            x_reg = _mm_sub_epi8(x_reg, _mm_and_si128(mask, y_shifted));
            q_reg = _mm_add_epi8(q_reg, q_reg);
            q_reg = _mm_sub_epi8(q_reg, mask);
        }

        {   // Iteration 2
            bit_mask = _mm_add_epi8(bit_mask, bit_mask);

            __m128i x_shifted = _mm_andnot_si128(bit_mask, _mm_srli_epi16(x_reg, 5));
            __m128i y_shifted = _mm_slli_epi16(_mm_andnot_si128(bit_mask, y_reg), 5);

            __m128i mask = _mm_cmpeq_epi8(_mm_max_epu8(x_shifted, y_reg), x_shifted);
            x_reg = _mm_sub_epi8(x_reg, _mm_and_si128(mask, y_shifted));
            q_reg = _mm_add_epi8(q_reg, q_reg);
            q_reg = _mm_sub_epi8(q_reg, mask);
        }

        {   // Iteration 3
            bit_mask = _mm_add_epi8(bit_mask, bit_mask);

            __m128i x_shifted = _mm_andnot_si128(bit_mask, _mm_srli_epi16(x_reg, 4));
            __m128i y_shifted = _mm_slli_epi16(_mm_andnot_si128(bit_mask, y_reg), 4);

            __m128i mask = _mm_cmpeq_epi8(_mm_max_epu8(x_shifted, y_reg), x_shifted);
            x_reg = _mm_sub_epi8(x_reg, _mm_and_si128(mask, y_shifted));
            q_reg = _mm_add_epi8(q_reg, q_reg);
            q_reg = _mm_sub_epi8(q_reg, mask);
        }

        {   // Iteration 4
            bit_mask = _mm_add_epi8(bit_mask, bit_mask);

            __m128i x_shifted = _mm_andnot_si128(bit_mask, _mm_srli_epi16(x_reg, 3));
            __m128i y_shifted = _mm_slli_epi16(_mm_andnot_si128(bit_mask, y_reg), 3);

            __m128i mask = _mm_cmpeq_epi8(_mm_max_epu8(x_shifted, y_reg), x_shifted);
            x_reg = _mm_sub_epi8(x_reg, _mm_and_si128(mask, y_shifted));
            q_reg = _mm_add_epi8(q_reg, q_reg);
            q_reg = _mm_sub_epi8(q_reg, mask);
        }

        {   // Iteration 5
            bit_mask = _mm_add_epi8(bit_mask, bit_mask);

            __m128i x_shifted = _mm_andnot_si128(bit_mask, _mm_srli_epi16(x_reg, 2));
            __m128i y_shifted = _mm_add_epi8(y_reg, y_reg);
            y_shifted = _mm_add_epi8(y_shifted, y_shifted);

            __m128i mask = _mm_cmpeq_epi8(_mm_max_epu8(x_shifted, y_reg), x_shifted);
            x_reg = _mm_sub_epi8(x_reg, _mm_and_si128(mask, y_shifted));
            q_reg = _mm_add_epi8(q_reg, q_reg);
            q_reg = _mm_sub_epi8(q_reg, mask);
        }

        {   // Iteration 6
            bit_mask = _mm_add_epi8(bit_mask, bit_mask);

            __m128i x_shifted = _mm_andnot_si128(bit_mask, _mm_srli_epi16(x_reg, 1));
            __m128i y_shifted = _mm_add_epi8(y_reg, y_reg);

            __m128i mask = _mm_cmpeq_epi8(_mm_max_epu8(x_shifted, y_reg), x_shifted);
            x_reg = _mm_sub_epi8(x_reg, _mm_and_si128(mask, y_shifted));
            q_reg = _mm_add_epi8(q_reg, q_reg);
            q_reg = _mm_sub_epi8(q_reg, mask);
        }

        {   // Iteration 7
            __m128i x_shifted = x_reg;
            //__m128i y_shifted = y_reg;

            __m128i mask = _mm_cmpeq_epi8(_mm_max_epu8(x_shifted, y_reg), x_shifted);
            //x_reg = _mm_sub_epi8(x_reg, _mm_and_si128(mask, y_shifted));
            q_reg = _mm_add_epi8(q_reg, q_reg);
            q_reg = _mm_sub_epi8(q_reg, mask);
        }

        return vec16x8u{q_reg};
    }

    auto vec16x8u_long_division_unrolled = vector_division_test_bench<vec16x8u, vec16x8u_long_division_unrolled_impl>;

    BENCHMARK(div_8u::vec16x8u_long_division_unrolled);

    #endif



    #if defined(AVEL_SSE2)

    vec16x8u vec16x8u_32f_div_subnormal_unpacking_impl(vec16x8u x, vec16x8u y) {
        __m128i x_raw = decay(x);

        __m128i x_lo = _mm_unpacklo_epi8(x_raw, _mm_setzero_si128());
        __m128i x_hi = _mm_unpackhi_epi8(x_raw, _mm_setzero_si128());

        __m128i x0 = _mm_unpacklo_epi16(x_lo, _mm_setzero_si128());
        __m128i x1 = _mm_unpackhi_epi16(x_lo, _mm_setzero_si128());
        __m128i x2 = _mm_unpacklo_epi16(x_hi, _mm_setzero_si128());
        __m128i x3 = _mm_unpackhi_epi16(x_hi, _mm_setzero_si128());

        __m128i y_raw = decay(y);

        __m128i y_lo = _mm_unpacklo_epi8(y_raw, _mm_setzero_si128());
        __m128i y_hi = _mm_unpackhi_epi8(y_raw, _mm_setzero_si128());

        __m128i y_q0 = _mm_unpacklo_epi16(y_lo, _mm_setzero_si128());
        __m128i y_q1 = _mm_unpackhi_epi16(y_lo, _mm_setzero_si128());
        __m128i y_q2 = _mm_unpacklo_epi16(y_hi, _mm_setzero_si128());
        __m128i y_q3 = _mm_unpackhi_epi16(y_hi, _mm_setzero_si128());

        __m128i quot_q0 = _mm_cvttps_epi32(_mm_div_ps(_mm_castsi128_ps(x0), _mm_castsi128_ps(y_q0)));
        __m128i quot_q1 = _mm_cvttps_epi32(_mm_div_ps(_mm_castsi128_ps(x1), _mm_castsi128_ps(y_q1)));
        __m128i quot_q2 = _mm_cvttps_epi32(_mm_div_ps(_mm_castsi128_ps(x2), _mm_castsi128_ps(y_q2)));
        __m128i quot_q3 = _mm_cvttps_epi32(_mm_div_ps(_mm_castsi128_ps(x3), _mm_castsi128_ps(y_q3)));

        __m128i quot_h0 = _mm_packs_epi32(quot_q0, quot_q1);
        __m128i quot_h1 = _mm_packs_epi32(quot_q2, quot_q3);

        __m128i quot = _mm_packus_epi16(quot_h0, quot_h1);
        return vec16x8u{quot};
    }

    auto vec16x8u_32f_div_subnormal_unpacking = vector_division_test_bench<vec16x8u, vec16x8u_32f_div_subnormal_unpacking_impl>;

    BENCHMARK(div_8u::vec16x8u_32f_div_subnormal_unpacking);

    #endif



    #if defined(AVEL_SSE2)

    vec16x8u vec16x8u_32f_div_normal_unpacking_impl(vec16x8u x, vec16x8u y) {
        __m128i x_raw = decay(x);

        __m128i x_h0 = _mm_unpacklo_epi8(x_raw, _mm_setzero_si128());
        __m128i x_h1 = _mm_unpackhi_epi8(x_raw, _mm_setzero_si128());

        __m128i x_q0 = _mm_unpacklo_epi16(x_h0, _mm_setzero_si128());
        __m128i x_q1 = _mm_unpackhi_epi16(x_h0, _mm_setzero_si128());
        __m128i x_q2 = _mm_unpacklo_epi16(x_h1, _mm_setzero_si128());
        __m128i x_q3 = _mm_unpackhi_epi16(x_h1, _mm_setzero_si128());

        __m128i y_raw = decay(y);

        __m128i y_lo = _mm_unpacklo_epi8(y_raw, _mm_setzero_si128());
        __m128i y_hi = _mm_unpackhi_epi8(y_raw, _mm_setzero_si128());

        __m128i y_q0 = _mm_unpacklo_epi16(y_lo, _mm_setzero_si128());
        __m128i y_q1 = _mm_unpackhi_epi16(y_lo, _mm_setzero_si128());
        __m128i y_q2 = _mm_unpacklo_epi16(y_hi, _mm_setzero_si128());
        __m128i y_q3 = _mm_unpackhi_epi16(y_hi, _mm_setzero_si128());

        __m128i quot_q0 = _mm_cvttps_epi32(_mm_div_ps(_mm_cvtepi32_ps(x_q0), _mm_cvtepi32_ps(y_q0)));
        __m128i quot_q1 = _mm_cvttps_epi32(_mm_div_ps(_mm_cvtepi32_ps(x_q1), _mm_cvtepi32_ps(y_q1)));
        __m128i quot_q2 = _mm_cvttps_epi32(_mm_div_ps(_mm_cvtepi32_ps(x_q2), _mm_cvtepi32_ps(y_q2)));
        __m128i quot_q3 = _mm_cvttps_epi32(_mm_div_ps(_mm_cvtepi32_ps(x_q3), _mm_cvtepi32_ps(y_q3)));

        __m128i quot_h0 = _mm_packs_epi32(quot_q0, quot_q1);
        __m128i quot_h1 = _mm_packs_epi32(quot_q2, quot_q3);

        __m128i quot = _mm_packus_epi16(quot_h0, quot_h1);
        return vec16x8u{quot};
    }

    auto vec16x8u_32f_div_normal_unpacking = vector_division_test_bench<vec16x8u, vec16x8u_32f_div_normal_unpacking_impl>;

    BENCHMARK(div_8u::vec16x8u_32f_div_normal_unpacking);

    #endif



    #if defined(AVEL_SSE2)

    vec16x8u vec16x8u_32f_div_normal_masking_impl(vec16x8u x, vec16x8u y) {
        const __m128i mask0 = _mm_set1_epi32(0x000000ff);
        const __m128i mask1 = _mm_set1_epi32(0x0000ff00);
        const __m128i mask2 = _mm_set1_epi32(0x00ff0000);

        __m128i x_q0 = _mm_and_si128(decay(x), mask0);
        __m128i x_q1 = _mm_and_si128(decay(x), mask1);
        __m128i x_q2 = _mm_and_si128(decay(x), mask2);
        __m128i x_q3 = _mm_srli_epi32(decay(x), 24);

        __m128i y_q0 = _mm_and_si128(decay(y), mask0);
        __m128i y_q1 = _mm_and_si128(decay(y), mask1);
        __m128i y_q2 = _mm_and_si128(decay(y), mask2);
        __m128i y_q3 = _mm_srli_epi32(decay(y), 24);

        __m128i quot_q0 = _mm_cvttps_epi32(_mm_div_ps(_mm_cvtepi32_ps(x_q0), _mm_cvtepi32_ps(y_q0)));
        __m128i quot_q1 = _mm_cvttps_epi32(_mm_div_ps(_mm_cvtepi32_ps(x_q1), _mm_cvtepi32_ps(y_q1)));
        __m128i quot_q2 = _mm_cvttps_epi32(_mm_div_ps(_mm_cvtepi32_ps(x_q2), _mm_cvtepi32_ps(y_q2)));
        __m128i quot_q3 = _mm_cvttps_epi32(_mm_div_ps(_mm_cvtepi32_ps(x_q3), _mm_cvtepi32_ps(y_q3)));

        __m128i quot = _mm_or_si128(
            _mm_or_si128(
                _mm_slli_epi32(quot_q0, 0),
                _mm_slli_epi32(quot_q1, 8)
            ),
            _mm_or_si128(
                _mm_slli_epi32(quot_q2, 16),
                _mm_slli_epi32(quot_q3, 24)
            )
        );

        return vec16x8u{quot};
    }

    auto vec16x8u_32f_div_normal_masking = vector_division_test_bench<vec16x8u, vec16x8u_32f_div_normal_masking_impl>;

    BENCHMARK(div_8u::vec16x8u_32f_div_normal_masking);

    #endif




    #if defined(AVEL_AVX2)

    vec16x8u vec16x8u_32f_div_normal_widening256_impl(vec16x8u x, vec16x8u y) {
        __m256i x_h0 = _mm256_cvtepu8_epi32(decay(x));
        __m256i x_h1 = _mm256_cvtepu8_epi32(_mm_bsrli_si128(decay(x), 8));

        __m256i y_h0 = _mm256_cvtepu8_epi32(decay(y));
        __m256i y_h1 = _mm256_cvtepu8_epi32(_mm_bsrli_si128(decay(y), 8));

        __m256i quot_q0 = _mm256_cvttps_epi32(_mm256_div_ps(_mm256_cvtepi32_ps(x_h0), _mm256_cvtepi32_ps(y_h0)));
        __m256i quot_q1 = _mm256_cvttps_epi32(_mm256_div_ps(_mm256_cvtepi32_ps(x_h1), _mm256_cvtepi32_ps(y_h1)));

        __m256i quot32 = _mm256_packus_epi16(quot_q0, quot_q1);
        __m128i quot16_h0 = _mm256_extracti128_si256(quot32, 0x0);
        __m128i quot16_h1 = _mm256_extracti128_si256(quot32, 0x1);

        __m128i quot_shuffled = _mm_packus_epi16(quot16_h0, quot16_h1);
        __m128i quot = _mm_shuffle_epi32(quot_shuffled, 0xd8);

        return vec16x8u{quot};
    }

    auto vec16x8u_32f_div_normal_widening256 = vector_division_test_bench<vec16x8u, vec16x8u_32f_div_normal_widening256_impl>;

    BENCHMARK(div_8u::vec16x8u_32f_div_normal_widening256);

    #endif




    #if defined(AVEL_AVX512F)

    vec16x8u vec16x8u_32f_div_normal_widening512_impl(vec16x8u x, vec16x8u y) {
        __m128i quot = _mm512_cvtepi32_epi8(
            _mm512_cvttps_epi32(
                _mm512_div_ps(
                    _mm512_cvtepi32_ps(_mm512_cvtepu8_epi32(decay(x))),
                    _mm512_cvtepi32_ps(_mm512_cvtepu8_epi32(decay(y)))
                )
            )
        );

        return vec16x8u{quot};
    }

    auto vec16x8u_32f_div_normal_widening512 = vector_division_test_bench<vec16x8u, vec16x8u_32f_div_normal_widening512_impl>;

    BENCHMARK(div_8u::vec16x8u_32f_div_normal_widening512);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)

    vec16x8u vec16x8u_32f_div_normal_kmasking_impl(vec16x8u x, vec16x8u y) {
        const __mmask16 mask0 = 0x1111;
        const __mmask16 mask1 = 0x2222;
        const __mmask16 mask2 = 0x4444;

        __m128i x_q0 = _mm_maskz_mov_epi8(mask0, decay(x));
        __m128i x_q1 = _mm_maskz_mov_epi8(mask1, decay(x));
        __m128i x_q2 = _mm_maskz_mov_epi8(mask2, decay(x));
        __m128i x_q3 = _mm_srli_epi32(decay(x), 24);

        __m128i y_q0 = _mm_maskz_mov_epi8(mask0, decay(y));
        __m128i y_q1 = _mm_maskz_mov_epi8(mask1, decay(y));
        __m128i y_q2 = _mm_maskz_mov_epi8(mask2, decay(y));
        __m128i y_q3 = _mm_srli_epi32(decay(y), 24);

        __m128i quot_q0 = _mm_cvttps_epi32(_mm_div_ps(_mm_cvtepi32_ps(x_q0), _mm_cvtepi32_ps(y_q0)));
        __m128i quot_q1 = _mm_cvttps_epi32(_mm_div_ps(_mm_cvtepi32_ps(x_q1), _mm_cvtepi32_ps(y_q1)));
        __m128i quot_q2 = _mm_cvttps_epi32(_mm_div_ps(_mm_cvtepi32_ps(x_q2), _mm_cvtepi32_ps(y_q2)));
        __m128i quot_q3 = _mm_cvttps_epi32(_mm_div_ps(_mm_cvtepi32_ps(x_q3), _mm_cvtepi32_ps(y_q3)));

        __m128i quot = _mm_or_si128(
            _mm_slli_epi32(quot_q3, 24),
            _mm_ternarylogic_epi32(
                quot_q0,
                _mm_slli_epi32(quot_q1, 8),
                _mm_slli_epi32(quot_q2, 16),
                0xfe
            )
        );

        return vec16x8u{quot};
    }

    auto vec16x8u_32f_div_normal_kmasking = vector_division_test_bench<vec16x8u, vec16x8u_32f_div_normal_kmasking_impl>;

    BENCHMARK(div_8u::vec16x8u_32f_div_normal_kmasking);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW) && defined(AVEL_AVX512VBMI)

    vec16x8u vec16x8u_recip_lut_impl(vec16x8u x, vec16x8u y) {
        alignas(64) static constexpr std::uint8_t recip_hi_table_data[128] {
            0x00, 0x80, 0x55, 0x40, 0x33, 0x2a, 0x24, 0x20,
            0x1c, 0x19, 0x17, 0x15, 0x13, 0x12, 0x11, 0x10,
            0x0f, 0x0e, 0x0d, 0x0c, 0x0c, 0x0b, 0x0b, 0x0a,
            0x0a, 0x09, 0x09, 0x09, 0x08, 0x08, 0x08, 0x08,
            0x07, 0x07, 0x07, 0x07, 0x06, 0x06, 0x06, 0x06,
            0x06, 0x06, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
            0x05, 0x05, 0x05, 0x04, 0x04, 0x04, 0x04, 0x04,
            0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
            0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
            0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
            0x03, 0x03, 0x03, 0x03, 0x03, 0x02, 0x02, 0x02,
            0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
            0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
            0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
            0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
            0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02
        };

        alignas(64) static constexpr std::uint8_t recip_lo_table_data[256] {
            0x00, 0x00, 0x56, 0x00, 0x34, 0xab, 0x93, 0x00,
            0x72, 0x9a, 0x46, 0x56, 0xb2, 0x4a, 0x12, 0x00,
            0x10, 0x39, 0x7a, 0xcd, 0x31, 0xa3, 0x22, 0xab,
            0x3e, 0xd9, 0x7c, 0x25, 0xd4, 0x89, 0x43, 0x00,
            0xc2, 0x88, 0x51, 0x1d, 0xec, 0xbd, 0x91, 0x67,
            0x3f, 0x19, 0xf5, 0xd2, 0xb1, 0x91, 0x73, 0x56,
            0x3a, 0x1f, 0x06, 0xed, 0xd5, 0xbe, 0xa8, 0x93,
            0x7e, 0x6a, 0x57, 0x45, 0x33, 0x22, 0x11, 0x00,
            0xf1, 0xe1, 0xd3, 0xc4, 0xb6, 0xa9, 0x9c, 0x8f,
            0x82, 0x76, 0x6a, 0x5f, 0x54, 0x49, 0x3e, 0x34,
            0x2a, 0x20, 0x16, 0x0d, 0x04, 0xfb, 0xf2, 0xe9,
            0xe1, 0xd9, 0xd1, 0xc9, 0xc1, 0xba, 0xb2, 0xab,
            0xa4, 0x9d, 0x96, 0x90, 0x89, 0x83, 0x7d, 0x77,
            0x71, 0x6b, 0x65, 0x5f, 0x5a, 0x54, 0x4f, 0x4a,
            0x44, 0x3f, 0x3a, 0x35, 0x31, 0x2c, 0x27, 0x23,
            0x1e, 0x1a, 0x15, 0x11, 0x0d, 0x09, 0x05, 0x00,
            0xfd, 0xf9, 0xf5, 0xf1, 0xed, 0xea, 0xe6, 0xe2,
            0xdf, 0xdb, 0xd8, 0xd5, 0xd1, 0xce, 0xcb, 0xc8,
            0xc4, 0xc1, 0xbe, 0xbb, 0xb8, 0xb5, 0xb3, 0xb0,
            0xad, 0xaa, 0xa7, 0xa5, 0xa2, 0x9f, 0x9d, 0x9a,
            0x98, 0x95, 0x93, 0x90, 0x8e, 0x8b, 0x89, 0x87,
            0x84, 0x82, 0x80, 0x7e, 0x7b, 0x79, 0x77, 0x75,
            0x73, 0x71, 0x6f, 0x6d, 0x6b, 0x69, 0x67, 0x65,
            0x63, 0x61, 0x5f, 0x5d, 0x5b, 0x59, 0x58, 0x56,
            0x54, 0x52, 0x51, 0x4f, 0x4d, 0x4b, 0x4a, 0x48,
            0x47, 0x45, 0x43, 0x42, 0x40, 0x3f, 0x3d, 0x3c,
            0x3a, 0x39, 0x37, 0x36, 0x34, 0x33, 0x31, 0x30,
            0x2f, 0x2d, 0x2c, 0x2a, 0x29, 0x28, 0x26, 0x25,
            0x24, 0x22, 0x21, 0x20, 0x1f, 0x1d, 0x1c, 0x1b,
            0x1a, 0x19, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12,
            0x10, 0x0f, 0x0e, 0x0d, 0x0c, 0x0b, 0x0a, 0x09,
            0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x00
        };

        __m512i recip_hi0 = _mm512_load_si512(recip_hi_table_data +   0);
        __m512i recip_hi1 = _mm512_load_si512(recip_hi_table_data +  64);

        __m512i recip_lo0 = _mm512_load_si512(recip_lo_table_data +   0);
        __m512i recip_lo1 = _mm512_load_si512(recip_lo_table_data +  64);
        __m512i recip_lo2 = _mm512_load_si512(recip_lo_table_data + 128);
        __m512i recip_lo3 = _mm512_load_si512(recip_lo_table_data + 192);

        __m512i one = _mm512_set1_epi8(1);

        // Don't care about y == 0
        __m512i z = _mm512_sub_epi8(_mm512_castsi128_si512(decay(y)), one);
        __mmask64 high_bit_mask = _mm512_movepi8_mask(z);

        // Lookup low/high reciprocal halves
        __m512i recip_hi = _mm512_mask_blend_epi8(
            high_bit_mask,
            _mm512_permutex2var_epi8(recip_hi0, z, recip_hi1),
            one
        );

        __m512i recip_lo = _mm512_mask_blend_epi8(
            high_bit_mask,
            _mm512_permutex2var_epi8(recip_lo0, z, recip_lo1),
            _mm512_permutex2var_epi8(recip_lo2, z, recip_lo3)
        );

        // Construct 16-bit reciprocal
        __m512i zeros = _mm512_setzero_si512();
        __m512i reciprocals_lo = _mm512_unpacklo_epi8(recip_lo, recip_hi);
        __m512i reciprocals_hi = _mm512_unpackhi_epi8(recip_lo, recip_hi);

        __m512i x_lo = _mm512_unpacklo_epi8(_mm512_castsi128_si512(decay(x)), zeros);
        __m512i x_hi = _mm512_unpackhi_epi8(_mm512_castsi128_si512(decay(x)), zeros);

        // Compute quotient
        __m512i quotient_lo = _mm512_mulhi_epu16(x_lo, reciprocals_lo);
        __m512i quotient_hi = _mm512_mulhi_epu16(x_hi, reciprocals_hi);

        __m512i quotient = _mm512_packus_epi16(quotient_lo, quotient_hi);

        return vec16x8u{_mm512_castsi512_si128(quotient)};
    }

    auto vec16x8u_recip_lut = vector_division_test_bench<vec16x8u, vec16x8u_recip_lut_impl>;

    BENCHMARK(div_8u::vec16x8u_recip_lut);

    #endif

    //=====================================================
    // vec32x8u benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec32x8u vec32x8u_long_division_unrolled_impl(vec32x8u x, vec32x8u y) {
        __m256i x_reg = decay(x);
        __m256i y_reg = decay(y);

        __m256i undef = _mm256_undefined_si256();
        __m256i bit_mask = _mm256_cmpeq_epi8(undef, undef);
        __m256i q_reg = _mm256_setzero_si256();

        {   // Iteration 0
            bit_mask = _mm256_add_epi8(bit_mask, bit_mask);

            __m256i x_shifted = _mm256_andnot_si256(bit_mask, _mm256_srli_epi16(x_reg, 7));
            __m256i y_shifted = _mm256_slli_epi16(_mm256_andnot_si256(bit_mask, y_reg), 7);

            __m256i mask = _mm256_cmpeq_epi8(_mm256_max_epu8(x_shifted, y_reg), x_shifted);
            x_reg = _mm256_sub_epi8(x_reg, _mm256_and_si256(mask, y_shifted));
            q_reg = _mm256_add_epi8(q_reg, q_reg);
            q_reg = _mm256_sub_epi8(q_reg, mask);
        }

        {   // Iteration 1
            bit_mask = _mm256_add_epi8(bit_mask, bit_mask);

            __m256i x_shifted = _mm256_andnot_si256(bit_mask, _mm256_srli_epi16(x_reg, 6));
            __m256i y_shifted = _mm256_slli_epi16(_mm256_andnot_si256(bit_mask, y_reg), 6);

            __m256i mask = _mm256_cmpeq_epi8(_mm256_max_epu8(x_shifted, y_reg), x_shifted);
            x_reg = _mm256_sub_epi8(x_reg, _mm256_and_si256(mask, y_shifted));
            q_reg = _mm256_add_epi8(q_reg, q_reg);
            q_reg = _mm256_sub_epi8(q_reg, mask);
        }

        {   // Iteration 2
            bit_mask = _mm256_add_epi8(bit_mask, bit_mask);

            __m256i x_shifted = _mm256_andnot_si256(bit_mask, _mm256_srli_epi16(x_reg, 5));
            __m256i y_shifted = _mm256_slli_epi16(_mm256_andnot_si256(bit_mask, y_reg), 5);

            __m256i mask = _mm256_cmpeq_epi8(_mm256_max_epu8(x_shifted, y_reg), x_shifted);
            x_reg = _mm256_sub_epi8(x_reg, _mm256_and_si256(mask, y_shifted));
            q_reg = _mm256_add_epi8(q_reg, q_reg);
            q_reg = _mm256_sub_epi8(q_reg, mask);
        }

        {   // Iteration 3
            bit_mask = _mm256_add_epi8(bit_mask, bit_mask);

            __m256i x_shifted = _mm256_andnot_si256(bit_mask, _mm256_srli_epi16(x_reg, 4));
            __m256i y_shifted = _mm256_slli_epi16(_mm256_andnot_si256(bit_mask, y_reg), 4);

            __m256i mask = _mm256_cmpeq_epi8(_mm256_max_epu8(x_shifted, y_reg), x_shifted);
            x_reg = _mm256_sub_epi8(x_reg, _mm256_and_si256(mask, y_shifted));
            q_reg = _mm256_add_epi8(q_reg, q_reg);
            q_reg = _mm256_sub_epi8(q_reg, mask);
        }

        {   // Iteration 4
            bit_mask = _mm256_add_epi8(bit_mask, bit_mask);

            __m256i x_shifted = _mm256_andnot_si256(bit_mask, _mm256_srli_epi16(x_reg, 3));
            __m256i y_shifted = _mm256_slli_epi16(_mm256_andnot_si256(bit_mask, y_reg), 3);

            __m256i mask = _mm256_cmpeq_epi8(_mm256_max_epu8(x_shifted, y_reg), x_shifted);
            x_reg = _mm256_sub_epi8(x_reg, _mm256_and_si256(mask, y_shifted));
            q_reg = _mm256_add_epi8(q_reg, q_reg);
            q_reg = _mm256_sub_epi8(q_reg, mask);
        }

        {   // Iteration 5
            bit_mask = _mm256_add_epi8(bit_mask, bit_mask);

            __m256i x_shifted = _mm256_andnot_si256(bit_mask, _mm256_srli_epi16(x_reg, 2));
            __m256i y_shifted = _mm256_add_epi8(y_reg, y_reg);
            y_shifted = _mm256_add_epi8(y_shifted, y_shifted);

            __m256i mask = _mm256_cmpeq_epi8(_mm256_max_epu8(x_shifted, y_reg), x_shifted);
            x_reg = _mm256_sub_epi8(x_reg, _mm256_and_si256(mask, y_shifted));
            q_reg = _mm256_add_epi8(q_reg, q_reg);
            q_reg = _mm256_sub_epi8(q_reg, mask);
        }

        {   // Iteration 6
            bit_mask = _mm256_add_epi8(bit_mask, bit_mask);

            __m256i x_shifted = _mm256_andnot_si256(bit_mask, _mm256_srli_epi16(x_reg, 1));
            __m256i y_shifted = _mm256_add_epi8(y_reg, y_reg);

            __m256i mask = _mm256_cmpeq_epi8(_mm256_max_epu8(x_shifted, y_reg), x_shifted);
            x_reg = _mm256_sub_epi8(x_reg, _mm256_and_si256(mask, y_shifted));
            q_reg = _mm256_add_epi8(q_reg, q_reg);
            q_reg = _mm256_sub_epi8(q_reg, mask);
        }

        {   // Iteration 7
            __m256i x_shifted = x_reg;
            __m256i y_shifted = y_reg;

            __m256i mask = _mm256_cmpeq_epi8(_mm256_max_epu8(x_shifted, y_reg), x_shifted);
            x_reg = _mm256_sub_epi8(x_reg, _mm256_and_si256(mask, y_shifted));
            q_reg = _mm256_add_epi8(q_reg, q_reg);
            q_reg = _mm256_sub_epi8(q_reg, mask);
        }

        return vec32x8u{q_reg};
    }

    auto vec32x8u_long_division_unrolled = vector_division_test_bench<vec32x8u, vec32x8u_long_division_unrolled_impl>;

    BENCHMARK(div_8u::vec32x8u_long_division_unrolled);

    #endif



    #if defined(AVEL_AVX2)

    vec32x8u vec32x8u_generic_long_division_loop_impl(vec32x8u x, vec32x8u y) {
        // The lack of 8-bit shift instructions and native comparison operators means this can theoretically be made
        // better with careful attention
        vec32x8u quotient{};

        for (std::int32_t i = 8; (i-- > 0) && any(x);) {
            mask32x8u b = ((x >> i) >= y);
            x -= keep(b, y << i);
            quotient |= (vec32x8u{b} << i);
        }

        return quotient;
    }

    auto vec32x8u_generic_long_division_loop = vector_division_test_bench<vec32x8u, vec32x8u_generic_long_division_loop_impl>;

    BENCHMARK(div_8u::vec32x8u_generic_long_division_loop);

    #endif




    #if defined(AVEL_AVX2)

    vec32x8u vec32x8u_32f_div_subnormal_unpacking_impl(vec32x8u x, vec32x8u y) {
        __m256i x_raw = decay(x);

        __m256i x_lo = _mm256_unpacklo_epi8(x_raw, _mm256_setzero_si256());
        __m256i x_hi = _mm256_unpackhi_epi8(x_raw, _mm256_setzero_si256());

        __m256i x0 = _mm256_unpacklo_epi16(x_lo, _mm256_setzero_si256());
        __m256i x1 = _mm256_unpackhi_epi16(x_lo, _mm256_setzero_si256());
        __m256i x2 = _mm256_unpacklo_epi16(x_hi, _mm256_setzero_si256());
        __m256i x3 = _mm256_unpackhi_epi16(x_hi, _mm256_setzero_si256());

        __m256i y_raw = decay(y);

        __m256i y_lo = _mm256_unpacklo_epi8(y_raw, _mm256_setzero_si256());
        __m256i y_hi = _mm256_unpackhi_epi8(y_raw, _mm256_setzero_si256());

        __m256i y_q0 = _mm256_unpacklo_epi16(y_lo, _mm256_setzero_si256());
        __m256i y_q1 = _mm256_unpackhi_epi16(y_lo, _mm256_setzero_si256());
        __m256i y_q2 = _mm256_unpacklo_epi16(y_hi, _mm256_setzero_si256());
        __m256i y_q3 = _mm256_unpackhi_epi16(y_hi, _mm256_setzero_si256());

        __m256i quot_q0 = _mm256_cvttps_epi32(_mm256_div_ps(_mm256_castsi256_ps(x0), _mm256_castsi256_ps(y_q0)));
        __m256i quot_q1 = _mm256_cvttps_epi32(_mm256_div_ps(_mm256_castsi256_ps(x1), _mm256_castsi256_ps(y_q1)));
        __m256i quot_q2 = _mm256_cvttps_epi32(_mm256_div_ps(_mm256_castsi256_ps(x2), _mm256_castsi256_ps(y_q2)));
        __m256i quot_q3 = _mm256_cvttps_epi32(_mm256_div_ps(_mm256_castsi256_ps(x3), _mm256_castsi256_ps(y_q3)));

        __m256i quot_h0 = _mm256_packus_epi32(quot_q0, quot_q1);
        __m256i quot_h1 = _mm256_packus_epi32(quot_q2, quot_q3);

        __m256i quot = _mm256_packus_epi16(quot_h0, quot_h1);
        return vec32x8u{quot};
    }

    auto vec32x8u_32f_div_subnormal_unpacking = vector_division_test_bench<vec32x8u, vec32x8u_32f_div_subnormal_unpacking_impl>;

    BENCHMARK(div_8u::vec32x8u_32f_div_subnormal_unpacking);

    #endif



    #if defined(AVEL_AVX2)

    vec32x8u vec32x8u_32f_div_normal_masking_impl(vec32x8u x, vec32x8u y) {
        const __m256i mask0 = _mm256_set1_epi32(0x000000ff);
        const __m256i mask1 = _mm256_set1_epi32(0x0000ff00);
        const __m256i mask2 = _mm256_set1_epi32(0x00ff0000);

        __m256i x_q0 = _mm256_and_si256(decay(x), mask0);
        __m256i x_q1 = _mm256_and_si256(decay(x), mask1);
        __m256i x_q2 = _mm256_and_si256(decay(x), mask2);
        __m256i x_q3 = _mm256_srli_epi32(decay(x), 24);

        __m256i y_q0 = _mm256_and_si256(decay(y), mask0);
        __m256i y_q1 = _mm256_and_si256(decay(y), mask1);
        __m256i y_q2 = _mm256_and_si256(decay(y), mask2);
        __m256i y_q3 = _mm256_srli_epi32(decay(y), 24);

        __m256i quot_q0 = _mm256_cvttps_epi32(_mm256_div_ps(_mm256_cvtepi32_ps(x_q0), _mm256_cvtepi32_ps(y_q0)));
        __m256i quot_q1 = _mm256_cvttps_epi32(_mm256_div_ps(_mm256_cvtepi32_ps(x_q1), _mm256_cvtepi32_ps(y_q1)));
        __m256i quot_q2 = _mm256_cvttps_epi32(_mm256_div_ps(_mm256_cvtepi32_ps(x_q2), _mm256_cvtepi32_ps(y_q2)));
        __m256i quot_q3 = _mm256_cvttps_epi32(_mm256_div_ps(_mm256_cvtepi32_ps(x_q3), _mm256_cvtepi32_ps(y_q3)));

        __m256i quot = _mm256_or_si256(
            _mm256_or_si256(
                _mm256_slli_epi32(quot_q0, 0),
                _mm256_slli_epi32(quot_q1, 8)
            ),
            _mm256_or_si256(
                _mm256_slli_epi32(quot_q2, 16),
                _mm256_slli_epi32(quot_q3, 24)
            )
        );

        return vec32x8u{quot};
    }

    auto vec32x8u_32f_div_normal_masking = vector_division_test_bench<vec32x8u, vec32x8u_32f_div_normal_masking_impl>;

    BENCHMARK(div_8u::vec32x8u_32f_div_normal_masking);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)

    vec32x8u vec32x8u_32f_div_normal_kmasking_impl(vec32x8u x, vec32x8u y) {
        const __mmask32 mask0 = 0x11111111;
        const __mmask32 mask1 = 0x22222222;
        const __mmask32 mask2 = 0x44444444;

        __m256i x_q0 = _mm256_maskz_mov_epi8(mask0, decay(x));
        __m256i x_q1 = _mm256_maskz_mov_epi8(mask1, decay(x));
        __m256i x_q2 = _mm256_maskz_mov_epi8(mask2, decay(x));
        __m256i x_q3 = _mm256_srli_epi32(decay(x), 24);

        __m256i y_q0 = _mm256_maskz_mov_epi8(mask0, decay(y));
        __m256i y_q1 = _mm256_maskz_mov_epi8(mask1, decay(y));
        __m256i y_q2 = _mm256_maskz_mov_epi8(mask2, decay(y));
        __m256i y_q3 = _mm256_srli_epi32(decay(y), 24);

        __m256i quot_q0 = _mm256_cvttps_epi32(_mm256_div_ps(_mm256_cvtepi32_ps(x_q0), _mm256_cvtepi32_ps(y_q0)));
        __m256i quot_q1 = _mm256_cvttps_epi32(_mm256_div_ps(_mm256_cvtepi32_ps(x_q1), _mm256_cvtepi32_ps(y_q1)));
        __m256i quot_q2 = _mm256_cvttps_epi32(_mm256_div_ps(_mm256_cvtepi32_ps(x_q2), _mm256_cvtepi32_ps(y_q2)));
        __m256i quot_q3 = _mm256_cvttps_epi32(_mm256_div_ps(_mm256_cvtepi32_ps(x_q3), _mm256_cvtepi32_ps(y_q3)));

        __m256i quot = _mm256_or_si256(
            _mm256_slli_epi32(quot_q3, 24),
            _mm256_ternarylogic_epi32(
                quot_q0,
                _mm256_slli_epi32(quot_q1, 8),
                _mm256_slli_epi32(quot_q2, 16),
                0xfe
            )
        );

        return vec32x8u{quot};
    }

    auto vec32x8u_32f_div_normal_kmasking = vector_division_test_bench<vec32x8u, vec32x8u_32f_div_normal_kmasking_impl>;

    BENCHMARK(div_8u::vec32x8u_32f_div_normal_kmasking);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW) && defined(AVEL_AVX512VBMI)

    vec32x8u vec32x8u_recip_lut_impl(vec32x8u x, vec32x8u y) {
        alignas(64) static constexpr std::uint8_t recip_hi_table_data[128] {
            0x00, 0x80, 0x55, 0x40, 0x33, 0x2a, 0x24, 0x20,
            0x1c, 0x19, 0x17, 0x15, 0x13, 0x12, 0x11, 0x10,
            0x0f, 0x0e, 0x0d, 0x0c, 0x0c, 0x0b, 0x0b, 0x0a,
            0x0a, 0x09, 0x09, 0x09, 0x08, 0x08, 0x08, 0x08,
            0x07, 0x07, 0x07, 0x07, 0x06, 0x06, 0x06, 0x06,
            0x06, 0x06, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
            0x05, 0x05, 0x05, 0x04, 0x04, 0x04, 0x04, 0x04,
            0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
            0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
            0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
            0x03, 0x03, 0x03, 0x03, 0x03, 0x02, 0x02, 0x02,
            0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
            0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
            0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
            0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
            0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02
        };

        alignas(64) static constexpr std::uint8_t recip_lo_table_data[256] {
            0x00, 0x00, 0x56, 0x00, 0x34, 0xab, 0x93, 0x00,
            0x72, 0x9a, 0x46, 0x56, 0xb2, 0x4a, 0x12, 0x00,
            0x10, 0x39, 0x7a, 0xcd, 0x31, 0xa3, 0x22, 0xab,
            0x3e, 0xd9, 0x7c, 0x25, 0xd4, 0x89, 0x43, 0x00,
            0xc2, 0x88, 0x51, 0x1d, 0xec, 0xbd, 0x91, 0x67,
            0x3f, 0x19, 0xf5, 0xd2, 0xb1, 0x91, 0x73, 0x56,
            0x3a, 0x1f, 0x06, 0xed, 0xd5, 0xbe, 0xa8, 0x93,
            0x7e, 0x6a, 0x57, 0x45, 0x33, 0x22, 0x11, 0x00,
            0xf1, 0xe1, 0xd3, 0xc4, 0xb6, 0xa9, 0x9c, 0x8f,
            0x82, 0x76, 0x6a, 0x5f, 0x54, 0x49, 0x3e, 0x34,
            0x2a, 0x20, 0x16, 0x0d, 0x04, 0xfb, 0xf2, 0xe9,
            0xe1, 0xd9, 0xd1, 0xc9, 0xc1, 0xba, 0xb2, 0xab,
            0xa4, 0x9d, 0x96, 0x90, 0x89, 0x83, 0x7d, 0x77,
            0x71, 0x6b, 0x65, 0x5f, 0x5a, 0x54, 0x4f, 0x4a,
            0x44, 0x3f, 0x3a, 0x35, 0x31, 0x2c, 0x27, 0x23,
            0x1e, 0x1a, 0x15, 0x11, 0x0d, 0x09, 0x05, 0x00,
            0xfd, 0xf9, 0xf5, 0xf1, 0xed, 0xea, 0xe6, 0xe2,
            0xdf, 0xdb, 0xd8, 0xd5, 0xd1, 0xce, 0xcb, 0xc8,
            0xc4, 0xc1, 0xbe, 0xbb, 0xb8, 0xb5, 0xb3, 0xb0,
            0xad, 0xaa, 0xa7, 0xa5, 0xa2, 0x9f, 0x9d, 0x9a,
            0x98, 0x95, 0x93, 0x90, 0x8e, 0x8b, 0x89, 0x87,
            0x84, 0x82, 0x80, 0x7e, 0x7b, 0x79, 0x77, 0x75,
            0x73, 0x71, 0x6f, 0x6d, 0x6b, 0x69, 0x67, 0x65,
            0x63, 0x61, 0x5f, 0x5d, 0x5b, 0x59, 0x58, 0x56,
            0x54, 0x52, 0x51, 0x4f, 0x4d, 0x4b, 0x4a, 0x48,
            0x47, 0x45, 0x43, 0x42, 0x40, 0x3f, 0x3d, 0x3c,
            0x3a, 0x39, 0x37, 0x36, 0x34, 0x33, 0x31, 0x30,
            0x2f, 0x2d, 0x2c, 0x2a, 0x29, 0x28, 0x26, 0x25,
            0x24, 0x22, 0x21, 0x20, 0x1f, 0x1d, 0x1c, 0x1b,
            0x1a, 0x19, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12,
            0x10, 0x0f, 0x0e, 0x0d, 0x0c, 0x0b, 0x0a, 0x09,
            0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x00
        };

        __m512i recip_hi0 = _mm512_load_si512(recip_hi_table_data +   0);
        __m512i recip_hi1 = _mm512_load_si512(recip_hi_table_data +  64);

        __m512i recip_lo0 = _mm512_load_si512(recip_lo_table_data +   0);
        __m512i recip_lo1 = _mm512_load_si512(recip_lo_table_data +  64);
        __m512i recip_lo2 = _mm512_load_si512(recip_lo_table_data + 128);
        __m512i recip_lo3 = _mm512_load_si512(recip_lo_table_data + 192);

        __m512i one = _mm512_set1_epi8(1);

        // Don't care about y == 0
        __m512i z = _mm512_sub_epi8(_mm512_castsi256_si512(decay(y)), one);
        __mmask64 high_bit_mask = _mm512_movepi8_mask(z);

        // Lookup low/high reciprocal halves
        __m512i recip_hi = _mm512_mask_blend_epi8(
            high_bit_mask,
            _mm512_permutex2var_epi8(recip_hi0, z, recip_hi1),
            one
        );

        __m512i recip_lo = _mm512_mask_blend_epi8(
            high_bit_mask,
            _mm512_permutex2var_epi8(recip_lo0, z, recip_lo1),
            _mm512_permutex2var_epi8(recip_lo2, z, recip_lo3)
        );

        // Construct 16-bit reciprocal
        __m512i zeros = _mm512_setzero_si512();
        __m512i reciprocals_lo = _mm512_unpacklo_epi8(recip_lo, recip_hi);
        __m512i reciprocals_hi = _mm512_unpackhi_epi8(recip_lo, recip_hi);

        __m512i x_lo = _mm512_unpacklo_epi8(_mm512_castsi256_si512(decay(x)), zeros);
        __m512i x_hi = _mm512_unpackhi_epi8(_mm512_castsi256_si512(decay(x)), zeros);

        // Compute quotient
        __m512i quotient_lo = _mm512_mulhi_epu16(x_lo, reciprocals_lo);
        __m512i quotient_hi = _mm512_mulhi_epu16(x_hi, reciprocals_hi);

        __m512i quotient = _mm512_packus_epi16(quotient_lo, quotient_hi);

        return vec32x8u{_mm512_castsi512_si256(quotient)};
    }

    auto vec32x8u_recip_lut = vector_division_test_bench<vec32x8u, vec32x8u_recip_lut_impl>;

    BENCHMARK(div_8u::vec32x8u_recip_lut);

    #endif

    //=====================================================
    // vec64x8u benchmarks
    //=====================================================

    //TODO: Add unrolled loop variant

    #if defined(AVEL_AVX512BW)

    vec64x8u vec64x8u_long_division_unrolled_impl(vec64x8u x, vec64x8u y) {
        __m512i x_reg = decay(x);
        __m512i y_reg = decay(y);

        __m512i bit_mask = _mm512_set1_epi8(-1);
        __m512i q_reg = _mm512_setzero_si512();
        __m512i ones = _mm512_set1_epi8(1);

        {   // Iteration 0
            bit_mask = _mm512_add_epi8(bit_mask, bit_mask);

            __m512i x_shifted = _mm512_andnot_si512(bit_mask, _mm512_srli_epi16(x_reg, 7));
            __m512i y_shifted = _mm512_slli_epi16(_mm512_andnot_si512(bit_mask, y_reg), 7);

            __mmask64 mask = _mm512_cmp_epu8_mask(x_shifted, y_reg, _MM_CMPINT_NLT);
            x_reg = _mm512_sub_epi8(x_reg, _mm512_maskz_mov_epi8(mask, y_shifted));
            q_reg = _mm512_add_epi8(q_reg, q_reg);
            q_reg = _mm512_mask_add_epi8(q_reg, mask, q_reg, ones);
        }

        {   // Iteration 1
            bit_mask = _mm512_add_epi8(bit_mask, bit_mask);

            __m512i x_shifted = _mm512_andnot_si512(bit_mask, _mm512_srli_epi16(x_reg, 6));
            __m512i y_shifted = _mm512_slli_epi16(_mm512_andnot_si512(bit_mask, y_reg), 6);

            __mmask64 mask = _mm512_cmp_epu8_mask(x_shifted, y_reg, _MM_CMPINT_NLT);
            x_reg = _mm512_sub_epi8(x_reg, _mm512_maskz_mov_epi8(mask, y_shifted));
            q_reg = _mm512_add_epi8(q_reg, q_reg);
            q_reg = _mm512_mask_add_epi8(q_reg, mask, q_reg, ones);
        }

        {   // Iteration 2
            bit_mask = _mm512_add_epi8(bit_mask, bit_mask);

            __m512i x_shifted = _mm512_andnot_si512(bit_mask, _mm512_srli_epi16(x_reg, 5));
            __m512i y_shifted = _mm512_slli_epi16(_mm512_andnot_si512(bit_mask, y_reg), 5);

            __mmask64 mask = _mm512_cmp_epu8_mask(x_shifted, y_reg, _MM_CMPINT_NLT);
            x_reg = _mm512_sub_epi8(x_reg, _mm512_maskz_mov_epi8(mask, y_shifted));
            q_reg = _mm512_add_epi8(q_reg, q_reg);
            q_reg = _mm512_mask_add_epi8(q_reg, mask, q_reg, ones);
        }

        {   // Iteration 3
            bit_mask = _mm512_add_epi8(bit_mask, bit_mask);

            __m512i x_shifted = _mm512_andnot_si512(bit_mask, _mm512_srli_epi16(x_reg, 4));
            __m512i y_shifted = _mm512_slli_epi16(_mm512_andnot_si512(bit_mask, y_reg), 4);

            __mmask64 mask = _mm512_cmp_epu8_mask(x_shifted, y_reg, _MM_CMPINT_NLT);
            x_reg = _mm512_sub_epi8(x_reg, _mm512_maskz_mov_epi8(mask, y_shifted));
            q_reg = _mm512_add_epi8(q_reg, q_reg);
            q_reg = _mm512_mask_add_epi8(q_reg, mask, q_reg, ones);
        }

        {   // Iteration 4
            bit_mask = _mm512_add_epi8(bit_mask, bit_mask);

            __m512i x_shifted = _mm512_andnot_si512(bit_mask, _mm512_srli_epi16(x_reg, 3));
            __m512i y_shifted = _mm512_slli_epi16(_mm512_andnot_si512(bit_mask, y_reg), 3);

            __mmask64 mask = _mm512_cmp_epu8_mask(x_shifted, y_reg, _MM_CMPINT_NLT);
            x_reg = _mm512_sub_epi8(x_reg, _mm512_maskz_mov_epi8(mask, y_shifted));
            q_reg = _mm512_add_epi8(q_reg, q_reg);
            q_reg = _mm512_mask_add_epi8(q_reg, mask, q_reg, ones);
        }

        {   // Iteration 5
            bit_mask = _mm512_add_epi8(bit_mask, bit_mask);

            __m512i x_shifted = _mm512_andnot_si512(bit_mask, _mm512_srli_epi16(x_reg, 2));
            __m512i y_shifted = _mm512_add_epi8(y_reg, y_reg);
            y_shifted = _mm512_add_epi8(y_shifted, y_shifted);

            __mmask64 mask = _mm512_cmp_epu8_mask(x_shifted, y_reg, _MM_CMPINT_NLT);
            x_reg = _mm512_sub_epi8(x_reg, _mm512_maskz_mov_epi8(mask, y_shifted));
            q_reg = _mm512_add_epi8(q_reg, q_reg);
            q_reg = _mm512_mask_add_epi8(q_reg, mask, q_reg, ones);
        }

        {   // Iteration 6
            bit_mask = _mm512_add_epi8(bit_mask, bit_mask);

            __m512i x_shifted = _mm512_andnot_si512(bit_mask, _mm512_srli_epi16(x_reg, 1));
            __m512i y_shifted = _mm512_add_epi8(y_reg, y_reg);

            __mmask64 mask = _mm512_cmp_epu8_mask(x_shifted, y_reg, _MM_CMPINT_NLT);
            x_reg = _mm512_sub_epi8(x_reg, _mm512_maskz_mov_epi8(mask, y_shifted));
            q_reg = _mm512_add_epi8(q_reg, q_reg);
            q_reg = _mm512_mask_add_epi8(q_reg, mask, q_reg, ones);
        }

        {   // Iteration 7
            __m512i x_shifted = x_reg;
            __m512i y_shifted = y_reg;

            __mmask64 mask = _mm512_cmp_epu8_mask(x_shifted, y_reg, _MM_CMPINT_NLT);
            x_reg = _mm512_sub_epi8(x_reg, _mm512_maskz_mov_epi8(mask, y_shifted));
            q_reg = _mm512_add_epi8(q_reg, q_reg);
            q_reg = _mm512_mask_add_epi8(q_reg, mask, q_reg, ones);
        }

        return vec64x8u{q_reg};
    }

    auto vec64x8u_long_division_unrolled = vector_division_test_bench<vec64x8u, vec64x8u_long_division_unrolled_impl>;

    BENCHMARK(div_8u::vec64x8u_long_division_unrolled);

    #endif



    #if defined(AVEL_AVX512BW)

    vec64x8u vec64x8u_generic_long_division_loop_impl(vec64x8u x, vec64x8u y) {
        // The lack of 8-bit shift instructions and native comparison operators means this can theoretically be made
        // better with careful attention
        vec64x8u quotient{};

        for (std::int32_t i = 8; (i-- > 0) && any(x);) {
            mask64x8u b = ((x >> i) >= y);
            x -= keep(b, y << i);
            quotient |= (vec64x8u{b} << i);
        }

        return quotient;
    }

    auto vec64x8u_generic_long_division_loop = vector_division_test_bench<vec64x8u, vec64x8u_generic_long_division_loop_impl>;

    BENCHMARK(div_8u::vec64x8u_generic_long_division_loop);

    #endif



    #if defined(AVEL_AVX512BW)

    vec64x8u vec64x8u_32f_cvt_impl(vec64x8u x, vec64x8u y) {
        __m512i x_q0 = _mm512_cvtepi8_epi32(_mm512_extracti32x4_epi32(decay(x), 0x0));
        __m512i x_q1 = _mm512_cvtepi8_epi32(_mm512_extracti32x4_epi32(decay(x), 0x1));
        __m512i x_q2 = _mm512_cvtepi8_epi32(_mm512_extracti32x4_epi32(decay(x), 0x2));
        __m512i x_q3 = _mm512_cvtepi8_epi32(_mm512_extracti32x4_epi32(decay(x), 0x3));

        __m512i y_q0 = _mm512_cvtepi8_epi32(_mm512_extracti32x4_epi32(decay(x), 0x0));
        __m512i y_q1 = _mm512_cvtepi8_epi32(_mm512_extracti32x4_epi32(decay(x), 0x1));
        __m512i y_q2 = _mm512_cvtepi8_epi32(_mm512_extracti32x4_epi32(decay(x), 0x2));
        __m512i y_q3 = _mm512_cvtepi8_epi32(_mm512_extracti32x4_epi32(decay(x), 0x3));

        __m512i quot_q0 = _mm512_cvttps_epi32(_mm512_div_ps(_mm512_castsi512_ps(x_q0), _mm512_castsi512_ps(y_q0)));
        __m512i quot_q1 = _mm512_cvttps_epi32(_mm512_div_ps(_mm512_castsi512_ps(x_q1), _mm512_castsi512_ps(y_q1)));
        __m512i quot_q2 = _mm512_cvttps_epi32(_mm512_div_ps(_mm512_castsi512_ps(x_q2), _mm512_castsi512_ps(y_q2)));
        __m512i quot_q3 = _mm512_cvttps_epi32(_mm512_div_ps(_mm512_castsi512_ps(x_q3), _mm512_castsi512_ps(y_q3)));

        __m128i quot0 = _mm512_cvtepi32_epi8(quot_q0);
        __m128i quot1 = _mm512_cvtepi32_epi8(quot_q1);
        __m128i quot2 = _mm512_cvtepi32_epi8(quot_q2);
        __m128i quot3 = _mm512_cvtepi32_epi8(quot_q3);

        __m256i quot_h0 = _mm256_inserti128_si256(_mm256_castsi128_si256(quot0), quot1, 0x01);
        __m256i quot_h1 = _mm256_inserti128_si256(_mm256_castsi128_si256(quot2), quot3, 0x01);

        __m512i quot = _mm512_inserti64x4(_mm512_castsi256_si512(quot_h0), quot_h1, 0x01);

        return vec64x8u{quot};
    }

    auto vec64x8u_32f_cvt = vector_division_test_bench<vec64x8u, vec64x8u_32f_cvt_impl>;

    BENCHMARK(div_8u::vec64x8u_32f_cvt);

    #endif



    #if defined(AVEL_AVX512BW)

    vec64x8u vec64x8u_32f_div_subnormal_unpacking_impl(vec64x8u x, vec64x8u y) {
        __m512i x_raw = decay(x);

        __m512i x_lo = _mm512_unpacklo_epi8(x_raw, _mm512_setzero_si512());
        __m512i x_hi = _mm512_unpackhi_epi8(x_raw, _mm512_setzero_si512());

        __m512i x0 = _mm512_unpacklo_epi16(x_lo, _mm512_setzero_si512());
        __m512i x1 = _mm512_unpackhi_epi16(x_lo, _mm512_setzero_si512());
        __m512i x2 = _mm512_unpacklo_epi16(x_hi, _mm512_setzero_si512());
        __m512i x3 = _mm512_unpackhi_epi16(x_hi, _mm512_setzero_si512());

        __m512i y_raw = decay(y);

        __m512i y_lo = _mm512_unpacklo_epi8(y_raw, _mm512_setzero_si512());
        __m512i y_hi = _mm512_unpackhi_epi8(y_raw, _mm512_setzero_si512());

        __m512i y_q0 = _mm512_unpacklo_epi16(y_lo, _mm512_setzero_si512());
        __m512i y_q1 = _mm512_unpackhi_epi16(y_lo, _mm512_setzero_si512());
        __m512i y_q2 = _mm512_unpacklo_epi16(y_hi, _mm512_setzero_si512());
        __m512i y_q3 = _mm512_unpackhi_epi16(y_hi, _mm512_setzero_si512());

        __m512i quot_q0 = _mm512_cvttps_epi32(_mm512_div_ps(_mm512_castsi512_ps(x0), _mm512_castsi512_ps(y_q0)));
        __m512i quot_q1 = _mm512_cvttps_epi32(_mm512_div_ps(_mm512_castsi512_ps(x1), _mm512_castsi512_ps(y_q1)));
        __m512i quot_q2 = _mm512_cvttps_epi32(_mm512_div_ps(_mm512_castsi512_ps(x2), _mm512_castsi512_ps(y_q2)));
        __m512i quot_q3 = _mm512_cvttps_epi32(_mm512_div_ps(_mm512_castsi512_ps(x3), _mm512_castsi512_ps(y_q3)));

        __m512i quot_h0 = _mm512_packus_epi32(quot_q0, quot_q1);
        __m512i quot_h1 = _mm512_packus_epi32(quot_q2, quot_q3);

        __m512i quot = _mm512_packus_epi16(quot_h0, quot_h1);
        return vec64x8u{quot};
    }

    auto vec64x8u_32f_div_subnormal_unpacking = vector_division_test_bench<vec64x8u, vec64x8u_32f_div_subnormal_unpacking_impl>;

    BENCHMARK(div_8u::vec64x8u_32f_div_subnormal_unpacking);

    #endif



    #if defined(AVEL_AVX512BW)

    vec64x8u vec64x8u_32f_div_normal_masking_impl(vec64x8u x, vec64x8u y) {
        const __m512i mask0 = _mm512_set1_epi32(0x000000ff);
        const __m512i mask1 = _mm512_set1_epi32(0x0000ff00);
        const __m512i mask2 = _mm512_set1_epi32(0x00ff0000);

        __m512i x_q0 = _mm512_and_si512(decay(x), mask0);
        __m512i x_q1 = _mm512_and_si512(decay(x), mask1);
        __m512i x_q2 = _mm512_and_si512(decay(x), mask2);
        __m512i x_q3 = _mm512_srli_epi32(decay(x), 24);

        __m512i y_q0 = _mm512_and_si512(decay(y), mask0);
        __m512i y_q1 = _mm512_and_si512(decay(y), mask1);
        __m512i y_q2 = _mm512_and_si512(decay(y), mask2);
        __m512i y_q3 = _mm512_srli_epi32(decay(y), 24);

        __m512i quot_q0 = _mm512_cvttps_epi32(_mm512_div_ps(_mm512_cvtepi32_ps(x_q0), _mm512_cvtepi32_ps(y_q0)));
        __m512i quot_q1 = _mm512_cvttps_epi32(_mm512_div_ps(_mm512_cvtepi32_ps(x_q1), _mm512_cvtepi32_ps(y_q1)));
        __m512i quot_q2 = _mm512_cvttps_epi32(_mm512_div_ps(_mm512_cvtepi32_ps(x_q2), _mm512_cvtepi32_ps(y_q2)));
        __m512i quot_q3 = _mm512_cvttps_epi32(_mm512_div_ps(_mm512_cvtepi32_ps(x_q3), _mm512_cvtepi32_ps(y_q3)));

        __m512i quot = _mm512_or_si512(
            _mm512_or_si512(
                _mm512_slli_epi32(quot_q0, 0),
                _mm512_slli_epi32(quot_q1, 8)
            ),
            _mm512_or_si512(
                _mm512_slli_epi32(quot_q2, 16),
                _mm512_slli_epi32(quot_q3, 24)
            )
        );

        return vec64x8u{quot};
    }

    auto vec64x8u_32f_div_normal_masking = vector_division_test_bench<vec64x8u, vec64x8u_32f_div_normal_masking_impl>;

    BENCHMARK(div_8u::vec64x8u_32f_div_normal_masking);

    #endif



    #if defined(AVEL_AVX512BW)

    vec64x8u vec64x8u_32f_div_normal_kmasking_impl(vec64x8u x, vec64x8u y) {
        const __mmask64 mask0 = 0x1111111111111111ull;
        const __mmask64 mask1 = 0x2222222222222222ull;
        const __mmask64 mask2 = 0x4444444444444444ull;

        __m512i x_q0 = _mm512_maskz_mov_epi8(mask0, decay(x));
        __m512i x_q1 = _mm512_maskz_mov_epi8(mask1, decay(x));
        __m512i x_q2 = _mm512_maskz_mov_epi8(mask2, decay(x));
        __m512i x_q3 = _mm512_srli_epi32(decay(x), 24);

        __m512i y_q0 = _mm512_maskz_mov_epi8(mask0, decay(y));
        __m512i y_q1 = _mm512_maskz_mov_epi8(mask1, decay(y));
        __m512i y_q2 = _mm512_maskz_mov_epi8(mask2, decay(y));
        __m512i y_q3 = _mm512_srli_epi32(decay(y), 24);

        __m512i quot_q0 = _mm512_cvttps_epi32(_mm512_div_ps(_mm512_cvtepi32_ps(x_q0), _mm512_cvtepi32_ps(y_q0)));
        __m512i quot_q1 = _mm512_cvttps_epi32(_mm512_div_ps(_mm512_cvtepi32_ps(x_q1), _mm512_cvtepi32_ps(y_q1)));
        __m512i quot_q2 = _mm512_cvttps_epi32(_mm512_div_ps(_mm512_cvtepi32_ps(x_q2), _mm512_cvtepi32_ps(y_q2)));
        __m512i quot_q3 = _mm512_cvttps_epi32(_mm512_div_ps(_mm512_cvtepi32_ps(x_q3), _mm512_cvtepi32_ps(y_q3)));

        __m512i quot = _mm512_or_si512(
            _mm512_slli_epi32(quot_q3, 24),
            _mm512_ternarylogic_epi32(
                quot_q0,
                _mm512_slli_epi32(quot_q1, 8),
                _mm512_slli_epi32(quot_q2, 16),
                0xfe
            )
        );

        return vec64x8u{quot};
    }

    auto vec64x8u_32f_div_normal_kmasking = vector_division_test_bench<vec64x8u, vec64x8u_32f_div_normal_kmasking_impl>;

    BENCHMARK(div_8u::vec64x8u_32f_div_normal_kmasking);

    #endif



    #if defined(AVEL_AVX512BW) && defined(AVEL_AVX512VBMI)

    vec64x8u vec64x8u_recip_lut_impl(vec64x8u x, vec64x8u y) {
        alignas(64) static constexpr std::uint8_t recip_hi_table_data[128] {
            0x00, 0x80, 0x55, 0x40, 0x33, 0x2a, 0x24, 0x20,
            0x1c, 0x19, 0x17, 0x15, 0x13, 0x12, 0x11, 0x10,
            0x0f, 0x0e, 0x0d, 0x0c, 0x0c, 0x0b, 0x0b, 0x0a,
            0x0a, 0x09, 0x09, 0x09, 0x08, 0x08, 0x08, 0x08,
            0x07, 0x07, 0x07, 0x07, 0x06, 0x06, 0x06, 0x06,
            0x06, 0x06, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
            0x05, 0x05, 0x05, 0x04, 0x04, 0x04, 0x04, 0x04,
            0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
            0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
            0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
            0x03, 0x03, 0x03, 0x03, 0x03, 0x02, 0x02, 0x02,
            0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
            0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
            0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
            0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
            0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02
        };

        alignas(64) static constexpr std::uint8_t recip_lo_table_data[256] {
            0x00, 0x00, 0x56, 0x00, 0x34, 0xab, 0x93, 0x00,
            0x72, 0x9a, 0x46, 0x56, 0xb2, 0x4a, 0x12, 0x00,
            0x10, 0x39, 0x7a, 0xcd, 0x31, 0xa3, 0x22, 0xab,
            0x3e, 0xd9, 0x7c, 0x25, 0xd4, 0x89, 0x43, 0x00,
            0xc2, 0x88, 0x51, 0x1d, 0xec, 0xbd, 0x91, 0x67,
            0x3f, 0x19, 0xf5, 0xd2, 0xb1, 0x91, 0x73, 0x56,
            0x3a, 0x1f, 0x06, 0xed, 0xd5, 0xbe, 0xa8, 0x93,
            0x7e, 0x6a, 0x57, 0x45, 0x33, 0x22, 0x11, 0x00,
            0xf1, 0xe1, 0xd3, 0xc4, 0xb6, 0xa9, 0x9c, 0x8f,
            0x82, 0x76, 0x6a, 0x5f, 0x54, 0x49, 0x3e, 0x34,
            0x2a, 0x20, 0x16, 0x0d, 0x04, 0xfb, 0xf2, 0xe9,
            0xe1, 0xd9, 0xd1, 0xc9, 0xc1, 0xba, 0xb2, 0xab,
            0xa4, 0x9d, 0x96, 0x90, 0x89, 0x83, 0x7d, 0x77,
            0x71, 0x6b, 0x65, 0x5f, 0x5a, 0x54, 0x4f, 0x4a,
            0x44, 0x3f, 0x3a, 0x35, 0x31, 0x2c, 0x27, 0x23,
            0x1e, 0x1a, 0x15, 0x11, 0x0d, 0x09, 0x05, 0x00,
            0xfd, 0xf9, 0xf5, 0xf1, 0xed, 0xea, 0xe6, 0xe2,
            0xdf, 0xdb, 0xd8, 0xd5, 0xd1, 0xce, 0xcb, 0xc8,
            0xc4, 0xc1, 0xbe, 0xbb, 0xb8, 0xb5, 0xb3, 0xb0,
            0xad, 0xaa, 0xa7, 0xa5, 0xa2, 0x9f, 0x9d, 0x9a,
            0x98, 0x95, 0x93, 0x90, 0x8e, 0x8b, 0x89, 0x87,
            0x84, 0x82, 0x80, 0x7e, 0x7b, 0x79, 0x77, 0x75,
            0x73, 0x71, 0x6f, 0x6d, 0x6b, 0x69, 0x67, 0x65,
            0x63, 0x61, 0x5f, 0x5d, 0x5b, 0x59, 0x58, 0x56,
            0x54, 0x52, 0x51, 0x4f, 0x4d, 0x4b, 0x4a, 0x48,
            0x47, 0x45, 0x43, 0x42, 0x40, 0x3f, 0x3d, 0x3c,
            0x3a, 0x39, 0x37, 0x36, 0x34, 0x33, 0x31, 0x30,
            0x2f, 0x2d, 0x2c, 0x2a, 0x29, 0x28, 0x26, 0x25,
            0x24, 0x22, 0x21, 0x20, 0x1f, 0x1d, 0x1c, 0x1b,
            0x1a, 0x19, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12,
            0x10, 0x0f, 0x0e, 0x0d, 0x0c, 0x0b, 0x0a, 0x09,
            0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x00
        };

        __m512i recip_hi0 = _mm512_load_si512(recip_hi_table_data +   0);
        __m512i recip_hi1 = _mm512_load_si512(recip_hi_table_data +  64);

        __m512i recip_lo0 = _mm512_load_si512(recip_lo_table_data +   0);
        __m512i recip_lo1 = _mm512_load_si512(recip_lo_table_data +  64);
        __m512i recip_lo2 = _mm512_load_si512(recip_lo_table_data + 128);
        __m512i recip_lo3 = _mm512_load_si512(recip_lo_table_data + 192);

        __m512i one = _mm512_set1_epi8(1);

        // Don't care about y == 0
        __m512i z = _mm512_sub_epi8(decay(y), one);
        __mmask64 high_bit_mask = _mm512_movepi8_mask(z);

        // Lookup low/high reciprocal halves
        __m512i recip_hi = _mm512_mask_blend_epi8(
            high_bit_mask,
            _mm512_permutex2var_epi8(recip_hi0, z, recip_hi1),
            one
        );

        __m512i recip_lo = _mm512_mask_blend_epi8(
            high_bit_mask,
            _mm512_permutex2var_epi8(recip_lo0, z, recip_lo1),
            _mm512_permutex2var_epi8(recip_lo2, z, recip_lo3)
        );

        // Construct 16-bit reciprocal
        __m512i zeros = _mm512_setzero_si512();
        __m512i reciprocals_lo = _mm512_unpacklo_epi8(recip_lo, recip_hi);
        __m512i reciprocals_hi = _mm512_unpackhi_epi8(recip_lo, recip_hi);

        __m512i x_lo = _mm512_unpacklo_epi8(decay(x), zeros);
        __m512i x_hi = _mm512_unpackhi_epi8(decay(x), zeros);

        // Compute quotient
        __m512i quotient_lo = _mm512_mulhi_epu16(x_lo, reciprocals_lo);
        __m512i quotient_hi = _mm512_mulhi_epu16(x_hi, reciprocals_hi);

        __m512i quotient = _mm512_packus_epi16(quotient_lo, quotient_hi);

        return vec64x8u{quotient};
    }

    auto vec64x8u_recip_lut = vector_division_test_bench<vec64x8u, vec64x8u_recip_lut_impl>;

    BENCHMARK(div_8u::vec64x8u_recip_lut);

    #endif

}

#endif //AVEL_DIVISION_UINT8_HPP
