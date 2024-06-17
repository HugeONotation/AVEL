#ifndef AVEL_FMOD_F64_HPP
#define AVEL_FMOD_F64_HPP

#include <cmath>

namespace avel::benchmarks::fmod_64f {

    //=====================================================
    // scalar 64f benchmarks
    //=====================================================

    double scalar_native_impl(double x, double y) {
        return std::fmod(x, y);
    }

    auto scalar_native = scalar_test_bench<double, double, scalar_native_impl>;

    BENCHMARK(fmod_64f::scalar_native);



    double scalar_u64_bit_by_bit_impl(double x, double y) {
        if (std::isnan(x) | std::isnan(y) | std::isinf(x) | y == 0.0f) {
            return NAN;
        }

        if (std::isinf(y) | (std::abs(x) < std::abs(y))) {
            return x;
        }

        // Decompose x into denominator representation
        std::uint64_t x_bits     = avel::bit_cast<std::uint64_t>(x) & 0x7fffffffffffffff;
        std::uint64_t x_sig_bits = avel::bit_cast<std::uint64_t>(x) & 0x000fffffffffffff;

        std::int64_t r_exp = 0;
        std::uint64_t r_sig = 0;

        if (x_bits >= 0x10000000000000) [[likely]] {
            //Handle decomposing normal x
            r_sig = (x_sig_bits | 0x10000000000000) << 10;
            r_exp = (x_bits >> 52) & 0x7ff;
        } else {
            //Handle decomposing subnormal x
            auto lzcnt = avel::countl_zero(x_sig_bits);
            r_sig = x_sig_bits << (lzcnt - 1);
            r_exp = 12 - lzcnt;
        }

        // Decompose y into representation of denominator
        std::uint64_t y_bits     = avel::bit_cast<std::uint64_t>(y) & 0x7fffffffffffffff;
        std::uint64_t y_sig_bits = avel::bit_cast<std::uint64_t>(y) & 0x000fffffffffffff;

        std::int64_t d_exp = 0;
        std::uint64_t d_sig = 0;

        if (y_bits >= 0x10000000000000) [[likely]] {
            //Handle decomposing normal x
            d_sig = (y_sig_bits | 0x10000000000000) << 10;
            d_exp = (y_bits >> 52) & 0x7ff;
        } else {
            //Handle decomposing subnormal x
            auto lzcnt = avel::countl_zero(y_sig_bits);
            d_sig = y_sig_bits << (lzcnt - 1);
            d_exp = 12 - lzcnt;
        }

        // Run repeated subtraction
        while ((r_exp > d_exp) & bool(r_sig)) {
            if (r_sig >= d_sig) {
                r_sig -= d_sig;
            }

            r_sig <<= 1;
            r_exp -= 1;
        }

        if (r_sig >= d_sig) {
            r_sig -= d_sig;
        }

        std::uint64_t result_bits = std::uint64_t(std::signbit(x)) << 63;

        if (r_sig) [[likely]] {
            std::uint32_t t = avel::countl_zero(r_sig);
            r_sig <<= t;
            r_exp -= (t - 1);

            if (r_exp > 0) [[likely]] {
                // Compose normal result
                result_bits = (r_exp << 52) | ((r_sig >> 11) & 0xfffffffffffff);
            } else {
                // Compose subnormal result
                result_bits = (r_sig >> (12 - r_exp));
            }
        }

        double result = avel::bit_cast<double>(result_bits);
        double ret = avel::copysign(result, x);
        return ret;
    }

    auto scalar_u64_bit_by_bit = scalar_test_bench<double, double, scalar_u64_bit_by_bit_impl>;

    BENCHMARK(fmod_64f::scalar_u64_bit_by_bit);




