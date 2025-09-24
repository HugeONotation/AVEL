#ifndef AVEL_DIV_8I_HPP
#define AVEL_DIV_8I_HPP

namespace avel::benchmarks::div_8i {

    //=====================================================
    // scalar 8i benchmarks
    //=====================================================

    std::int8_t scalar_native_impl(std::int8_t x, std::int8_t y) {
        return x / y;
    }

    auto scalar_native = scalar_division_test_bench<std::int8_t, scalar_native_impl>;

    BENCHMARK(div_8i::scalar_native);



    std::int8_t scalar_recip_lut_impl(std::int8_t x, std::int8_t y) {
        alignas(256) static constexpr std::uint32_t reciprocals[256] {
            0x00000001, 0x00010000, 0x00008000, 0x00005556, 0x00004000, 0x00003334, 0x00002aab, 0x00002493, 0x00002000, 0x00001c72, 0x0000199a, 0x00001746, 0x00001556, 0x000013b2, 0x0000124a, 0x00001112,
            0x00001000, 0x00000f10, 0x00000e39, 0x00000d7a, 0x00000ccd, 0x00000c31, 0x00000ba3, 0x00000b22, 0x00000aab, 0x00000a3e, 0x000009d9, 0x0000097c, 0x00000925, 0x000008d4, 0x00000889, 0x00000843,
            0x00000800, 0x000007c2, 0x00000788, 0x00000751, 0x0000071d, 0x000006ec, 0x000006bd, 0x00000691, 0x00000667, 0x0000063f, 0x00000619, 0x000005f5, 0x000005d2, 0x000005b1, 0x00000591, 0x00000573,
            0x00000556, 0x0000053a, 0x0000051f, 0x00000506, 0x000004ed, 0x000004d5, 0x000004be, 0x000004a8, 0x00000493, 0x0000047e, 0x0000046a, 0x00000457, 0x00000445, 0x00000433, 0x00000422, 0x00000411,
            0x00000400, 0x000003f1, 0x000003e1, 0x000003d3, 0x000003c4, 0x000003b6, 0x000003a9, 0x0000039c, 0x0000038f, 0x00000382, 0x00000376, 0x0000036a, 0x0000035f, 0x00000354, 0x00000349, 0x0000033e,
            0x00000334, 0x0000032a, 0x00000320, 0x00000316, 0x0000030d, 0x00000304, 0x000002fb, 0x000002f2, 0x000002e9, 0x000002e1, 0x000002d9, 0x000002d1, 0x000002c9, 0x000002c1, 0x000002ba, 0x000002b2,
            0x000002ab, 0x000002a4, 0x0000029d, 0x00000296, 0x00000290, 0x00000289, 0x00000283, 0x0000027d, 0x00000277, 0x00000271, 0x0000026b, 0x00000265, 0x0000025f, 0x0000025a, 0x00000254, 0x0000024f,
            0x0000024a, 0x00000244, 0x0000023f, 0x0000023a, 0x00000235, 0x00000231, 0x0000022c, 0x00000227, 0x00000223, 0x0000021e, 0x0000021a, 0x00000215, 0x00000211, 0x0000020d, 0x00000209, 0x00000205,
            0xfffffe00, 0xfffffdfb, 0xfffffdf7, 0xfffffdf3, 0xfffffdef, 0xfffffdeb, 0xfffffde6, 0xfffffde2, 0xfffffddd, 0xfffffdd9, 0xfffffdd4, 0xfffffdcf, 0xfffffdcb, 0xfffffdc6, 0xfffffdc1, 0xfffffdbc,
            0xfffffdb6, 0xfffffdb1, 0xfffffdac, 0xfffffda6, 0xfffffda1, 0xfffffd9b, 0xfffffd95, 0xfffffd8f, 0xfffffd89, 0xfffffd83, 0xfffffd7d, 0xfffffd77, 0xfffffd70, 0xfffffd6a, 0xfffffd63, 0xfffffd5c,
            0xfffffd55, 0xfffffd4e, 0xfffffd46, 0xfffffd3f, 0xfffffd37, 0xfffffd2f, 0xfffffd27, 0xfffffd1f, 0xfffffd17, 0xfffffd0e, 0xfffffd05, 0xfffffcfc, 0xfffffcf3, 0xfffffcea, 0xfffffce0, 0xfffffcd6,
            0xfffffccc, 0xfffffcc2, 0xfffffcb7, 0xfffffcac, 0xfffffca1, 0xfffffc96, 0xfffffc8a, 0xfffffc7e, 0xfffffc71, 0xfffffc64, 0xfffffc57, 0xfffffc4a, 0xfffffc3c, 0xfffffc2d, 0xfffffc1f, 0xfffffc0f,
            0xfffffc00, 0xfffffbef, 0xfffffbdd, 0xfffffbcd, 0xfffffbbb, 0xfffffba9, 0xfffffb96, 0xfffffb82, 0xfffffb6d, 0xfffffb58, 0xfffffb42, 0xfffffb2b, 0xfffffb13, 0xfffffafa, 0xfffffae1, 0xfffffac6,
            0xfffffaaa, 0xfffffa8d, 0xfffffa6f, 0xfffffa4f, 0xfffffa2e, 0xfffffa0b, 0xfffff9e7, 0xfffff9c1, 0xfffff999, 0xfffff96f, 0xfffff943, 0xfffff914, 0xfffff8e3, 0xfffff8af, 0xfffff878, 0xfffff83e,
            0xfffff800, 0xfffff7bd, 0xfffff777, 0xfffff72c, 0xfffff6db, 0xfffff684, 0xfffff627, 0xfffff5c2, 0xfffff555, 0xfffff4de, 0xfffff45d, 0xfffff3cf, 0xfffff333, 0xfffff286, 0xfffff1c7, 0xfffff0f0,
            0xfffff000, 0xffffeeee, 0xffffedb6, 0xffffec4e, 0xffffeaaa, 0xffffe8ba, 0xffffe666, 0xffffe38e, 0xffffdfff, 0xffffdb6d, 0xffffd555, 0xffffcccc, 0xffffc000, 0xffffaaaa, 0xffff8000, 0xffff0000
        };

        return (std::int32_t(x) * reciprocals[std::uint8_t(y)]) / 65536;
    }

