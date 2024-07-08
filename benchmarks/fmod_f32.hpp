#ifndef AVEL_FMOD_F32_HPP
#define AVEL_FMOD_F32_HPP

#include <cmath>
#include <array>

namespace avel::benchmarks::fmod_32f {

    //=====================================================
    // scalar f32 benchmarks
    //=====================================================

    float scalar_native_impl(float x, float y) {
        return std::fmod(x, y);
    }

    auto scalar_native = scalar_test_bench<float, float, scalar_native_impl>;

    BENCHMARK(fmod_32f::scalar_native);



    float scalar_u32_bit_by_bit_impl(float x, float y) {
        if (std::isnan(x) | std::isnan(y) | std::isinf(x) | y == 0.0f) {
            return NAN;
        }

        if (std::isinf(y) | (std::abs(x) < std::abs(y))) {
            return x;
        }

        // Decompose x into denominator representation
        std::uint32_t x_bits     = avel::bit_cast<std::uint32_t>(x) & 0x7fffffff;
        std::uint32_t x_sig_bits = avel::bit_cast<std::uint32_t>(x) & 0x007fffff;

        std::int32_t r_exp = 0;
        std::uint32_t r_sig = 0;

        if (x_bits >= 0x800000) [[likely]] {
            //Handle decomposing normal x
            r_sig = (x_sig_bits | 0x800000) << 7;
            r_exp = (x_bits >> 23) & 0xff;
        } else {
            //Handle decomposing subnormal x
            auto lzcnt = avel::countl_zero(x_sig_bits);
            r_sig = x_sig_bits << (lzcnt - 1);
            r_exp = 9 - lzcnt;
        }

        // Decompose y into representation of denominator
        std::uint32_t y_bits     = avel::bit_cast<std::uint32_t>(y) & 0x7fffffff;
        std::uint32_t y_sig_bits = avel::bit_cast<std::uint32_t>(y) & 0x007fffff;

        std::int32_t d_exp = 0;
        std::uint32_t d_sig = 0;

        if (y_bits >= 0x800000) [[likely]] {
            //Handle decomposing normal x
            d_sig = (y_sig_bits | 0x800000) << 7;
            d_exp = (y_bits >> 23) & 0xff;
        } else {
            //Handle decomposing subnormal x
            auto lzcnt = avel::countl_zero(y_sig_bits);
            d_sig = y_sig_bits << (lzcnt - 1);
            d_exp = 9 - lzcnt;
        }

        // Execute long division via repeated subtraction
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

        // Reconstruct result
        std::uint32_t result_bits = std::signbit(x) << 31;

        if (r_sig) [[likely]] {
            std::uint32_t t = avel::countl_zero(r_sig);
            r_sig <<= t;
            r_exp -= (t - 1);

            if (r_exp > 0) [[likely]] {
                // Compose normal result
                result_bits = (r_exp << 23) | ((r_sig >> 8) & 0x07fffff);
            } else {
                // Compose subnormal result
                result_bits = (r_sig >> (9 - r_exp));
            }
        }

        float result = avel::bit_cast<float>(result_bits);
        float ret = std::copysign(result, x);
        return ret;
    }

    auto scalar_u32_bit_by_bit = scalar_test_bench<float, float, scalar_u32_bit_by_bit_impl>;

    BENCHMARK(fmod_32f::scalar_u32_bit_by_bit);




    float scalar_32f_bit_by_bit_impl(float x, float y) {
        float x_orig = x;

        if (avel::isnan(x) | avel::isnan(y) | avel::isinf(x) | y == 0.0f) {
            return NAN;
        }

        x = std::abs(x);
        y = std::abs(y);

        if (avel::isinf(y) | (x < y)) {
            return x;
        }

        int dummy = 0;
        float t = std::ldexp(std::frexp(y, &dummy), std::ilogb(x) + 1);

        // Core loop
        while (x >= y) {
            if (x >= t) {
                x -= t;
            }

            t *= 0.5f;
        }

        x = std::copysign(x, x_orig);

        return x;
    }

    auto scalar_32f_bit_by_bit = scalar_test_bench<float, float, scalar_32f_bit_by_bit_impl>;

    BENCHMARK(fmod_32f::scalar_32f_bit_by_bit);



    float scalar_u32_division_impl(float x, float y) {
        if (avel::isnan(x) | avel::isnan(y) | avel::isinf(x) | y == 0.0f) {
            return NAN;
        }

        if (avel::isinf(y) | (avel::abs(x) < avel::abs(y))) {
            return x;
        }

        // Decompose x into denominator representation
        std::uint32_t x_bits     = avel::bit_cast<std::uint32_t>(x) & 0x7fffffff;
        std::uint32_t x_sig_bits = avel::bit_cast<std::uint32_t>(x) & 0x007fffff;

        std::int32_t r_exp = 0;
        std::uint32_t r_sig = 0;

        if (x_bits >= 0x800000) [[likely]] {
            //Handle decomposing normal x
            r_sig = (x_sig_bits | 0x800000) << 8;
            r_exp = (x_bits >> 23) & 0xff;
        } else {
            //Handle decomposing subnormal x
            auto lzcnt = avel::countl_zero(x_sig_bits);
            r_sig = x_sig_bits << lzcnt;
            r_exp = 9 - lzcnt;
        }

        // Decompose y into representation of denominator
        std::uint32_t y_bits     = avel::bit_cast<std::uint32_t>(y) & 0x7fffffff;
        std::uint32_t y_sig_bits = avel::bit_cast<std::uint32_t>(y) & 0x007fffff;

        std::int32_t d_exp = 0;
        std::uint32_t d_sig = 0;

        if (y_bits >= 0x800000) [[likely]] {
            //Handle decomposing normal x
            d_sig = (y_sig_bits | 0x800000) << 8;
            d_exp = (y_bits >> 23) & 0xff;
        } else {
            //Handle decomposing subnormal x
            auto lzcnt = avel::countl_zero(y_sig_bits);
            d_sig = y_sig_bits << lzcnt;
            d_exp = 9 - lzcnt;
        }

        std::uint32_t d_sig0 = d_sig >> avel::countr_zero(d_sig);

        std::int32_t threshold = avel::countr_zero(d_sig) + d_exp;
        while ((r_exp > threshold) & bool(r_sig)) {
            std::uint32_t quotient = r_sig / d_sig0;
            std::uint32_t subtrahend = quotient * d_sig0;

            r_sig -= subtrahend;

            std::uint32_t lzcnt = avel::countl_zero(r_sig);
            r_sig <<= lzcnt;
            r_exp -= lzcnt;
        }

        if (r_exp > d_exp || (r_exp == d_exp && r_sig > d_sig)) {
            std::uint32_t d_width = avel::bit_width(d_sig0);
            std::uint32_t d = d_sig0 << (32 - (r_exp - d_exp) - d_width);

            std::uint32_t quotient = r_sig / d;
            std::uint32_t subtrahend = quotient * d;

            r_sig -= subtrahend;

            std::uint32_t lzcnt = avel::countl_zero(r_sig);
            r_sig <<= lzcnt;
            r_exp -= lzcnt;
        }

        std::uint32_t result_bits = 0;

        if (r_exp > 0) [[likely]] {
            // Compose normal result
            result_bits = (r_exp << 23) | ((r_sig >> 8) & 0x07fffff);
        } else {
            // Compose subnormal result
            result_bits = (r_sig >> (9 - r_exp));
        }

        if (r_sig == 0) {
            result_bits = 0;
        }

        float result = avel::bit_cast<float>(result_bits);
        float ret = avel::copysign(result, x);
        return ret;
    }

    auto scalar_u32_division = scalar_test_bench<float, float, scalar_u32_division_impl>;

    BENCHMARK(fmod_32f::scalar_u32_division);