    double scalar_f64_bit_by_bit_impl(double x, double y) {
        double x_orig = x;
        //TODO: Properly handle subnormal inputs

        if (avel::isnan(x) | avel::isnan(y) | avel::isinf(x) | y == 0.0) {
            return NAN;
        }

        x = std::abs(x);
        y = std::abs(y);

        if (avel::isinf(y) | (x < y)) {
            return x;
        }

        // Incorrect handling of subnormal values:
        if ((y < 2.22507385850720138309e-308) | (x < 2.22507385850720138309e-308)) {
            return x;
        }

        // Copy exponent from x to y and subtract one to produce d
        std::uint64_t exponent_field = 0x7ff0000000000000;
        std::uint64_t x_bits = avel::bit_cast<std::uint64_t>(x);
        std::uint64_t y_bits = avel::bit_cast<std::uint64_t>(y);

        double d = 0.0f;

        if (y < 2.22507385850720138309e-308) {
            d = y;
            while (d < 2.22507385850720138309e-308 && d < x) {
                d *= 2.0;
            }

            if (d > x) {
                d *= 0.5;
            }
        } else {
            std::uint64_t d_bits = (y_bits & ~exponent_field) | (x_bits & exponent_field);
            d = avel::bit_cast<double>(d_bits);
        }

        // Core loop
        while (x >= y) {
            if (x >= d) {
                x -= d;
            }

            d *= 0.5;
        }

        x = std::copysign(x, x_orig);

        return x;
    }

    auto scalar_f64_bit_by_bit = scalar_test_bench<double, double, scalar_f64_bit_by_bit_impl>;

    BENCHMARK(fmod_64f::scalar_f64_bit_by_bit);



    double scalar_u64_division_impl(double x, double y) {
        if (avel::isnan(x) | avel::isnan(y) | avel::isinf(x) | y == 0.) {
            return NAN;
        }

        if (avel::isinf(y) | (avel::abs(x) < avel::abs(y))) {
            return x;
        }

        // Decompose x into denominator representation
        std::uint64_t x_bits     = avel::bit_cast<std::uint64_t>(x) & 0x7fffffffffffffff;
        std::uint64_t x_sig_bits = avel::bit_cast<std::uint64_t>(x) & 0x000fffffffffffff;

        std::int64_t r_exp = 0;
        std::uint64_t r_sig = 0;

        if (x_bits >= 0x10000000000000) {
            [[likely]]
            //Handle decomposing normal x
            r_sig = (x_sig_bits | 0x10000000000000) << 11;
            r_exp = (x_bits >> 52) & 0x7ff;
        } else {
            //Handle decomposing subnormal x
            auto lzcnt = avel::countl_zero(x_sig_bits);
            r_sig = x_sig_bits << lzcnt;
            r_exp = 12 - lzcnt;
        }

        // Decompose y into representation of denominator
        std::uint64_t y_bits     = avel::bit_cast<std::uint64_t>(y) & 0x7fffffffffffffff;
        std::uint64_t y_sig_bits = avel::bit_cast<std::uint64_t>(y) & 0x000fffffffffffff;

        std::int64_t d_exp = 0;
        std::uint64_t d_sig = 0;

        if (y_bits >= 0x10000000000000) {
            [[likely]]
            //Handle decomposing normal x
            d_sig = (y_sig_bits | 0x10000000000000) << 11;
            d_exp = (y_bits >> 52) & 0x7ff;
        } else {
            //Handle decomposing subnormal x
            auto lzcnt = avel::countl_zero(y_sig_bits);
            d_sig = y_sig_bits << lzcnt;
            d_exp = 12 - lzcnt;
        }

        std::int64_t d_sig_width = 64 - avel::countr_zero(d_sig);
        std::uint64_t d_sig0 = d_sig >> avel::countr_zero(d_sig);

        while ((r_exp - d_exp + d_sig_width > 64) & bool(r_sig)) {
            std::uint64_t quotient = r_sig / d_sig0;
            std::uint64_t subtracthend = quotient * d_sig0;

            r_sig -= subtracthend;

            std::uint32_t lzcnt = avel::countl_zero(r_sig);
            r_sig <<= lzcnt;
            r_exp -= lzcnt;
        }

        if (r_exp > d_exp || (r_exp == d_exp && r_sig > d_sig)) {
            std::uint64_t d_width = avel::bit_width(d_sig0);
            std::uint64_t d = d_sig0 << (64 - (r_exp - d_exp) - d_width);

            std::uint64_t quotient = r_sig / d;
            std::uint64_t subtracthend = quotient * d;

            r_sig -= subtracthend;

            std::uint32_t lzcnt = avel::countl_zero(r_sig);
            r_sig <<= lzcnt;
            r_exp -= lzcnt;
        }

        std::uint64_t result_bits = 0;

        if (r_exp > 0) {
            [[likely]]
            // Compose normal result
            result_bits = (r_exp << 52) | ((r_sig >> 11) & 0x000fffffffffffff);
        } else {
            // Compose subnormal result
            result_bits = (r_sig >> (12 - r_exp));
        }

        if (r_sig == 0) {
            result_bits = 0;
        }

        double result = avel::bit_cast<double>(result_bits);
        double ret = avel::copysign(result, x);
        return ret;
    }