    auto scalar_recip_lut = scalar_division_test_bench<std::int8_t, scalar_recip_lut_impl>;

    BENCHMARK(div_8i::scalar_recip_lut);



    std::int8_t scalar_recip_smaller_lut_impl(std::int8_t x, std::int8_t y) {
        alignas(128) static constexpr std::uint16_t reciprocals[128] {
            0x0000, 0x8000, 0xaaaa, 0xc000, 0xcccc, 0xd555, 0xdb6d, 0xdfff, 0xe38e, 0xe666, 0xe8ba, 0xeaaa, 0xec4e, 0xedb6, 0xeeee, 0xf000,
            0xf0f0, 0xf1c7, 0xf286, 0xf333, 0xf3cf, 0xf45d, 0xf4de, 0xf555, 0xf5c2, 0xf627, 0xf684, 0xf6db, 0xf72c, 0xf777, 0xf7bd, 0xf800,
            0xf83e, 0xf878, 0xf8af, 0xf8e3, 0xf914, 0xf943, 0xf96f, 0xf999, 0xf9c1, 0xf9e7, 0xfa0b, 0xfa2e, 0xfa4f, 0xfa6f, 0xfa8d, 0xfaaa,
            0xfac6, 0xfae1, 0xfafa, 0xfb13, 0xfb2b, 0xfb42, 0xfb58, 0xfb6d, 0xfb82, 0xfb96, 0xfba9, 0xfbbb, 0xfbcd, 0xfbdd, 0xfbef, 0xfc00,
            0xfc0f, 0xfc1f, 0xfc2d, 0xfc3c, 0xfc4a, 0xfc57, 0xfc64, 0xfc71, 0xfc7e, 0xfc8a, 0xfc96, 0xfca1, 0xfcac, 0xfcb7, 0xfcc2, 0xfccc,
            0xfcd6, 0xfce0, 0xfcea, 0xfcf3, 0xfcfc, 0xfd05, 0xfd0e, 0xfd17, 0xfd1f, 0xfd27, 0xfd2f, 0xfd37, 0xfd3f, 0xfd46, 0xfd4e, 0xfd55,
            0xfd5c, 0xfd63, 0xfd6a, 0xfd70, 0xfd77, 0xfd7d, 0xfd83, 0xfd89, 0xfd8f, 0xfd95, 0xfd9b, 0xfda1, 0xfda6, 0xfdac, 0xfdb1, 0xfdb6,
            0xfdbc, 0xfdc1, 0xfdc6, 0xfdcb, 0xfdcf, 0xfdd4, 0xfdd9, 0xfddd, 0xfde2, 0xfde6, 0xfdeb, 0xfdef, 0xfdf3, 0xfdf7, 0xfdfb, 0xfe00
        };

        std::uint8_t index = (y < 0 ? -y : y) - 1;
        std::int32_t reciprocal = 0xffff0000 | reciprocals[index];

        if (y > 0) {
            reciprocal = -reciprocal;
        }

        return (std::int32_t(x) * reciprocal) / 65536;
    }