    float scalar_u64_division_impl(float x, float y) {
        if (avel::isnan(x) | avel::isnan(y) | avel::isinf(x) | y == 0.0f) {
            return NAN;
        }

        if (avel::isinf(y) | (avel::abs(x) < avel::abs(y))) {
            return x;
        }

        // Decompose x into denominator representation
        std::uint32_t x_bits     = avel::bit_cast<std::uint32_t>(x) & 0x7fffffff;
        std::uint32_t x_sig_bits = avel::bit_cast<std::uint32_t>(x) & 0x007fffff;

        std::int32_t r_exp = 0;
        std::uint64_t r_sig = 0;

        if (x_bits >= 0x800000) [[likely]] {
            //Handle decomposing normal x
            r_sig = std::uint64_t(x_sig_bits | 0x800000) << (32 + 8);
            r_exp = (x_bits >> 23) & 0xff;
        } else {
            //Handle decomposing subnormal x
            auto lzcnt = avel::countl_zero(x_sig_bits);
            r_sig = std::uint64_t(x_sig_bits) << (lzcnt + 32);
            r_exp = 9 - lzcnt;
        }

        // Decompose y into representation of denominator
        std::uint32_t y_bits     = avel::bit_cast<std::uint32_t>(y) & 0x7fffffff;
        std::uint32_t y_sig_bits = avel::bit_cast<std::uint32_t>(y) & 0x007fffff;

        std::int32_t d_exp = 0;
        std::uint64_t d_sig = 0;

        if (y_bits >= 0x800000) [[likely]] {
            //Handle decomposing normal x
            d_sig = std::uint64_t(y_sig_bits | 0x800000) << (32 + 8);
            d_exp = (y_bits >> 23) & 0xff;
        } else {
            //Handle decomposing subnormal x
            auto lzcnt = avel::countl_zero(y_sig_bits);
            d_sig = std::uint64_t(y_sig_bits) << (lzcnt + 32);
            d_exp = 9 - lzcnt;
        }

        std::int32_t d_sig_width = 64 - avel::countr_zero(d_sig);
        std::uint64_t d_sig0 = d_sig >> avel::countr_zero(d_sig);

        std::int32_t threshold = 64 - d_sig_width + d_exp;

        while ((r_exp > threshold) & bool(r_sig)) {
            std::uint64_t quotient = r_sig / d_sig0;
            std::uint64_t subtrahend = quotient * d_sig0;

            r_sig -= subtrahend;

            std::uint32_t lzcnt = avel::countl_zero(r_sig);
            r_sig <<= lzcnt;
            r_exp -= lzcnt;
        }

        if (r_exp > d_exp || (r_exp == d_exp && r_sig > d_sig)) {
            std::uint32_t s = d_exp - r_exp + avel::countl_zero(d_sig0);
            std::uint64_t d = d_sig0 << s;

            std::uint64_t quotient = r_sig / d;
            std::uint64_t subtrahend = quotient * d;

            r_sig -= subtrahend;

            std::uint32_t lzcnt = avel::countl_zero(r_sig);
            r_sig <<= lzcnt;
            r_exp -= lzcnt;
        }

        std::uint32_t result_bits = 0;

        if (r_exp > 0) [[likely]] {
            // Compose normal result
            result_bits = (r_exp << 23) | ((r_sig >> (32 + 8)) & 0x07fffff);
        } else {
            // Compose subnormal result
            result_bits = (r_sig >> (32 + 9 - r_exp));
        }

        if (r_sig == 0) {
            result_bits = 0;
        }

        float result = avel::bit_cast<float>(result_bits);
        float ret = avel::copysign(result, x);
        return ret;
    }

    auto scalar_u64_division = scalar_test_bench<float, float, scalar_u64_division_impl>;

    BENCHMARK(fmod_32f::scalar_u64_division);


    #if defined(AVEL_X86)

    /* TODO: Complete
    AVEL_FINL std::uint64_t divide(std::array<std::uint64_t, 2> x, std::uint64_t y) {
        std::uint64_t quot;
        std::uint64_t rem;
        __asm__("divq %[v]" : "=a"(quot), "=d"(rem) : [v] "r"(y), "a"(x[0]), "d"(x[1]));
        return quot;
    }

    AVEL_FINL std::array<std::uint64_t, 2> mul(std::uint64_t x, std::uint64_t y) {
        std::uint64_t prod_hi;
        std::uint64_t prod_lo;

        __asm__(
            "mul [y]"
            : // Outputs
            "=a"(prod_lo),
            "=d"(prod_hi)
            : // Inputs
            "ax"(x),
            [y] "r"(y)
            : // Clobbers
            "rax", "rdx"
        );

        return std::array<std::uint64_t, 2>{prod_lo, prod_hi};
    }

    AVEL_FINL std::array<std::uint64_t, 2> sub(std::array<std::uint64_t, 2> x, std::array<std::uint64_t, 2> y) {
        std::uint64_t result_lo = x[0];
        std::uint64_t result_hi = x[1];

        //TODO: Implement
        __asm__(
            "sub %0, %2\n"
            "sub %1, %3"
            : //Outputs
            "+r"(result_lo),
            "+r"(result_hi)
            : //Inputs
            "r"(y[0]),
            "r"(y[1])
            : //Clobbers
        );

        return std::array<std::uint64_t, 2>{result_lo, result_hi};
    }

    AVEL_FINL std::array<std::uint64_t, 2> shift_left(std::array<std::uint64_t, 2> v, std::uint8_t s) {
        std::uint64_t result_hi = v[0];
        std::uint64_t result_lo = v[1];

        //TODO: Implement
        __asm__(
            "shld [y], [x], CL\n"
            "shl [x], CL\n"
            : //Outputs
            [x] "+r"(result_lo),
            [y] "+r"(result_hi)
            : //Inputs
            "c"(s)
            : //Clobbers
        );

        return std::array<std::uint64_t, 2>{result_lo, result_hi};
    }

    float scalar_u128_u64_division_impl(float x, float y) {
        if (avel::isnan(x) | avel::isnan(y) | avel::isinf(x) | y == 0.0f) {
            return NAN;
        }

        if (avel::isinf(y) | (avel::abs(x) < avel::abs(y))) {
            return x;
        }

        // Decompose x into denominator representation
        std::uint32_t x_bits     = avel::bit_cast<std::uint32_t>(x) & 0x7fffffff;
        std::uint32_t x_sig_bits = avel::bit_cast<std::uint32_t>(x) & 0x007fffff;

        std::int32_t r_exp = 0;
        std::uint64_t r_sig = 0;

        if (x_bits >= 0x800000) [[likely]] {
            //Handle decomposing normal x
            r_sig = std::uint64_t(x_sig_bits | 0x800000) << (32 + 8);
            r_exp = (x_bits >> 23) & 0xff;
        } else {
            //Handle decomposing subnormal x
            auto lzcnt = avel::countl_zero(x_sig_bits);
            r_sig = std::uint64_t(x_sig_bits) << (lzcnt + 32);
            r_exp = 9 - lzcnt;
        }

        // Decompose y into representation of denominator
        std::uint32_t y_bits     = avel::bit_cast<std::uint32_t>(y) & 0x7fffffff;
        std::uint32_t y_sig_bits = avel::bit_cast<std::uint32_t>(y) & 0x007fffff;

        std::int32_t d_exp = 0;
        std::uint64_t d_sig = 0;

        if (y_bits >= 0x800000) [[likely]] {
            //Handle decomposing normal x
            d_sig = std::uint64_t(y_sig_bits | 0x800000) << (32 + 8);
            d_exp = (y_bits >> 23) & 0xff;
        } else {
            //Handle decomposing subnormal x
            auto lzcnt = avel::countl_zero(y_sig_bits);
            d_sig = std::uint64_t(y_sig_bits) << (lzcnt + 32);
            d_exp = 9 - lzcnt;
        }

        std::int32_t d_sig_width = 64 - avel::countr_zero(d_sig);
        std::uint64_t d_sig0 = d_sig >> avel::countr_zero(d_sig);

        std::int32_t threshold = 64 - d_sig_width + d_exp;

        while ((r_exp > threshold) & bool(r_sig)) {
            std::uint64_t quotient = r_sig / d_sig0;
            std::uint64_t subtrahend = quotient * d_sig0;

            r_sig -= subtrahend;

            std::uint32_t lzcnt = avel::countl_zero(r_sig);
            r_sig <<= lzcnt;
            r_exp -= lzcnt;
        }

        if (r_exp > d_exp || (r_exp == d_exp && r_sig > d_sig)) {
            std::uint32_t s = d_exp - r_exp + avel::countl_zero(d_sig0);
            std::uint64_t d = d_sig0 << s;

            std::uint64_t quotient = r_sig / d;
            std::uint64_t subtrahend = quotient * d;

            r_sig -= subtrahend;

            std::uint32_t lzcnt = avel::countl_zero(r_sig);
            r_sig <<= lzcnt;
            r_exp -= lzcnt;
        }

        std::uint32_t result_bits = 0;

        if (r_exp > 0) [[likely]] {
            // Compose normal result
            result_bits = (r_exp << 23) | ((r_sig >> (32 + 8)) & 0x07fffff);
        } else {
            // Compose subnormal result
            result_bits = (r_sig >> (32 + 9 - r_exp));
        }

        if (r_sig == 0) {
            result_bits = 0;
        }

        float result = avel::bit_cast<float>(result_bits);
        float ret = avel::copysign(result, x);
        return ret;
    }

    auto scalar_u128_u64_division = scalar_test_bench<float, float, scalar_u128_u64_division_impl>;

    BENCHMARK(fmod_32f::scalar_u128_u64_division);
    */

    #endif