    auto scalar_u64_division = scalar_test_bench<double, double, scalar_u64_division_impl>;

    BENCHMARK(fmod_64f::scalar_u64_division);



    double scalar_denom64u_impl(double x, double y) {
        if (avel::isnan(x) | avel::isnan(y) | avel::isinf(x) | y == 0.) {
            return NAN;
        }

        if (avel::isinf(y) | (avel::abs(x) < avel::abs(y))) {
            return x;
        }

        // Decompose x into denominator representation
        std::uint64_t x_bits     = avel::bit_cast<std::uint64_t>(x) & 0x7fffffffffffffff;
        std::uint64_t x_sig_bits = avel::bit_cast<std::uint64_t>(x) & 0x000fffffffffffff;

        std::int64_t r_exp = 0;
        std::uint64_t r_sig = 0;

        if (x_bits >= 0x10000000000000) {
            [[likely]]
            //Handle decomposing normal x
            r_sig = (x_sig_bits | 0x10000000000000) << 11;
            r_exp = (x_bits >> 52) & 0x7ff;
        } else {
            //Handle decomposing subnormal x
            auto lzcnt = avel::countl_zero(x_sig_bits);
            r_sig = x_sig_bits << lzcnt;
            r_exp = 12 - lzcnt;
        }

        // Decompose y into representation of denominator
        std::uint64_t y_bits     = avel::bit_cast<std::uint64_t>(y) & 0x7fffffffffffffff;
        std::uint64_t y_sig_bits = avel::bit_cast<std::uint64_t>(y) & 0x000fffffffffffff;

        std::int64_t d_exp = 0;
        std::uint64_t d_sig = 0;

        if (y_bits >= 0x10000000000000) {
            [[likely]]
            //Handle decomposing normal x
            d_sig = (y_sig_bits | 0x10000000000000) << 11;
            d_exp = (y_bits >> 52) & 0x7ff;
        } else {
            //Handle decomposing subnormal x
            auto lzcnt = avel::countl_zero(y_sig_bits);
            d_sig = y_sig_bits << lzcnt;
            d_exp = 12 - lzcnt;
        }

        std::int64_t d_sig_width = 64 - avel::countr_zero(d_sig);
        std::uint64_t d_sig0 = d_sig >> avel::countr_zero(d_sig);

        avel::Denom64u d_denom{d_sig0};
        while ((r_exp - d_exp + d_sig_width > 64) & bool(r_sig)) {
            std::uint64_t quotient = r_sig / d_denom;
            std::uint64_t subtracthend = quotient * d_sig0;

            r_sig -= subtracthend;

            std::uint32_t lzcnt = avel::countl_zero(r_sig);
            r_sig <<= lzcnt;
            r_exp -= lzcnt;
        }

        if (r_exp > d_exp || (r_exp == d_exp && r_sig > d_sig)) {
            std::uint64_t d_width = avel::bit_width(d_sig0);
            std::uint64_t d = d_sig0 << (64 - (r_exp - d_exp) - d_width);

            std::uint64_t quotient = r_sig / d;
            std::uint64_t subtracthend = quotient * d;

            r_sig -= subtracthend;

            std::uint32_t lzcnt = avel::countl_zero(r_sig);
            r_sig <<= lzcnt;
            r_exp -= lzcnt;
        }

        std::uint64_t result_bits = 0;

        if (r_exp > 0) {
            [[likely]]
            // Compose normal result
            result_bits = (r_exp << 52) | ((r_sig >> 11) & 0x000fffffffffffff);
        } else {
            // Compose subnormal result
            result_bits = (r_sig >> (12 - r_exp));
        }

        if (r_sig == 0) {
            result_bits = 0;
        }

        double result = avel::bit_cast<double>(result_bits);
        double ret = avel::copysign(result, x);
        return ret;
    }