    auto scalar_recip_smaller_lut = scalar_division_test_bench<std::int8_t, scalar_recip_smaller_lut_impl>;

    BENCHMARK(div_8i::scalar_recip_smaller_lut);



    std::int8_t scalar_f32_div_impl(std::int8_t x, std::int8_t y) {
        return std::int8_t(float(x) / float(y));
    }

    auto scalar_f32_div = scalar_division_test_bench<std::int8_t, scalar_f32_div_impl>;

    BENCHMARK(div_8i::scalar_f32_div);

    //=====================================================
    // vec16x8i benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    #endif

    //=====================================================
    // vec32x8i benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    #endif

    //=====================================================
    // vec64x8i benchmarks
    //=====================================================

    #if defined(AVEL_AVX512BW)

    //TODO: Implement
    vec64x8i vec64x8i_32f_div_normal_masking_impl(vec64x8i x, vec64x8i y) {
        const __m512i mask0 = _mm512_set1_epi32(0x000000ff);
        const __m512i mask1 = _mm512_set1_epi32(0x0000ff00);
        const __m512i mask2 = _mm512_set1_epi32(0x00ff0000);

        const __m512i ones = _mm512_set1_epi32(0xffffffff);

        __m512i x_q0 = _mm512_and_si512(decay(x), mask0);
        __m512i x_q1 = _mm512_and_si512(decay(x), mask1);
        __m512i x_q2 = _mm512_and_si512(decay(x), mask2);
        __m512i x_q3 = _mm512_srai_epi32(decay(x), 24);

        __m512i y_q0 = _mm512_and_si512(decay(y), mask0);
        __m512i y_q1 = _mm512_and_si512(decay(y), mask1);
        __m512i y_q2 = _mm512_and_si512(decay(y), mask2);
        __m512i y_q3 = _mm512_srai_epi32(decay(y), 24);

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

        return vec64x8i{quot};
    }

    auto vec64x8i_32f_div_normal_masking = vector_division_test_bench<vec64x8i, vec64x8i_32f_div_normal_masking_impl>;

    BENCHMARK(div_8i::vec64x8i_32f_div_normal_masking);

    #endif

    #if defined(AVEL_AVX512BW)

    //TODO: Test
    vec64x8i vec64x8i_32f_div_normal_kmasking_impl(vec64x8i x, vec64x8i y) {
        const __mmask64 mask0 = 0x1111111111111111ull;
        const __mmask64 mask1 = 0x2222222222222222ull;
        const __mmask64 mask2 = 0x4444444444444444ull;

        const __m512i ones = _mm512_set1_epi32(0xffffffff);

        __m512i x_q0 = _mm512_mask_mov_epi8(ones, mask0, decay(x));
        __m512i x_q1 = _mm512_mask_mov_epi8(ones, mask1, decay(x));
        __m512i x_q2 = _mm512_mask_mov_epi8(ones, mask2, decay(x));
        __m512i x_q3 = _mm512_srai_epi32(decay(x), 24);

        __m512i y_q0 = _mm512_mask_mov_epi8(ones, mask0, decay(y));
        __m512i y_q1 = _mm512_mask_mov_epi8(ones, mask1, decay(y));
        __m512i y_q2 = _mm512_mask_mov_epi8(ones, mask2, decay(y));
        __m512i y_q3 = _mm512_srai_epi32(decay(y), 24);

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

        return vec64x8i{quot};
    }

    auto vec64x8i_32f_div_normal_kmasking = vector_division_test_bench<vec64x8i, vec64x8i_32f_div_normal_kmasking_impl>;

    BENCHMARK(div_8i::vec64x8i_32f_div_normal_kmasking);

    #endif



    #if defined(AVEL_AVX512BW) && defined(AVEL_AVX512VBMI)