    float scalar_denom32u_impl(float x, float y) {
        if (avel::isnan(x) | avel::isnan(y) | avel::isinf(x) | y == 0.0f) {
            return NAN;
        }

        if (avel::isinf(y) | (avel::abs(x) < avel::abs(y))) {
            return x;
        }

        // Decompose x into denominator representation
        std::uint32_t x_bits     = avel::bit_cast<std::uint32_t>(x) & 0x7fffffff;
        std::uint32_t x_sig_bits = avel::bit_cast<std::uint32_t>(x) & 0x007fffff;

        std::int32_t r_exp = 0;
        std::uint32_t r_sig = 0;

        if (x_bits >= 0x800000) [[likely]] {
            //Handle decomposing normal x
            r_sig = (x_sig_bits | 0x800000) << 8;
            r_exp = (x_bits >> 23) & 0xff;
        } else {
            //Handle decomposing subnormal x
            auto lzcnt = avel::countl_zero(x_sig_bits);
            r_sig = x_sig_bits << lzcnt;
            r_exp = 9 - lzcnt;
        }

        // Decompose y into representation of denominator
        std::uint32_t y_bits     = avel::bit_cast<std::uint32_t>(y) & 0x7fffffff;
        std::uint32_t y_sig_bits = avel::bit_cast<std::uint32_t>(y) & 0x007fffff;

        std::int32_t d_exp = 0;
        std::uint32_t d_sig = 0;

        if (y_bits >= 0x800000) [[likely]] {
            //Handle decomposing normal x
            d_sig = (y_sig_bits | 0x800000) << 8;
            d_exp = (y_bits >> 23) & 0xff;
        } else {
            //Handle decomposing subnormal x
            auto lzcnt = avel::countl_zero(y_sig_bits);
            d_sig = y_sig_bits << lzcnt;
            d_exp = 9 - lzcnt;
        }

        const std::int32_t d_sig_tzcnt = avel::countr_zero(d_sig);
        const std::int32_t threshold = d_sig_tzcnt + d_exp;
        const std::uint32_t d_sig0 = d_sig >> d_sig_tzcnt;

        avel::Denom32u d_denom{d_sig0};
        while ((r_exp > threshold) & bool(r_sig)) {
            std::uint32_t quotient = r_sig / d_denom;
            std::uint32_t subtrahend = quotient * d_sig0;

            r_sig -= subtrahend;

            std::uint32_t lzcnt = avel::countl_zero(r_sig);
            r_sig <<= lzcnt;
            r_exp -= lzcnt;
        }

        if (r_exp > d_exp || (r_exp == d_exp && r_sig > d_sig)) {
            std::uint32_t d_width = avel::bit_width(d_sig0);

            std::uint32_t pos_offset = (32 - (r_exp - d_exp) - d_width);
            d_denom <<= pos_offset;

            std::uint32_t quotient = r_sig / d_denom;
            std::uint32_t subtrahend = quotient * d_denom.value();

            r_sig -= subtrahend;

            std::uint32_t lzcnt = avel::countl_zero(r_sig);
            r_sig <<= lzcnt;
            r_exp -= lzcnt;
        }


        std::uint32_t result_bits = 0;
        if (r_exp > 0) [[likely]] {
            // Compose normal result
            result_bits = (r_exp << 23) | ((r_sig >> 8) & 0x07fffff);
        } else {
            // Compose subnormal result
            result_bits = (r_sig >> (9 - r_exp));
        }

        if (r_sig == 0) {
            result_bits = 0;
        }

        float result = avel::bit_cast<float>(result_bits);
        float ret = avel::copysign(result, x);
        return ret;
    }

    auto scalar_denom32u = scalar_test_bench<float, float, scalar_denom32u_impl>;

    BENCHMARK(fmod_32f::scalar_denom32u);



    float scalar_denom64u_impl(float x, float y) {
        if (avel::isnan(x) | avel::isnan(y) | avel::isinf(x) | y == 0.0f) {
            return NAN;
        }

        if (avel::isinf(y) | (avel::abs(x) < avel::abs(y))) {
            return x;
        }

        // Decompose x into denominator representation
        std::uint32_t x_bits     = avel::bit_cast<std::uint32_t>(x) & 0x7fffffff;
        std::uint32_t x_sig_bits = avel::bit_cast<std::uint32_t>(x) & 0x007fffff;

        std::int32_t r_exp = 0;
        std::uint64_t r_sig = 0;

        if (x_bits >= 0x800000) [[likely]] {
            //Handle decomposing normal x
            r_sig = std::uint64_t(x_sig_bits | 0x800000) << (32 + 8);
            r_exp = (x_bits >> 23) & 0xff;
        } else {
            //Handle decomposing subnormal x
            auto lzcnt = avel::countl_zero(x_sig_bits);
            r_sig = std::uint64_t(x_sig_bits) << (lzcnt + 32);
            r_exp = 9 - lzcnt;
        }

        // Decompose y into representation of denominator
        std::uint32_t y_bits     = avel::bit_cast<std::uint32_t>(y) & 0x7fffffff;
        std::uint32_t y_sig_bits = avel::bit_cast<std::uint32_t>(y) & 0x007fffff;

        std::int32_t d_exp = 0;
        std::uint64_t d_sig = 0;

        if (y_bits >= 0x800000) [[likely]] {
            //Handle decomposing normal x
            d_sig = std::uint64_t(y_sig_bits | 0x800000) << (32 + 8);
            d_exp = (y_bits >> 23) & 0xff;
        } else {
            //Handle decomposing subnormal x
            auto lzcnt = avel::countl_zero(y_sig_bits);
            d_sig = std::uint64_t(y_sig_bits) << (lzcnt + 32);
            d_exp = 9 - lzcnt;
        }

        const std::int64_t d_sig_tzcnt = avel::countr_zero(d_sig);
        const std::int64_t threshold = d_sig_tzcnt + d_exp;
        const std::uint64_t d_sig0 = d_sig >> d_sig_tzcnt;

        avel::Denom64u d_denom{d_sig0};
        while ((r_exp > threshold) & bool(r_sig)) {
            std::uint64_t quotient = r_sig / d_denom;
            std::uint64_t subtrahend = quotient * d_sig0;

            r_sig -= subtrahend;

            std::uint32_t lzcnt = avel::countl_zero(r_sig);
            r_sig <<= lzcnt;
            r_exp -= lzcnt;
        }

        if (r_exp > d_exp || (r_exp == d_exp && r_sig > d_sig)) {
            std::uint32_t d_width = avel::bit_width(d_sig0);

            std::uint32_t pos_offset = (64 - (r_exp - d_exp) - d_width);
            d_denom <<= pos_offset;

            std::uint64_t quotient = r_sig / d_denom;
            std::uint64_t subtrahend = quotient * d_denom.value();

            r_sig -= subtrahend;

            std::uint32_t lzcnt = avel::countl_zero(r_sig);
            r_sig <<= lzcnt;
            r_exp -= lzcnt;
        }

        std::uint32_t result_bits = 0;

        if (r_exp > 0) [[likely]] {
            // Compose normal result
            result_bits = (r_exp << 23) | ((r_sig >> (32 + 8)) & 0x07fffff);
        } else {
            // Compose subnormal result
            result_bits = (r_sig >> (32 + 9 - r_exp));
        }

        if (r_sig == 0) {
            result_bits = 0;
        }

        float result = avel::bit_cast<float>(result_bits);
        float ret = std::copysign(result, x);
        return ret;
    }

    auto scalar_denom64u = scalar_test_bench<float, float, scalar_denom64u_impl>;

    BENCHMARK(fmod_32f::scalar_denom64u);



    float scalar_fixed_point_recip(float x, float y) {
        float x_orig = x;

        //std::uint64_t recip = ( / ) / ();

        return avel::copysign(x, x_orig);
    }

    auto scalar_fixed_point = scalar_test_bench<float, float, scalar_fixed_point_recip>;

    BENCHMARK(fmod_32f::scalar_fixed_point);



    float scalar_64f_div_impl(float x, float y) {
        if (avel::isnan(x) | avel::isnan(y) | avel::isinf(x) | y == 0.0f) {
            return NAN;
        }

        if (avel::isinf(y) | (avel::abs(x) < avel::abs(y))) {
            return x;
        }

        auto decrement = [] (double x) -> double {
            std::uint64_t y = avel::bit_cast<std::uint64_t>(x);
            y -= 1;

            return avel::bit_cast<double>(y);
        };

        double xd = double(avel::abs(x));
        const double yd = double(avel::abs(y));

        // Mask used to reduce number of significant digits in subtrahend
        // to prevent an inexact operation. This leaves 29 significant bits
        // which when multiplied again yd's max of 24 significant digits,
        // produces a result with at most 52 significant digits
        constexpr std::int64_t mask = 0xffffffffff000000ll;

        // Loop runs for at most 10 iterations
        while ((yd + yd) <= xd) {
            double quotient = xd / yd;

            // Note: The generated assembly is suboptimal for x86 on a number of compilers. The AND operation is
            // performed in a GPR after moving the value instead of within the XMM registers.
            auto quotient_bits = avel::bit_cast<std::uint64_t>(decrement(quotient));
            auto masked_quotient_bits = quotient_bits & mask;
            double masked_quotient = avel::bit_cast<double>(masked_quotient_bits);

            double trunc_q = std::trunc(masked_quotient);

            double subtrahend = trunc_q * yd;
            xd -= subtrahend;
        }

        if (xd >= yd) {
            xd -= yd;
        }

        float ret = xd;
        ret = avel::copysign(ret, x);
        return ret;
    }

    auto scalar_64f_div = scalar_test_bench<float, float, scalar_64f_div_impl>;

    BENCHMARK(fmod_32f::scalar_64f_div);



    #if defined(AVEL_FMA) || defined(AVEL_NEON)

    float scalar_64f_div_fma_impl(float x, float y) {
        if (avel::isnan(x) | avel::isnan(y) | avel::isinf(x) | y == 0.0f) {
            return NAN;
        }

        if (avel::isinf(y) | (avel::abs(x) < avel::abs(y))) {
            return x;
        }

        auto decrement = [] (double x) -> double {
            std::uint64_t y = avel::bit_cast<std::uint64_t>(x);
            y -= 1;

            return avel::bit_cast<double>(y);
        };

        double xd = double(avel::abs(x));
        const double yd = double(avel::abs(y));

        // Loop runs for at most 6 iterations
        while ((yd + yd) <= xd) {
            double quotient = xd / yd;
            double tq = std::trunc(decrement(quotient));
            xd = std::fma(-tq, yd, xd);
        }

        if (xd >= yd) {
            xd -= yd;
        }

        float ret = xd;
        ret = avel::copysign(ret, x);
        return ret;
    }