    auto scalar_denom64u = scalar_test_bench<double, double, scalar_denom64u_impl>;

    BENCHMARK(fmod_64f::scalar_denom64u);

    //=====================================================
    // vec2x64f benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    vec2x64f vec2x64f_bit_by_bit_64f_impl(vec2x64f x, vec2x64f y) {
        vec2x64f x_orig = x;

        mask2x64f is_result_nan = avel::isnan(x) | avel::isnan(y) | avel::isinf(x) | y == vec2x64f{0.0f};

        x = avel::abs(x);
        y = avel::abs(y);

        vec2x64i dummy;
        //TODO: Use AVX-512 getmant to get matnissa in range of [1.0, 2.0) to avoid need to add one
        vec2x64f t = avel::ldexp(avel::frexp(y, &dummy), avel::ilogb(x) + vec2x64i{1});

        // Core loop
        mask2x64f m = !is_result_nan & (x >= y);
        while (avel::any(m)) {
            x -= keep(m & (x >= t), t);
            t *= vec2x64f{0.5f};
            m &= (x >= y);
        }

        x = avel::copysign(x, x_orig);
        x = avel::blend(is_result_nan, vec2x64f{NAN}, x);

        return x;
    }

    auto vec2x64f_bit_by_bit_64f = vector_test_bench<vec2x64f, vec2x64f, vec2x64f_bit_by_bit_64f_impl>;

    BENCHMARK(fmod_64f::vec2x64f_bit_by_bit_64f);

    #endif



    #if defined(AVEL_AVX512DQ)



    #endif

    //=====================================================
    // vec4x64f benchmarks
    //=====================================================

    #if defined(AVEL_AVX2)

    vec4x64f vec4x64f_bit_by_bit_64f_impl(vec4x64f x, vec4x64f y) {
        vec4x64f x_orig = x;

        mask4x64f is_result_nan = avel::isnan(x) | avel::isnan(y) | avel::isinf(x) | y == vec4x64f{0.0f};

        x = avel::abs(x);
        y = avel::abs(y);

        vec4x64i dummy;
        //TODO: Use AVX-512 getmant to get matnissa in range of [1.0, 2.0) to avoid need to add one
        vec4x64f t = avel::ldexp(avel::frexp(y, &dummy), avel::ilogb(x) + vec4x64i{1});

        // Core loop
        mask4x64f m = !is_result_nan & (x >= y);
        while (avel::any(m)) {
            x -= keep(m & (x >= t), t);
            t *= vec4x64f{0.5f};
            m &= (x >= y);
        }

        x = avel::copysign(x, x_orig);
        x = avel::blend(is_result_nan, vec4x64f{NAN}, x);

        return x;
    }

    auto vec4x64f_bit_by_bit_64f = vector_test_bench<vec4x64f, vec4x64f, vec4x64f_bit_by_bit_64f_impl>;

    BENCHMARK(fmod_64f::vec4x64f_bit_by_bit_64f);

    #endif



    #if defined(AVEL_AVX512DQ)



    #endif

    //=====================================================
    // vec8x64f benchmarks
    //=====================================================

    #if defined(AVEL_AVX512F)

    vec8x64f vec8x64f_bit_by_bit_64f_impl(vec8x64f x, vec8x64f y) {
        vec8x64f x_orig = x;

        mask8x64f is_result_nan = avel::isnan(x) | avel::isnan(y) | avel::isinf(x) | y == vec8x64f{0.0f};

        x = avel::abs(x);
        y = avel::abs(y);

        vec8x64i dummy;
        //TODO: Use AVX-512 getmant to get matnissa in range of [1.0, 2.0) to avoid need to add one
        vec8x64f t = avel::ldexp(avel::frexp(y, &dummy), avel::ilogb(x) + vec8x64i{1});

        // Core loop
        mask8x64f m = !is_result_nan & (x >= y);
        while (avel::any(m)) {
            x -= keep(m & (x >= t), t);
            t *= vec8x64f{0.5f};
            m &= (x >= y);
        }

        x = avel::copysign(x, x_orig);
        x = avel::blend(is_result_nan, vec8x64f{NAN}, x);

        return x;
    }

    auto vec8x64f_bit_by_bit_64f = vector_test_bench<vec8x64f, vec8x64f, vec8x64f_bit_by_bit_64f_impl>;

    BENCHMARK(fmod_64f::vec8x64f_bit_by_bit_64f);

    #endif

    /*
    #if defined(AVEL_AVX512F)

    vec8x64f vec8x64f_bit_by_bit_64f_impl(vec8x64f x, vec8x64f y) {
        __m512d abs_x = _mm512_abs_pd(decay(x));
        __m512d abs_y = _mm512_abs_pd(decay(y));

        __m512d infinity = _mm512_set1_pd(INFINITY);
        __m512d zero = _mm512_setzero_pd();

        __mmask8 is_result_nan = _kor_mask8(
            _kor_mask8(_mm512_cmpeq_pd_mask(infinity, abs_x), _mm512_cmpneq_pd_mask(decay(x), decay(x))),
            _kor_mask8(_mm512_cmpeq_pd_mask(zero,     abs_y), _mm512_cmpneq_pd_mask(decay(y), decay(y)))
        );

        __m512d neg_one = _mm512_set1_pd(-1.0);

        __m512d exp_diff = _mm512_sub_pd(_mm512_getexp_pd(decay(x)), _mm512_getexp_pd(decay(y)));
        __m512d d = _mm512_scalef_pd(abs_y, _mm512_max_pd(exp_diff, _mm512_setzero_pd()));
        d = _mm512_mask_scalef_pd(d, _mm512_cmp_pd_mask(abs_x, d, _CMP_LT_OQ), d, neg_one);

        __mmask8 m = ~is_result_nan & _mm512_cmp_pd_mask(abs_y, abs_x, _CMP_LT_OQ);
        while (m) {
            __mmask8 cmp_result = _mm512_mask_cmp_pd_mask(m, d, abs_x, _CMP_LT_OQ);
            abs_x = _mm512_mask_sub_pd(abs_x, cmp_result, abs_x, d);
            d = _mm512_mask_scalef_pd(d, m, d, neg_one);

            m = _mm512_mask_cmp_pd_mask(m, abs_y, abs_x, _CMP_LT_OQ);
        }

        // Perform copy sign operation
        auto ret = _mm512_castsi512_pd(
            _mm512_ternarylogic_epi32(
                _mm512_castpd_si512(decay(x)),
                _mm512_castpd_si512(abs_x),
                _mm512_set1_epi64(double_sign_bit_mask_bits),
                0xe4
            )
        );

        // Conditionally make result nan
        ret = _mm512_mask_blend_pd(is_result_nan, ret, _mm512_set1_pd(NAN));

        return vec8x64f{ret};
    }

    auto vec8x64f_bit_by_bit_64f = vector_test_bench<vec8x64f, vec8x64f, vec8x64f_bit_by_bit_64f_impl>;

    BENCHMARK(fmod_64f::vec8x64f_bit_by_bit_64f);

    #endif
    */


    #if defined(AVEL_AVX512DQ)

    vec8x64f vec8x64f_bit_by_bit_64f_avx512dq_impl(vec8x64f x, vec8x64f y) {
        __mmask8 is_result_nan = _kor_mask8(
            _mm512_fpclass_pd_mask(decay(x), 0x99),
            _mm512_fpclass_pd_mask(decay(y), 0x87)
        );

        __m512d abs_x = _mm512_abs_pd(decay(x));
        __m512d abs_y = _mm512_abs_pd(decay(y));

        __m512d neg_one = _mm512_set1_pd(-1.0);

        __m512d exp_diff = _mm512_sub_pd(_mm512_getexp_pd(decay(x)), _mm512_getexp_pd(decay(y)));
        __m512d d = _mm512_scalef_pd(abs_y, _mm512_max_pd(exp_diff, _mm512_setzero_pd()));
        d = _mm512_mask_scalef_pd(d, _mm512_cmp_pd_mask(abs_x, d, _CMP_LT_OQ), d, neg_one);

        __mmask8 m = ~is_result_nan & _mm512_cmp_pd_mask(abs_y, abs_x, _CMP_LT_OQ);
        while (m) {
            __mmask8 cmp_result = _mm512_mask_cmp_pd_mask(m, d, abs_x, _CMP_LT_OQ);
            abs_x = _mm512_mask_sub_pd(abs_x, cmp_result, abs_x, d);
            d = _mm512_mask_scalef_pd(d, m, d, neg_one);

            m = _mm512_mask_cmp_pd_mask(m, abs_y, abs_x, _CMP_LT_OQ);
        }

        // Perform copy sign operation
        auto ret = _mm512_range_pd(decay(x), abs_x, 0x02);

        // Conditionally make result nan
        ret = _mm512_mask_blend_pd(is_result_nan, ret, _mm512_set1_pd(NAN));

        return vec8x64f{ret};
    }

    auto vec8x64f_bit_by_bit_64f_avx512dq = vector_test_bench<vec8x64f, vec8x64f, vec8x64f_bit_by_bit_64f_avx512dq_impl>;

    BENCHMARK(fmod_64f::vec8x64f_bit_by_bit_64f_avx512dq);

    #endif


    #if defined(AVEL_AVX512DQ) && defined(AVEL_PCLMULQDQ)

    vec8x64f vec8x64f_denom64u_avx512dq_impl(vec8x64f x, vec8x64f y) {
        //TODO: Test correctness
        __mmask8 is_result_nan = _kor_mask16(
            _mm512_fpclass_pd_mask(decay(x), 0x99),
            _mm512_fpclass_pd_mask(decay(y), 0x87)
        );

        // Decompose x into exponent and significand
        __mmask8 is_x_edge_case = _mm512_fpclass_pd_mask(decay(x), 0x9f);
        __m512i r_exp = _mm512_mask_cvtpd_epi64(_mm512_set1_epi64(-1100), ~is_x_edge_case, _mm512_getexp_pd(decay(x)));

        __m512d x_sig = _mm512_getmant_pd(decay(x), _MM_MANT_NORM_1_2, _MM_MANT_SIGN_zero);
        __m512i r_sig = _mm512_maskz_srli_epi32(~is_x_edge_case, _mm512_slli_epi32(_mm512_castpd_si512(x_sig), 11), 1);


        // Decompose y into exponent and significand
        __mmask16 is_y_edge_case = _mm512_fpclass_pd_mask(decay(y), 0x9f);
        __m512i d_exp = _mm512_mask_cvtpd_epi64(_mm512_set1_epi64(1100), ~is_y_edge_case, _mm512_getexp_pd(decay(y)));

        __m512d y_sig = _mm512_getmant_pd(decay(y), _MM_MANT_NORM_1_2, _MM_MANT_SIGN_zero);
        __m512i d_sig = _mm512_maskz_srli_epi64(~is_y_edge_case, _mm512_slli_epi64(_mm512_castpd_si512(y_sig), 11), 1);

        // Ensure that d_sig does not have lanes equal to zero
        d_sig = _mm512_mask_blend_epi64(is_result_nan, d_sig, _mm512_set1_epi64(0x01));

        // Execute long division via repeated subtraction
        avel::Denom8x64u d_denom{vec8x64u{d_sig}};
        //TODO: Consider using 64i denominator instead since the multiplication is more efficient

        __m512i threshold = _mm512_add_epi64(decay(countr_zero(vec8x64u{d_sig})), d_exp);
        __mmask8 loop_mask =
            _mm512_cmp_epi64_mask(r_exp, threshold, _MM_CMPINT_NLE) &
            _mm512_test_epi64_mask(r_sig, r_sig);
        while (loop_mask) {
            vec8x64u quotient = vec8x64u{r_sig} / d_denom;
            vec8x64u subtrahend = quotient * d_denom.value();

            r_sig = _mm512_mask_sub_epi64(r_sig, loop_mask, r_sig, decay(subtrahend));

            __m512i lzcnt = _mm512_lzcnt_epi64(r_sig);
            r_sig = _mm512_mask_sllv_epi64(r_sig, loop_mask, r_sig, lzcnt);
            r_exp = _mm512_mask_sub_epi64(r_exp, loop_mask, r_exp, lzcnt);

            auto a = _mm512_cmp_epi64_mask(r_exp, threshold, _MM_CMPINT_NLE);
            auto b = _mm512_test_epi64_mask(r_sig, r_sig);
            loop_mask = a & b;
        }

        {
            auto m =
                _mm512_cmp_epi64_mask(r_exp, d_exp, _MM_CMPINT_NLE) |
                (
                    _mm512_cmp_epi64_mask(r_exp, d_exp, _MM_CMPINT_EQ) &
                    _mm512_cmp_epi64_mask(r_sig, d_sig, _MM_CMPINT_NLE)
                );

            vec8x64u d_width = bit_width(vec8x64u{d_sig});
            vec8x64u pos_offset = (vec8x64u{64} - (vec8x64u{r_exp} - vec8x64u{d_exp}) - d_width);
            d_denom <<= pos_offset;

            vec8x64u quotient = vec8x64u{r_sig} / d_denom;
            vec8x64u subtrahend = quotient * d_denom.value();

            r_sig = _mm512_mask_sub_epi64(r_sig, m, r_sig, decay(subtrahend));

            __m512i lzcnt = _mm512_lzcnt_epi64(r_sig);
            r_sig = _mm512_mask_sllv_epi64(r_sig, m, r_sig, lzcnt);
            r_exp = _mm512_mask_sub_epi64(r_exp, m, r_exp, lzcnt);
        }

        // Reconstruct result
        __m512d ret = _mm512_range_pd(decay(x), _mm512_setzero_pd(), 0x02);
        ret = _mm512_or_pd(ret, _mm512_cvtepi64_pd(r_sig));

        __m512i exponent_offset = _mm512_sub_epi64(r_exp, _mm512_set1_epi64(62));
        auto tmp = _mm512_cvtepi64_pd(exponent_offset);
        ret = _mm512_scalef_pd(ret, tmp);

        ret = _mm512_mask_blend_pd(is_result_nan, ret, _mm512_set1_pd(NAN));
        return vec8x64f{ret};
    }

    auto vec8x64f_denom64u_avx512dq = vector_test_bench<vec8x64f, vec8x64f, vec8x64f_denom64u_avx512dq_impl>;

    BENCHMARK(fmod_64f::vec8x64f_denom64u_avx512dq);

    #endif

}

#endif //AVEL_FMOD_F64_HPP