    vec64x8i vec64x8i_recip_lut_impl(vec64x8i x, vec64x8i y) {
        //TODO: Complete and test implementation
        alignas(128) static constexpr std::uint16_t reciprocals[128] {
            0x0000, 0x8000, 0xaaaa, 0xc000, 0xcccc, 0xd555, 0xdb6d, 0xdfff, 0xe38e, 0xe666, 0xe8ba, 0xeaaa, 0xec4e, 0xedb6, 0xeeee, 0xf000,
            0xf0f0, 0xf1c7, 0xf286, 0xf333, 0xf3cf, 0xf45d, 0xf4de, 0xf555, 0xf5c2, 0xf627, 0xf684, 0xf6db, 0xf72c, 0xf777, 0xf7bd, 0xf800,
            0xf83e, 0xf878, 0xf8af, 0xf8e3, 0xf914, 0xf943, 0xf96f, 0xf999, 0xf9c1, 0xf9e7, 0xfa0b, 0xfa2e, 0xfa4f, 0xfa6f, 0xfa8d, 0xfaaa,
            0xfac6, 0xfae1, 0xfafa, 0xfb13, 0xfb2b, 0xfb42, 0xfb58, 0xfb6d, 0xfb82, 0xfb96, 0xfba9, 0xfbbb, 0xfbcd, 0xfbdd, 0xfbef, 0xfc00,
            0xfc0f, 0xfc1f, 0xfc2d, 0xfc3c, 0xfc4a, 0xfc57, 0xfc64, 0xfc71, 0xfc7e, 0xfc8a, 0xfc96, 0xfca1, 0xfcac, 0xfcb7, 0xfcc2, 0xfccc,
            0xfcd6, 0xfce0, 0xfcea, 0xfcf3, 0xfcfc, 0xfd05, 0xfd0e, 0xfd17, 0xfd1f, 0xfd27, 0xfd2f, 0xfd37, 0xfd3f, 0xfd46, 0xfd4e, 0xfd55,
            0xfd5c, 0xfd63, 0xfd6a, 0xfd70, 0xfd77, 0xfd7d, 0xfd83, 0xfd89, 0xfd8f, 0xfd95, 0xfd9b, 0xfda1, 0xfda6, 0xfdac, 0xfdb1, 0xfdb6,
            0xfdbc, 0xfdc1, 0xfdc6, 0xfdcb, 0xfdcf, 0xfdd4, 0xfdd9, 0xfddd, 0xfde2, 0xfde6, 0xfdeb, 0xfdef, 0xfdf3, 0xfdf7, 0xfdfb, 0xfe00
        };

        constexpr __mmask64 even_kmask = 0x5555555555555555ull;
        constexpr __mmask64  odd_kmask = 0xaaaaaaaaaaaaaaaaull;

        __m512i even_vmask = _mm512_set1_epi16(0x00ff);

        __mmask64 is_y_negative = _mm512_movepi8_mask(decay(y));

        __m512i zero = _mm512_setzero_si512();
        __m512i one = _mm512_set1_epi8(0x01);
        __m512i indices = _mm512_sub_epi8(_mm512_mask_sub_epi8(decay(y), is_y_negative, zero, decay(y)), one);

        __m512i indices_even = _mm512_and_si512(even_vmask, indices);
        __m512i indices_odd  = _mm512_srli_epi16(indices, 8);

        __m512i lut_lo = _mm512_load_si512(reciprocals + 0x00);
        __m512i lut_hi = _mm512_load_si512(reciprocals + 0x40);

        __m512i reciprocal_low_halves_even = _mm512_permutex2var_epi16(lut_lo, indices_even, lut_hi);
        __m512i reciprocal_low_halves_odd  = _mm512_permutex2var_epi16(lut_lo, indices_odd,  lut_hi);

        __m512i reciprocal_even = _mm512_and_si512(even_vmask, decay(y));
        __m512i reciprocal_odd  = _mm512_srli_epi16(decay(y), 8);

        __m512i multiplicand_even = _mm512_and_si512(even_vmask, decay(x));
        __m512i multiplicand_odd  = _mm512_srli_epi16(decay(x), 8);

        //TODo: Handle case where y == 1 or y == -1 via blending

        __mmask64 is_abs_y_one = _mm512_test_epi8_mask(indices, indices);

        __m512i products_even = _mm512_mulhi_epi16(decay(x), reciprocal_even);
        __m512i products_odd  = _mm512_mulhi_epi16(decay(x), reciprocal_odd );

        __m512i quotient = _mm512_setzero_si512();
        //TODO: Assign quotient properly

        return vec64x8i{quotient};
    }

    auto vec64x8i_recip_lut = vector_division_test_bench<vec64x8i, vec64x8i_recip_lut_impl>;

    BENCHMARK(div_8i::vec64x8i_recip_lut);

    #endif

}

#endif //AVEL_DIV_8I_HPP