    auto scalar_64f_div_fma = scalar_test_bench<float, float, scalar_64f_div_fma_impl>;

    BENCHMARK(fmod_32f::scalar_64f_div_fma);

    #endif



    float scalar_64f_recip_impl(float x, float y) {
        if (avel::isnan(x) | avel::isnan(y) | avel::isinf(x) | y == 0.0f) {
            return NAN;
        }

        if (avel::isinf(y) | (avel::abs(x) < avel::abs(y))) {
            return x;
        }

        auto decrement = [] (double x) -> double {
            std::uint64_t y = avel::bit_cast<std::uint64_t>(x);
            y -= 1;

            return avel::bit_cast<double>(y);
        };

        double xd = double(avel::abs(x));
        const double yd = double(avel::abs(y));
        const double recip_yd = decrement(1.0 / yd);

        // Mask used to reduce number of significant digits in subtrahend
        // to prevent an inexact operation. This leaves 29 significant bits
        // which when multiplied again yd's max of 24 significant digits,
        // produces a result with at most 52 significant digits
        constexpr std::int64_t mask = 0xffffffffff000000ll;

        // Loop runs for at most 10 iterations
        while ((yd + yd) <= xd) {
            double quotient = xd * recip_yd;

            //TODO: This leads to suboptimal codegen. Consider alternatives
            auto quotient_bits = avel::bit_cast<std::uint64_t>(quotient);
            auto masked_quotient_bits = quotient_bits & mask;
            double masked_quotient = avel::bit_cast<double>(masked_quotient_bits);

            double trunc_q = std::trunc(masked_quotient);

            double subtrahend = trunc_q * yd;
            xd -= subtrahend;
        }

        if (xd >= yd) {
            xd -= yd;
        }

        float ret = xd;
        ret = avel::copysign(ret, x);
        return ret;
    }

    auto scalar_64f_recip = scalar_test_bench<float, float, scalar_64f_recip_impl>;

    BENCHMARK(fmod_32f::scalar_64f_recip);



    #if defined(AVEL_FMA) || defined(AVEL_NEON)

    float scalar_64f_recip_fma_impl(float x, float y) {
        if (avel::isnan(x) | avel::isnan(y) | avel::isinf(x) | y == 0.0f) {
            return NAN;
        }

        if (avel::isinf(y) | (avel::abs(x) < avel::abs(y))) {
            return x;
        }

        auto decrement = [] (double x) -> double {
            std::uint64_t y = avel::bit_cast<std::uint64_t>(x);
            y -= 1;

            return avel::bit_cast<double>(y);
        };

        double xd = double(avel::abs(x));
        const double yd = double(avel::abs(y));
        const double recip_yd = decrement(1.0 / yd);

        // Loop runs for at most 10 iterations
        while ((yd + yd) <= xd) {
            double quotient = xd * recip_yd;
            double tq = std::trunc(quotient);
            xd = std::fma(-tq, yd, xd);
        }

        if (xd >= yd) {
            xd -= yd;
        }

        float ret = xd;
        ret = avel::copysign(ret, x);
        return ret;
    }

    auto scalar_64f_recip_fma = scalar_test_bench<float, float, scalar_64f_recip_fma_impl>;

    BENCHMARK(fmod_32f::scalar_64f_recip_fma);

    #endif

    //=====================================================
    // vec4x32f benchmarks
    //=====================================================

    #if defined(AVEL_SSE2)

    /*
    vec4x32f vec4x32f_bit_by_bit_32f_impl(vec4x32f x, vec4x32f y) {
        __m128i abs_mask = _mm_set1_epi32(0x7fffffff);

        // Get bits
        __m128i x_bits = _mm_castps_si128(decay(x));
        __m128i y_bits = _mm_castps_si128(decay(y));

        // Compute absolute value of both x and y
        __m128i abs_x_bits = _mm_and_si128(x_bits, abs_mask);
        __m128i abs_y_bits = _mm_and_si128(y_bits, abs_mask);

        // Test for subnormal inputs
        __m128i subnormal_threshold = _mm_set1_epi32(0x00800000);
        __m128i is_x_subnormal = _mm_cmplt_ps(abs_x_bits, subnormal_threshold);
        __m128i is_y_subnormal = _mm_cmplt_ps(abs_y_bits, subnormal_threshold);

        __m128i full_exp = _mm_set1_epi32(0x7f800000);

        bool subnormal_input_exists = _mm_movemask_epi8(_mm_or_si128(is_x_subnormal, is_y_subnormal));

        // Test for edge case inputs
        __m128 is_x_edge_case = _mm_cmpeq_epi32(_mm_and_si128(abs_x_bits, full_exp), full_exp);

        __m128 is_y_edge_case = _mm_andnot_ps(
            _mm_cmpeq_ps(decay(y), decay(y)), // Check if y is nan
            _mm_cmpeq_ps(decay(y), _mm_setzero_ps()) // Check if y is zero
        );

        __m128 is_result_nan = _mm_or_ps(is_x_edge_case, is_y_edge_case);

        // Remap d to have
        __m128 r = _mm_undefined_ps();
        __m128 d = _mm_undefined_ps();
        if (subnormal_input_exists) [[unlikely]] {
            __m128i one_bits = _mm_set1_epi32(0x3f800000);
            __m128i one_offset = _mm_set1_epi32(0xC000000);

            __m128i x_correction_factor_bits = _mm_add_epi32(one_bits, _mm_and_si128(is_x_subnormal, one_offset));
            __m128i y_correction_factor_bits = _mm_add_epi32(one_bits, _mm_and_si128(is_y_subnormal, one_offset));

            __m128 x_correction_factor = _mm_castsi128_ps(x_correction_factor_bits);
            __m128 y_correction_factor = _mm_castsi128_ps(y_correction_factor_bits);

            __m128 abs_x_corrected = _mm_mul_ps(_mm_castsi128_ps(abs_x_bits), x_correction_factor);
            __m128 abs_y_corrected = _mm_mul_ps(_mm_castsi128_ps(abs_y_bits), y_correction_factor);


            __m128i ;

            //TODO: Handle subnormal inputs properly and then test correctness


            __m128i exponent_mask = _mm_set1_epi32(0x7f800000);
            __m128i d_bits = _mm_or_si128(
                _mm_and_si128(exponent_mask, abs_x_bits),
                _mm_andnot_si128(exponent_mask, abs_y_bits)
            );

            r = _mm_castsi128_ps(abs_x_bits);
            d = _mm_castsi128_ps(d_bits);


            return {};
        } else {
            __m128i exponent_mask = _mm_set1_epi32(0x7f800000);
            __m128i d_bits = _mm_or_si128(
                _mm_and_si128(exponent_mask, abs_x_bits),
                _mm_andnot_si128(exponent_mask, abs_y_bits)
            );

            r = _mm_castsi128_ps(abs_x_bits);
            d = _mm_castsi128_ps(d_bits);
        }

        __m128 one_half = _mm_set1_ps(0.5f);

        __m128 m = _mm_andnot_ps(is_result_nan, _mm_cmple_ps(d, r));
        while (_mm_movemask_ps(m)) {
            auto cmp_result = _mm_and_ps(m, _mm_cmple_ps(d, r));
            r = _mm_sub_ps(r, _mm_and_ps(cmp_result, d));
            d = _mm_mul_ps(d, one_half);

            m = _mm_and_ps(m, _mm_cmple_ps(d, r));
        }

        // Copy sign bit from x
        __m128 sign_bit_mask = _mm_castsi128_ps(_mm_set1_epi32(0x80000000));
        __m128 ret = _mm_or_ps(_mm_and_ps(sign_bit_mask, decay(x)), r);

        // Conditionally make result nan
        ret = _mm_or_ps(
            _mm_andnot_ps(is_result_nan, ret),
            _mm_and_ps(is_result_nan, _mm_set1_ps(NAN))
        );

        return vec4x32f{ret};
    }

    auto vec4x32f_bit_by_bit_32f = vector_test_bench<vec4x32f, vec4x32f, vec4x32f_bit_by_bit_32f_impl>;

    BENCHMARK(fmod_32f::vec4x32f_bit_by_bit_32f);
    */

    #endif

    #if defined(AVEL_SSE2)

    /*
    vec4x32f vec4x32f_f64_recip_impl(vec4x32f x, vec4x32f y) {
        auto does_y_demand_special_handling = _mm_or_ps(
            _mm_cmpeq_ps(decay(y), )
            _mm_cmpeq_ps(decay(y), _mm_setzero_ps()),
        );

        __m128 abs_mask = _mm_castsi128_ps(_mm_set1_epi32(0x7fffffff));
        auto abs_x = _mm_and_ps(abs_mask, decay(x));
        auto xd_lo = _mm_cvtps_pd(decay(x));
        auto xd_hi = _mm_cvtps_pd(_mm_castsi128_ps(_mm_bsrli_si128(_mm_castps_si128(decay(x)), 8)));

        auto abs_y = _mm_and_ps(abs_mask, decay(y));
        auto yd_lo = _mm_cvtps_pd(decay(y));
        auto yd_hi = _mm_cvtps_pd(_mm_castsi128_ps(_mm_bsrli_si128(_mm_castps_si128(decay(y)), 8)));

        auto f64_one = _mm_set1_pd(1.0);
        auto recip_yd_lo = _mm_div_pd(f64_one, yd_lo);
        auto recip_yd_hi = _mm_div_pd(f64_one, yd_lo);

        //TODO: Complete implementation

        const auto mask = _mm_castsi128_pd(_mm_set1_epi64x(0xffffffffff000000ll));
        while (_mm_movemask_pd(_mm_cmple_pd(yd_lo, xd_lo))) {


        }

        while (_mm_movemask_pd(_mm_cmple_pd(yd_hi, xd_hi))) {


        }

        auto result_lo = ;
        auto result_hi = ;



        return ret;

    }

    BENCHMARK(fmod_32f::vec4x32f_f64_recip);
    */

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)

    vec4x32f vec4x32f_32u_bit_by_bit_impl(vec4x32f x, vec4x32f y) {
        __mmask8 is_result_nan = _kor_mask16(
            _mm_fpclass_ps_mask(decay(x), 0x99),
            _mm_fpclass_ps_mask(decay(y), 0x87)
        );

        // Decompose x into exponent and significand
        __mmask8 is_x_edge_case = _mm_fpclass_ps_mask(decay(x), 0x9f);
        __m128i r_exp = _mm_mask_cvtps_epi32(_mm_set1_epi32(-200), ~is_x_edge_case, _mm_getexp_ps(decay(x)));

        __m128 x_sig = _mm_getmant_ps(decay(x), _MM_MANT_NORM_1_2, _MM_MANT_SIGN_zero);
        __m128i r_sig = _mm_maskz_srli_epi32(~is_x_edge_case, _mm_slli_epi32(_mm_castps_si128(x_sig), 8), 1);


        // Decompose x into exponent and significand
        __mmask8 is_y_edge_case = _mm_fpclass_ps_mask(decay(y), 0x9f);
        __m128i d_exp = _mm_mask_cvtps_epi32(_mm_set1_epi32(200), ~is_y_edge_case, _mm_getexp_ps(decay(y)));

        __m128 y_sig = _mm_getmant_ps(decay(y), _MM_MANT_NORM_1_2, _MM_MANT_SIGN_zero);
        __m128i d_sig = _mm_maskz_srli_epi32(~is_y_edge_case, _mm_slli_epi32(_mm_castps_si128(y_sig), 8), 1);


        // Execute long division via repeated subtraction
        __m128i full = _mm_set1_epi32(-1);
        __mmask8 loop_mask = _mm_cmp_epi32_mask(d_exp, r_exp, _MM_CMPINT_LT) & _mm_test_epi32_mask(r_sig, r_sig);
        while (loop_mask) {
            auto m = _mm_mask_cmp_epu32_mask(loop_mask, d_sig, r_sig, _MM_CMPINT_LE);
            r_sig = _mm_mask_sub_epi32(r_sig, m, r_sig, d_sig);

            r_sig = _mm_mask_add_epi32(r_sig, loop_mask, r_sig, r_sig);
            r_exp = _mm_mask_add_epi32(r_exp, loop_mask, r_exp, full);

            loop_mask = _mm_cmp_epi32_mask(d_exp, r_exp, _MM_CMPINT_LT) & _mm_test_epi32_mask(r_sig, r_sig);
        }

        auto m = _mm_cmp_epi32_mask(d_exp, r_exp, _MM_CMPINT_LE) & _mm_cmp_epu32_mask(r_sig, d_sig, _MM_CMPINT_NLT);
        r_sig = _mm_mask_sub_epi32(r_sig, m, r_sig, d_sig);


        // Reconstruct result
        __m128 ret = _mm_range_ps(decay(x), _mm_setzero_ps(), 0x02);
        ret = _mm_or_ps(ret, _mm_cvtepi32_ps(r_sig));

        //TODO:
        __m128i exponent_offset = _mm_sub_epi32(r_exp, _mm_set1_epi32(30));
        auto tmp = _mm_cvtepi32_ps(exponent_offset);
        ret = _mm_scalef_ps(ret, tmp);

        ret = _mm_mask_blend_ps(is_result_nan, ret, _mm_set1_ps(NAN));
        return vec4x32f{ret};
    }

    auto vec4x32f_32u_bit_by_bit = vector_test_bench<vec4x32f, vec4x32f, vec4x32f_32u_bit_by_bit_impl>;

    BENCHMARK(fmod_32f::vec4x32f_32u_bit_by_bit);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)

    vec4x32f vec4x32f_f64_recip_avx512dq_widening_impl(vec4x32f x, vec4x32f y) {
        auto does_y_demand_special_handling = ~_mm_fpclass_ps_mask(decay(y), 0x87);
        auto nans = _mm_set1_ps(NAN);

        auto sign_bit_mask = _mm_castsi128_ps(_mm_set1_epi32(0x80000000));

        auto abs_x = _mm_mask_andnot_ps(nans, does_y_demand_special_handling, sign_bit_mask, decay(x));
        auto xd = _mm256_cvtps_pd(abs_x);

        auto abs_y = _mm_mask_andnot_ps(nans, does_y_demand_special_handling, sign_bit_mask, decay(y));
        auto yd = _mm256_cvtps_pd(abs_y);

        auto f64_one = _mm512_set1_pd(1.0);
        auto recip_yd = _mm512_castpd512_pd256(_mm512_div_round_pd(f64_one, _mm512_zextpd256_pd512(yd), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC));

        auto mask = _mm256_castsi256_pd(_mm256_set1_epi64x(0xffffffffff000000ll));
        while (_mm256_cmp_pd_mask(yd, xd, _CMP_LE_OQ)) {
            auto quotient = _mm256_mul_pd(xd, recip_yd);
            auto masked_quotient = _mm256_and_pd(quotient, mask);
            auto trunc_q = _mm256_roundscale_pd(masked_quotient, _MM_FROUND_TO_ZERO);

            xd = _mm256_fnmadd_pd(trunc_q, yd, xd);
        }

        auto result = _mm256_cvtpd_ps(xd);
        auto ret = avel::copysign(vec4x32f{result}, x);
        return ret;
    }

    auto vec4x32f_f64_recip_avx512dq_widening = vector_test_bench<vec4x32f, vec4x32f, vec4x32f_f64_recip_avx512dq_widening_impl>;

    BENCHMARK(fmod_32f::vec4x32f_f64_recip_avx512dq_widening);

    #endif

    //=====================================================
    // vec8x32f benchmarks
    //=====================================================



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)

    vec8x32f vec8x32f_32u_bit_by_bit_impl(vec8x32f x, vec8x32f y) {
        __mmask8 is_result_nan = _kor_mask16(
            _mm256_fpclass_ps_mask(decay(x), 0x99),
            _mm256_fpclass_ps_mask(decay(y), 0x87)
        );

        // Decompose x into exponent and significand
        __mmask8 is_x_edge_case = _mm256_fpclass_ps_mask(decay(x), 0x9f);
        __m256i r_exp = _mm256_mask_cvtps_epi32(_mm256_set1_epi32(-200), ~is_x_edge_case, _mm256_getexp_ps(decay(x)));

        __m256 x_sig = _mm256_getmant_ps(decay(x), _MM_MANT_NORM_1_2, _MM_MANT_SIGN_zero);
        __m256i r_sig = _mm256_maskz_srli_epi32(~is_x_edge_case, _mm256_slli_epi32(_mm256_castps_si256(x_sig), 8), 1);


        // Decompose x into exponent and significand
        __mmask8 is_y_edge_case = _mm256_fpclass_ps_mask(decay(y), 0x9f);
        __m256i d_exp = _mm256_mask_cvtps_epi32(_mm256_set1_epi32(200), ~is_y_edge_case, _mm256_getexp_ps(decay(y)));

        __m256 y_sig = _mm256_getmant_ps(decay(y), _MM_MANT_NORM_1_2, _MM_MANT_SIGN_zero);
        __m256i d_sig = _mm256_maskz_srli_epi32(~is_y_edge_case, _mm256_slli_epi32(_mm256_castps_si256(y_sig), 8), 1);


        // Execute long division via repeated subtraction
        __m256i full = _mm256_set1_epi32(-1);
        __mmask8 loop_mask = _mm256_cmp_epi32_mask(d_exp, r_exp, _MM_CMPINT_LT) & _mm256_test_epi32_mask(r_sig, r_sig);
        while (loop_mask) {
            auto m = _mm256_mask_cmp_epu32_mask(loop_mask, d_sig, r_sig, _MM_CMPINT_LE);
            r_sig = _mm256_mask_sub_epi32(r_sig, m, r_sig, d_sig);

            r_sig = _mm256_mask_add_epi32(r_sig, loop_mask, r_sig, r_sig);
            r_exp = _mm256_mask_add_epi32(r_exp, loop_mask, r_exp, full);

            loop_mask = _mm256_cmp_epi32_mask(d_exp, r_exp, _MM_CMPINT_LT) & _mm256_test_epi32_mask(r_sig, r_sig);
        }

        auto m = _mm256_cmp_epi32_mask(d_exp, r_exp, _MM_CMPINT_LE) & _mm256_cmp_epu32_mask(r_sig, d_sig, _MM_CMPINT_NLT);
        r_sig = _mm256_mask_sub_epi32(r_sig, m, r_sig, d_sig);


        // Reconstruct result
        __m256 ret = _mm256_range_ps(decay(x), _mm256_setzero_ps(), 0x02);
        ret = _mm256_or_ps(ret, _mm256_cvtepi32_ps(r_sig));

        __m256i exponent_offset = _mm256_sub_epi32(r_exp, _mm256_set1_epi32(30));
        auto tmp = _mm256_cvtepi32_ps(exponent_offset);
        ret = _mm256_scalef_ps(ret, tmp);

        ret = _mm256_mask_blend_ps(is_result_nan, ret, _mm256_set1_ps(NAN));
        return vec8x32f{ret};
    }

    auto vec8x32f_32u_bit_by_bit = vector_test_bench<vec8x32f, vec8x32f, vec8x32f_32u_bit_by_bit_impl>;

    BENCHMARK(fmod_32f::vec8x32f_32u_bit_by_bit);

    #endif



    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)

    vec8x32f vec8x32f_f64_recip_avx512dq_widening_impl(vec8x32f x, vec8x32f y) {
        auto does_y_demand_special_handling = ~_mm256_fpclass_ps_mask(decay(y), 0x87);
        auto nans = _mm256_set1_ps(NAN);

        auto sign_bit_mask = _mm256_castsi256_ps(_mm256_set1_epi32(0x80000000));

        auto abs_x = _mm256_mask_and_ps(nans, does_y_demand_special_handling, decay(x), sign_bit_mask);
        auto xd = _mm512_cvtps_pd(abs_x);

        auto abs_y = _mm256_mask_and_ps(nans, does_y_demand_special_handling, decay(y), sign_bit_mask);
        auto yd = _mm512_cvtps_pd(abs_y);

        auto f64_one = _mm512_set1_pd(1.0);
        auto recip_yd = _mm512_div_round_pd(f64_one, yd, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);

        auto mask = _mm512_castsi512_pd(_mm512_set1_epi64(0xffffffffff000000ll));
        while (_mm512_cmp_pd_mask(yd, xd, _CMP_LE_OQ)) {
            auto quotient = _mm512_mul_pd(xd, recip_yd);
            auto masked_quotient = _mm512_and_pd(quotient, mask);
            auto trunc_q = _mm512_roundscale_pd(masked_quotient, _MM_FROUND_TO_ZERO);

            xd = _mm512_fnmadd_pd(trunc_q, yd, xd);
        }

        auto result = _mm512_cvtpd_ps(xd);
        auto ret = avel::copysign(vec8x32f{result}, x);
        return ret;
    }

    auto vec8x32f_f64_recip_avx512dq_widening = vector_test_bench<vec8x32f, vec8x32f, vec8x32f_f64_recip_avx512dq_widening_impl>;

    BENCHMARK(fmod_32f::vec8x32f_f64_recip_avx512dq_widening);

    #endif

    //=====================================================
    // vec16x32f benchmarks
    //=====================================================

    #if defined(AVEL_AVX512F)

    vec16x32f vec16x32f_denom32u_impl(vec16x32f x, vec16x32f y) {
        //TODO: Test correctness
        __mmask16 is_result_nan = _kor_mask16(
            _mm512_fpclass_ps_mask(decay(x), 0x99),
            _mm512_fpclass_ps_mask(decay(y), 0x87)
        );

        // Decompose x into exponent and significand
        __mmask16 is_x_edge_case = _mm512_fpclass_ps_mask(decay(x), 0x9f);
        __m512i r_exp = _mm512_mask_cvtps_epi32(_mm512_set1_epi32(-200), ~is_x_edge_case, _mm512_getexp_ps(decay(x)));

        __m512 x_sig = _mm512_getmant_ps(decay(x), _MM_MANT_NORM_1_2, _MM_MANT_SIGN_zero);
        __m512i r_sig = _mm512_maskz_srli_epi32(~is_x_edge_case, _mm512_slli_epi32(_mm512_castps_si512(x_sig), 8), 1);


        // Decompose y into exponent and significand
        __mmask16 is_y_edge_case = _mm512_fpclass_ps_mask(decay(y), 0x9f);
        __m512i d_exp = _mm512_mask_cvtps_epi32(_mm512_set1_epi32(200), ~is_y_edge_case, _mm512_getexp_ps(decay(y)));

        __m512 y_sig = _mm512_getmant_ps(decay(y), _MM_MANT_NORM_1_2, _MM_MANT_SIGN_zero);
        __m512i d_sig = _mm512_maskz_srli_epi32(~is_y_edge_case, _mm512_slli_epi32(_mm512_castps_si512(y_sig), 8), 1);

        // Ensure that d_sig does not have lanes equal to zero
        d_sig = _mm512_mask_blend_epi32(is_result_nan, d_sig, _mm512_set1_epi32(0x01));

        // Execute long division via repeated subtraction
        avel::Denom16x32u d_denom{vec16x32u{d_sig}};

        __m512i threshold = _mm512_add_epi32(decay(countr_zero(vec16x32u{d_sig})), d_exp);
        __mmask16 loop_mask =
            _mm512_cmp_epi32_mask(r_exp, threshold, _MM_CMPINT_NLE) &
            _mm512_test_epi32_mask(r_sig, r_sig);

        while (loop_mask) {
            vec16x32u quotient = vec16x32u{r_sig} / d_denom;
            vec16x32u subtrahend = quotient * d_denom.value();

            r_sig = _mm512_mask_sub_epi32(r_sig, loop_mask, r_sig, decay(subtrahend));

            __m512i lzcnt = _mm512_lzcnt_epi32(r_sig);
            r_sig = _mm512_mask_sllv_epi32(r_sig, loop_mask, r_sig, lzcnt);
            r_exp = _mm512_mask_sub_epi32(r_exp, loop_mask, r_exp, lzcnt);

            auto a = _mm512_cmp_epi32_mask(r_exp, threshold, _MM_CMPINT_NLE);
            auto b = _mm512_test_epi32_mask(r_sig, r_sig);
            loop_mask = a & b;
        }

        {
            auto m =
                _mm512_cmp_epi32_mask(r_exp, d_exp, _MM_CMPINT_NLE) |
                (
                    _mm512_cmp_epi32_mask(r_exp, d_exp, _MM_CMPINT_EQ) &
                    _mm512_cmp_epi32_mask(r_sig, d_sig, _MM_CMPINT_NLE)
                );

            vec16x32u d_width = bit_width(vec16x32u{d_sig});
            vec16x32u pos_offset = (vec16x32u{32} - (vec16x32u{r_exp} - vec16x32u{d_exp}) - d_width);
            d_denom <<= pos_offset;

            vec16x32u quotient = vec16x32u{r_sig} / d_denom;
            vec16x32u subtrahend = quotient * d_denom.value();

            r_sig = _mm512_mask_sub_epi32(r_sig, m, r_sig, decay(subtrahend));

            __m512i lzcnt = _mm512_lzcnt_epi32(r_sig);
            r_sig = _mm512_mask_sllv_epi32(r_sig, m, r_sig, lzcnt);
            r_exp = _mm512_mask_sub_epi32(r_exp, m, r_exp, lzcnt);
        }

        // Reconstruct result
        __m512 ret = _mm512_range_ps(decay(x), _mm512_setzero_ps(), 0x02);
        ret = _mm512_or_ps(ret, _mm512_cvtepi32_ps(r_sig));

        __m512i exponent_offset = _mm512_sub_epi32(r_exp, _mm512_set1_epi32(30));
        auto tmp = _mm512_cvtepi32_ps(exponent_offset);
        ret = _mm512_scalef_ps(ret, tmp);

        ret = _mm512_mask_blend_ps(is_result_nan, ret, _mm512_set1_ps(NAN));
        return vec16x32f{ret};
    }

    auto vec16x32f_denom32u = vector_test_bench<vec16x32f, vec16x32f, vec16x32f_denom32u_impl>;

    BENCHMARK(fmod_32f::vec16x32f_denom32u);

    #endif



    #if defined(AVEL_AVX512DQ)

    vec16x32f vec16x32f_32u_bit_by_bit_impl(vec16x32f x, vec16x32f y) {
        __mmask16 is_result_nan = _kor_mask16(
            _mm512_fpclass_ps_mask(decay(x), 0x99),
            _mm512_fpclass_ps_mask(decay(y), 0x87)
        );

        // Decompose x into exponent and significand
        __mmask16 is_x_edge_case = _mm512_fpclass_ps_mask(decay(x), 0x9f);
        __m512i r_exp = _mm512_mask_cvtps_epi32(_mm512_set1_epi32(-200), ~is_x_edge_case, _mm512_getexp_ps(decay(x)));

        __m512 x_sig = _mm512_getmant_ps(decay(x), _MM_MANT_NORM_1_2, _MM_MANT_SIGN_zero);
        __m512i r_sig = _mm512_maskz_srli_epi32(~is_x_edge_case, _mm512_slli_epi32(_mm512_castps_si512(x_sig), 8), 1);


        // Decompose y into exponent and significand
        __mmask16 is_y_edge_case = _mm512_fpclass_ps_mask(decay(y), 0x9f);
        __m512i d_exp = _mm512_mask_cvtps_epi32(_mm512_set1_epi32(200), ~is_y_edge_case, _mm512_getexp_ps(decay(y)));

        __m512 y_sig = _mm512_getmant_ps(decay(y), _MM_MANT_NORM_1_2, _MM_MANT_SIGN_zero);
        __m512i d_sig = _mm512_maskz_srli_epi32(~is_y_edge_case, _mm512_slli_epi32(_mm512_castps_si512(y_sig), 8), 1);


        // Execute long division via repeated subtraction
        __m512i full = _mm512_set1_epi32(-1);
        __mmask16 loop_mask = _mm512_cmp_epi32_mask(d_exp, r_exp, _MM_CMPINT_LT) & _mm512_test_epi32_mask(r_sig, r_sig);
        while (loop_mask) {
            auto m = _mm512_mask_cmp_epu32_mask(loop_mask, d_sig, r_sig, _MM_CMPINT_LE);
            r_sig = _mm512_mask_sub_epi32(r_sig, m, r_sig, d_sig);

            r_sig = _mm512_mask_add_epi32(r_sig, loop_mask, r_sig, r_sig);
            r_exp = _mm512_mask_add_epi32(r_exp, loop_mask, r_exp, full);

            loop_mask = _mm512_cmp_epi32_mask(d_exp, r_exp, _MM_CMPINT_LT) & _mm512_test_epi32_mask(r_sig, r_sig);
        }

        auto m = _mm512_cmp_epi32_mask(d_exp, r_exp, _MM_CMPINT_LE) & _mm512_cmp_epu32_mask(r_sig, d_sig, _MM_CMPINT_NLT);
        r_sig = _mm512_mask_sub_epi32(r_sig, m, r_sig, d_sig);


        // Reconstruct result
        __m512 ret = _mm512_range_ps(decay(x), _mm512_setzero_ps(), 0x02);
        ret = _mm512_or_ps(ret, _mm512_cvtepi32_ps(r_sig));

        __m512i exponent_offset = _mm512_sub_epi32(r_exp, _mm512_set1_epi32(30));
        auto tmp = _mm512_cvtepi32_ps(exponent_offset);
        ret = _mm512_scalef_ps(ret, tmp);

        ret = _mm512_mask_blend_ps(is_result_nan, ret, _mm512_set1_ps(NAN));
        return vec16x32f{ret};
    }

    auto vec16x32f_32u_bit_by_bit = vector_test_bench<vec16x32f, vec16x32f, vec16x32f_32u_bit_by_bit_impl>;

    BENCHMARK(fmod_32f::vec16x32f_32u_bit_by_bit);

    #endif



    #if defined(AVEL_AVX512DQ)

    vec16x32f vec16x32f_bit_by_bit_32f_impl(vec16x32f x, vec16x32f y) {
        //TODO: Test correctness
        __mmask16 is_result_nan = _kor_mask16(
            _mm512_fpclass_ps_mask(decay(x), 0x99),
            _mm512_fpclass_ps_mask(decay(y), 0x87)
        );

        __m512 abs_x = _mm512_abs_ps(decay(x));
        __m512 abs_y = _mm512_abs_ps(decay(y));

        __m512 neg_one = _mm512_set1_ps(-1.0f);

        __m512 exp_diff = _mm512_sub_ps(_mm512_getexp_ps(decay(x)), _mm512_getexp_ps(decay(y)));
        __m512 d = _mm512_scalef_ps(abs_y, _mm512_max_ps(exp_diff, _mm512_setzero_ps()));
        d = _mm512_mask_scalef_ps(d, _mm512_cmp_ps_mask(abs_x, d, _CMP_LT_OQ), d, neg_one);

        __mmask16 m = ~is_result_nan & _mm512_cmp_ps_mask(abs_y, abs_x, _CMP_LT_OQ);
        while (m) {
            __mmask16 cmp_result = _mm512_mask_cmp_ps_mask(m, d, abs_x, _CMP_LT_OQ);
            abs_x = _mm512_mask_sub_ps(abs_x, cmp_result, abs_x, d);
            d = _mm512_mask_scalef_ps(d, m, d, neg_one);

            m = _mm512_mask_cmp_ps_mask(m, abs_y, abs_x, _CMP_LT_OQ);
        }

        // Perform copy sign operation
        auto ret = _mm512_range_ps(decay(x), abs_x, 0x02);

        // Conditionally make result nan
        ret = _mm512_mask_blend_ps(is_result_nan, ret, _mm512_set1_ps(NAN));

        return vec16x32f{ret};
    }

    auto vec16x32f_bit_by_bit_32f = vector_test_bench<vec16x32f, vec16x32f, vec16x32f_bit_by_bit_32f_impl>;

    BENCHMARK(fmod_32f::vec16x32f_bit_by_bit_32f);

    #endif



    #if defined(AVEL_AVX512DQ)

    vec16x32f vec16x32f_denom32u_avx512dq_impl(vec16x32f x, vec16x32f y) {
        //TODO: Test correctness
        __mmask16 is_result_nan = _kor_mask16(
            _mm512_fpclass_ps_mask(decay(x), 0x99),
            _mm512_fpclass_ps_mask(decay(y), 0x87)
        );

        // Decompose x into exponent and significand
        __mmask16 is_x_edge_case = _mm512_fpclass_ps_mask(decay(x), 0x9f);
        __m512i r_exp = _mm512_mask_cvtps_epi32(_mm512_set1_epi32(-200), ~is_x_edge_case, _mm512_getexp_ps(decay(x)));

        __m512 x_sig = _mm512_getmant_ps(decay(x), _MM_MANT_NORM_1_2, _MM_MANT_SIGN_zero);
        __m512i r_sig = _mm512_maskz_srli_epi32(~is_x_edge_case, _mm512_slli_epi32(_mm512_castps_si512(x_sig), 8), 1);


        // Decompose y into exponent and significand
        __mmask16 is_y_edge_case = _mm512_fpclass_ps_mask(decay(y), 0x9f);
        __m512i d_exp = _mm512_mask_cvtps_epi32(_mm512_set1_epi32(200), ~is_y_edge_case, _mm512_getexp_ps(decay(y)));

        __m512 y_sig = _mm512_getmant_ps(decay(y), _MM_MANT_NORM_1_2, _MM_MANT_SIGN_zero);
        __m512i d_sig = _mm512_maskz_srli_epi32(~is_y_edge_case, _mm512_slli_epi32(_mm512_castps_si512(y_sig), 8), 1);

        // Ensure that d_sig does not have lanes equal to zero
        d_sig = _mm512_mask_blend_epi32(is_result_nan, d_sig, _mm512_set1_epi32(0x01));

        // Execute long division via repeated subtraction
        avel::Denom16x32u d_denom{vec16x32u{d_sig}};

        __m512i threshold = _mm512_add_epi32(decay(countr_zero(vec16x32u{d_sig})), d_exp);
        __mmask16 loop_mask =
            _mm512_cmp_epi32_mask(r_exp, threshold, _MM_CMPINT_NLE) &
            _mm512_test_epi32_mask(r_sig, r_sig);
        while (loop_mask) {
            vec16x32u quotient = vec16x32u{r_sig} / d_denom;
            vec16x32u subtrahend = quotient * d_denom.value();

            r_sig = _mm512_mask_sub_epi32(r_sig, loop_mask, r_sig, decay(subtrahend));

            __m512i lzcnt = _mm512_lzcnt_epi32(r_sig);
            r_sig = _mm512_mask_sllv_epi32(r_sig, loop_mask, r_sig, lzcnt);
            r_exp = _mm512_mask_sub_epi32(r_exp, loop_mask, r_exp, lzcnt);

            auto a = _mm512_cmp_epi32_mask(r_exp, threshold, _MM_CMPINT_NLE);
            auto b = _mm512_test_epi32_mask(r_sig, r_sig);
            loop_mask = a & b;
        }

        {
            auto m =
                _mm512_cmp_epi32_mask(r_exp, d_exp, _MM_CMPINT_NLE) |
                (
                    _mm512_cmp_epi32_mask(r_exp, d_exp, _MM_CMPINT_EQ) &
                    _mm512_cmp_epi32_mask(r_sig, d_sig, _MM_CMPINT_NLE)
                );

            vec16x32u d_width = bit_width(vec16x32u{d_sig});
            vec16x32u pos_offset = (vec16x32u{32} - (vec16x32u{r_exp} - vec16x32u{d_exp}) - d_width);
            d_denom <<= pos_offset;

            vec16x32u quotient = vec16x32u{r_sig} / d_denom;
            vec16x32u subtrahend = quotient * d_denom.value();

            r_sig = _mm512_mask_sub_epi32(r_sig, m, r_sig, decay(subtrahend));

            __m512i lzcnt = _mm512_lzcnt_epi32(r_sig);
            r_sig = _mm512_mask_sllv_epi32(r_sig, m, r_sig, lzcnt);
            r_exp = _mm512_mask_sub_epi32(r_exp, m, r_exp, lzcnt);
        }

        // Reconstruct result
        __m512 ret = _mm512_range_ps(decay(x), _mm512_setzero_ps(), 0x02);
        ret = _mm512_or_ps(ret, _mm512_cvtepi32_ps(r_sig));

        __m512i exponent_offset = _mm512_sub_epi32(r_exp, _mm512_set1_epi32(30));
        auto tmp = _mm512_cvtepi32_ps(exponent_offset);
        ret = _mm512_scalef_ps(ret, tmp);

        ret = _mm512_mask_blend_ps(is_result_nan, ret, _mm512_set1_ps(NAN));
        return vec16x32f{ret};
    }

    auto vec16x32f_denom32u_avx512dq = vector_test_bench<vec16x32f, vec16x32f, vec16x32f_denom32u_avx512dq_impl>;

    BENCHMARK(fmod_32f::vec16x32f_denom32u_avx512dq);

    #endif



    #if defined(AVEL_AVX512DQ)

    vec16x32f vec16x32f_f64_div_avx512dq_impl(vec16x32f x, vec16x32f y) {
        const auto nans = _mm512_set1_ps(NAN);

        auto does_y_demand_special_handling = ~_mm512_fpclass_ps_mask(decay(y), 0x87);

        auto abs_x = _mm512_mask_abs_ps(nans, does_y_demand_special_handling, decay(x));
        auto xd_lo = _mm512_cvtps_pd(_mm256_castpd_ps(_mm512_extractf64x4_pd(_mm512_castps_pd(abs_x), 0x0)));
        auto xd_hi = _mm512_cvtps_pd(_mm256_castpd_ps(_mm512_extractf64x4_pd(_mm512_castps_pd(abs_x), 0x1)));

        auto abs_y = _mm512_mask_abs_ps(nans, does_y_demand_special_handling, decay(y));
        auto yd_lo = _mm512_cvtps_pd(_mm256_castpd_ps(_mm512_extractf64x4_pd(_mm512_castps_pd(abs_y), 0x0)));
        auto yd_hi = _mm512_cvtps_pd(_mm256_castpd_ps(_mm512_extractf64x4_pd(_mm512_castps_pd(abs_y), 0x1)));

        auto f64_one = _mm512_set1_pd(1.0);
        auto recip_yd_lo = _mm512_div_round_pd(f64_one, yd_lo, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
        auto recip_yd_hi = _mm512_div_round_pd(f64_one, yd_hi, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);

        const auto mask = _mm512_castsi512_pd(_mm512_set1_epi64(0xffffffffff000000ll));
        while (_mm512_cmp_pd_mask(yd_lo, xd_lo, _CMP_LE_OQ)) {
            auto quotient = _mm512_mul_pd(xd_lo, recip_yd_lo);
            auto masked_quotient = _mm512_and_pd(quotient, mask);
            auto trunc_q = _mm512_roundscale_pd(masked_quotient, _MM_FROUND_TO_ZERO);

            xd_lo = _mm512_fnmadd_pd(trunc_q, yd_lo, xd_lo);
        }

        while (_mm512_cmp_pd_mask(yd_hi, xd_hi, _CMP_LE_OQ)) {
            auto quotient = _mm512_mul_pd(xd_hi, recip_yd_hi);
            auto masked_quotient = _mm512_and_pd(quotient, mask);
            auto trunc_q = _mm512_roundscale_pd(masked_quotient, _MM_FROUND_TO_ZERO);

            xd_hi = _mm512_fnmadd_pd(trunc_q, yd_hi, xd_hi);
        }

        // Convert doubles back to floats and pack back into single register
        auto result_lo = _mm512_castps_pd(_mm512_zextps256_ps512(_mm512_cvtpd_ps(xd_lo)));
        auto result_hi = _mm256_castps_pd(_mm512_cvtpd_ps(xd_hi));

        auto result = _mm512_castpd_ps(_mm512_insertf64x4(result_lo, result_hi, 0x01));
        auto ret = avel::copysign(vec16x32f{result}, x);
        return ret;
    }

    auto vec16x32f_f64_div_avx512dq = vector_test_bench<vec16x32f, vec16x32f, vec16x32f_f64_div_avx512dq_impl>;

    BENCHMARK(fmod_32f::vec16x32f_f64_div_avx512dq);

    #endif



    #if defined(AVEL_AVX512DQ)

    vec16x32f vec16x32f_f64_div_fma_avx512dq_impl(vec16x32f x, vec16x32f y) {
        const auto nans = _mm512_set1_ps(NAN);

        auto does_y_demand_special_handling = ~_mm512_fpclass_ps_mask(decay(y), 0x87);

        auto abs_x = _mm512_mask_abs_ps(nans, does_y_demand_special_handling, decay(x));
        auto xd_lo = _mm512_cvtps_pd(_mm256_castpd_ps(_mm512_extractf64x4_pd(_mm512_castps_pd(abs_x), 0x0)));
        auto xd_hi = _mm512_cvtps_pd(_mm256_castpd_ps(_mm512_extractf64x4_pd(_mm512_castps_pd(abs_x), 0x1)));

        auto abs_y = _mm512_mask_abs_ps(nans, does_y_demand_special_handling, decay(y));
        auto yd_lo = _mm512_cvtps_pd(_mm256_castpd_ps(_mm512_extractf64x4_pd(_mm512_castps_pd(abs_y), 0x0)));
        auto yd_hi = _mm512_cvtps_pd(_mm256_castpd_ps(_mm512_extractf64x4_pd(_mm512_castps_pd(abs_y), 0x1)));

        while (_mm512_cmp_pd_mask(yd_lo, xd_lo, _CMP_LE_OQ)) {
            auto quotient = _mm512_div_round_pd(xd_lo, yd_lo, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
            auto trunc_q = _mm512_roundscale_pd(quotient, _MM_FROUND_TO_ZERO);
            xd_lo = _mm512_fnmadd_pd(trunc_q, yd_lo, xd_lo);
        }

        while (_mm512_cmp_pd_mask(yd_hi, xd_hi, _CMP_LE_OQ)) {
            auto quotient = _mm512_div_round_pd(xd_hi, yd_hi, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
            auto trunc_q = _mm512_roundscale_pd(quotient, _MM_FROUND_TO_ZERO);
            xd_hi = _mm512_fnmadd_pd(trunc_q, yd_hi, xd_hi);
        }

        // Convert doubles back to floats and pack back into single register
        auto result_lo = _mm512_castps_pd(_mm512_castps256_ps512(_mm512_cvtpd_ps(xd_lo)));
        auto result_hi = _mm256_castps_pd(_mm512_cvtpd_ps(xd_hi));

        auto result = _mm512_castpd_ps(_mm512_insertf64x4(result_lo, result_hi, 0x01));
        auto ret = avel::copysign(vec16x32f{result}, x);
        return ret;
    }

    auto vec16x32f_f64_div_fma_avx512dq = vector_test_bench<vec16x32f, vec16x32f, vec16x32f_f64_div_fma_avx512dq_impl>;

    BENCHMARK(fmod_32f::vec16x32f_f64_div_fma_avx512dq);

    #endif



    #if defined(AVEL_AVX512DQ)

    vec16x32f vec16x32f_f64_recip_fma_avx512dq_impl(vec16x32f x, vec16x32f y) {
        const auto nans = _mm512_set1_ps(NAN);

        auto does_y_demand_special_handling = ~_mm512_fpclass_ps_mask(decay(y), 0x87);

        auto abs_x = _mm512_mask_abs_ps(nans, does_y_demand_special_handling, decay(x));
        auto xd_lo = _mm512_cvtps_pd(_mm256_castpd_ps(_mm512_extractf64x4_pd(_mm512_castps_pd(abs_x), 0x0)));
        auto xd_hi = _mm512_cvtps_pd(_mm256_castpd_ps(_mm512_extractf64x4_pd(_mm512_castps_pd(abs_x), 0x1)));

        auto abs_y = _mm512_mask_abs_ps(nans, does_y_demand_special_handling, decay(y));
        auto yd_lo = _mm512_cvtps_pd(_mm256_castpd_ps(_mm512_extractf64x4_pd(_mm512_castps_pd(abs_y), 0x0)));
        auto yd_hi = _mm512_cvtps_pd(_mm256_castpd_ps(_mm512_extractf64x4_pd(_mm512_castps_pd(abs_y), 0x1)));

        auto f64_one = _mm512_set1_pd(1.0);
        auto recip_yd_lo = _mm512_div_round_pd(f64_one, yd_lo, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
        auto recip_yd_hi = _mm512_div_round_pd(f64_one, yd_hi, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);

        const auto mask = _mm512_castsi512_pd(_mm512_set1_epi64(0xffffffffff000000ll));
        while (_mm512_cmp_pd_mask(yd_lo, xd_lo, _CMP_LE_OQ)) {
            auto quotient = _mm512_mul_pd(xd_lo, recip_yd_lo);
            auto masked_quotient = _mm512_and_pd(quotient, mask);
            auto trunc_q = _mm512_roundscale_pd(masked_quotient, _MM_FROUND_TO_ZERO);

            xd_lo = _mm512_fnmadd_pd(trunc_q, yd_lo, xd_lo);
        }

        while (_mm512_cmp_pd_mask(yd_hi, xd_hi, _CMP_LE_OQ)) {
            auto quotient = _mm512_mul_pd(xd_hi, recip_yd_hi);
            auto masked_quotient = _mm512_and_pd(quotient, mask);
            auto trunc_q = _mm512_roundscale_pd(masked_quotient, _MM_FROUND_TO_ZERO);

            xd_hi = _mm512_fnmadd_pd(trunc_q, yd_hi, xd_hi);
        }

        // Convert doubles back to floats and pack back into single register
        auto result_lo = _mm512_castps_pd(_mm512_zextps256_ps512(_mm512_cvtpd_ps(xd_lo)));
        auto result_hi = _mm256_castps_pd(_mm512_cvtpd_ps(xd_hi));

        auto result = _mm512_castpd_ps(_mm512_insertf64x4(result_lo, result_hi, 0x01));
        auto ret = avel::copysign(vec16x32f{result}, x);
        return ret;
    }

    auto vec16x32f_f64_recip_fma_avx512dq = vector_test_bench<vec16x32f, vec16x32f, vec16x32f_f64_recip_fma_avx512dq_impl>;

    BENCHMARK(fmod_32f::vec16x32f_f64_recip_fma_avx512dq);

    #endif

}

#endif //AVEL_FMOD_F32_HPP
