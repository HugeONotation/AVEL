#ifndef AVEL_SCALAR32f_HPP
#define AVEL_SCALAR32f_HPP

#include <math.h>

namespace avel {

    template<class T>
    class Denominator;

    //=====================================================
    // Forward declarations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL float trunc(float x);

    [[nodiscard]]
    AVEL_FINL float round(float x);

    [[nodiscard]]
    AVEL_FINL float copysign(float mag, float sgn);

    [[nodiscard]]
    AVEL_FINL bool isnan(float v);

    [[nodiscard]]
    AVEL_FINL bool isinf(float arg);

    [[nodiscard]]
    AVEL_FINL bool isunordered(float x, float y);

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL float keep(bool m, float v) {
        if (m) {
            return v;
        } else {
            return 0.0f;
        }
    }

    [[nodiscard]]
    AVEL_FINL float clear(bool m, float v) {
        if (m) {
            return 0.0f;
        } else {
            return v;
        }
    }

    [[nodiscard]]
    AVEL_FINL float blend(bool m, float a, float b) {
        if (m) {
            return a;
        } else {
            return b;
        }
    }

    [[nodiscard]]
    AVEL_FINL float byteswap(float v) {
        return bit_cast<float>(byteswap(bit_cast<std::uint32_t>(v)));
    }

    [[nodiscard]]
    AVEL_FINL float max(float x, float y) {
        if (x < y) {
            return y;
        } else {
            return x;
        }
    }

    [[nodiscard]]
    AVEL_FINL float min(float x, float y) {
        if (x < y) {
            return x;
        } else {
            return y;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::array<float, 2> minmax(float x, float y) {
        if (x < y) {
            return {x, y};
        } else {
            return {y, x};
        }
    }

    [[nodiscard]]
    AVEL_FINL float clamp(float x, float lo, float hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL float negate(bool m, float x) {
        //TODO: Manual approach?
        if (m) {
            return -x;
        } else {
            return x;
        }
    }

    [[nodiscard]]
    AVEL_FINL float abs(float x) {
        #if defined(AVEL_SSE2) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))
        __asm__(
            "pslld $1, %[x]\n"
            "psrld $1, %[x]"
            : // Outputs
            [x] "+x"(x)
            : // Inputs
        );

        return x;

        #else
        return bit_cast<float>(bit_cast<std::int32_t>(x) & 0x7fffffff);

        #endif
    }

    [[nodiscard]]
    AVEL_FINL float neg_abs(float x) {
        #if defined(AVEL_SSE2) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))
        __asm__(
            "orps %[y], %[x]"
            : // Outputs
            [x] "+x"(x)
            : // Inputs
            [y] "x"(-0.0f)
        );

        return x;

        #else
        return bit_cast<float>(bit_cast<std::int32_t>(x) | 0x80000000);

        #endif
    }

    //=====================================================
    // General Floating-Point Operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL float fmax(float a, float b) {
        #if defined(AVEL_AVX512DQ) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))
        __asm__(
            "vrangess $5, %[y], %[x], %[x]"
            : // Outputs
            [x] "+x"(a)
            : // Inputs
            [y] "x"(b)
        );

        return a;

        #elif defined(AVEL_AVX) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))
        float scratch_x;
        float scratch_y;

        __asm__(
            "vmaxss    %[b], %[a], %[x] \n"
            "vxorps    %[y], %[y], %[y] \n"
            "vcmpss     $3, %[y], %[b], %[b]\n"
            "vblendvps %[b], %[a], %[x], %[a]\n"
            : // Outputs
            [a] "+x"(a),
            [b] "+x"(b),
            [x] "+x"(scratch_x),
            [y] "+x"(scratch_y)
            : // Inputs
        );

        return a;

        #elif defined(AVEL_SSE4_1) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))
        float scratch_x;
        float scratch_y;

        register float a_input asm("xmm0") = a;

        __asm__(
            "movaps   %%xmm0, %[x]\n"
            "movaps   %%xmm0, %[y]\n"
            "maxss    %[b], %[y]\n"
            "xorps    %%xmm0, %%xmm0\n"
            "cmpss    $3, %[b], %%xmm0\n"
            "blendvps %%xmm0, %[x], %[y]"

            : // Outputs
            [a] "+x"(a_input),
            [b] "+x"(b),
            [x] "+x"(scratch_x),
            [y] "+x"(scratch_y)
            : // Inputs
        );

        return scratch_y;

        #elif defined(AVEL_SSE2) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))
        float scratch_x;
        float scratch_y;

        __asm__(
            "movaps  %[a], %[x]\n"
            "movaps  %[a], %[y]\n"
            "maxss   %[b], %[y]\n"
            "cmpss   $3, %[b], %[b] \n"
            "movaps   %[b], %[a]\n"
            "andps    %[x], %[b]\n"
            "andnps   %[y], %[a]\n"
            "orps     %[b], %[a]"
            : // Outputs
            [a] "+x"(a),
            [b] "+x"(b),
            [x] "+x"(scratch_x),
            [y] "+x"(scratch_y)
            : // Inputs
        );

        return a;

        #else
        if (avel::isnan(a)) {
            return b;
        }

        if (avel::isnan(b)) {
            return a;
        }

        if (a > b) {
            return a;
        } else {
            return b;
        }

        #endif
    }

    [[nodiscard]]
    AVEL_FINL float fmin(float a, float b) {
        #if defined(AVEL_AVX512DQ) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))
        __asm__(
            "vrangess $4, %[y], %[x], %[x]"
            : // Outputs
            [x] "+x"(a)
            : // Inputs
            [y] "x"(b)
        );

        return a;

        #elif defined(AVEL_AVX) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))
        float scratch_x;
        float scratch_y;

        __asm__(
            "vminss    %[b], %[a], %[x] \n"
            "vxorps    %[y], %[y], %[y] \n"
            "vcmpss     $3, %[y], %[b], %[b]\n"
            "vblendvps %[b], %[a], %[x], %[a]\n"
            : // Outputs
            [a] "+x"(a),
            [b] "+x"(b),
            [x] "+x"(scratch_x),
            [y] "+x"(scratch_y)
            : // Inputs
        );

        return a;

        #elif defined(AVEL_SSE4_1) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))
        float scratch_x;
        float scratch_y;

        register float a_input asm("xmm0") = a;

        __asm__(
            "movaps   %%xmm0, %[x]\n"
            "movaps   %%xmm0, %[y]\n"
            "minss    %[b], %[y]\n"
            "xorps    %%xmm0, %%xmm0\n"
            "cmpss    $3, %[b], %%xmm0\n"
            "blendvps %%xmm0, %[x], %[y]"

            : // Outputs
            [a] "+x"(a_input),
            [b] "+x"(b),
            [x] "+x"(scratch_x),
            [y] "+x"(scratch_y)
            : // Inputs
        );

        return scratch_y;

        #elif defined(AVEL_SSE2) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))
        float scratch_x;
        float scratch_y;

        __asm__(
            "movaps  %[a], %[x]\n"
            "movaps  %[a], %[y]\n"
            "minss   %[b], %[y]\n"
            "cmpss   $3, %[b], %[b] \n"
            "movaps   %[b], %[a]\n"
            "andps    %[x], %[b]\n"
            "andnps   %[y], %[a]\n"
            "orps     %[b], %[a]"
            : // Outputs
            [a] "+x"(a),
            [b] "+x"(b),
            [x] "+x"(scratch_x),
            [y] "+x"(scratch_y)
            : // Inputs
        );

        return a;

        #else
        if (avel::isnan(a)) {
            return b;
        }

        if (avel::isnan(b)) {
            return a;
        }

        if (a < b) {
            return a;
        } else {
            return b;
        }

        #endif
    }

    [[nodiscard]]
    AVEL_FINL float fdim(float x, float y) {
        return avel::max(x - y, 0.0f);
    }

    [[nodiscard]]
    AVEL_FINL float fmod(float x, float y) {
        return std::fmod(x, y);

        /*
        if (avel::isunordered(x, y) | avel::isinf(x) | y == 0.0f) {
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

        //TODO: Test this
        */
    }

    [[nodiscard]]
    AVEL_FINL float frac(float x) {
        //TODO: Consider lower-level implementation
        if ((x == 0.0) || (x != x)) {
            return x;
        }

        if (avel::abs(x) == INFINITY) {
            return NAN;
        }

        return x - avel::trunc(x);
    }

    //=====================================================
    // Power Functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL float sqrt(float v) {
        return std::sqrt(v);
    }

    //=====================================================
    // Nearest Integer Functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL float ceil(float arg) {
        //TODO: Consider lower-level implementation
        return std::ceil(arg);
    }

    [[nodiscard]]
    AVEL_FINL float floor(float arg) {
        //TODO: Consider lower-level implementation
        return std::floor(arg);
    }

    [[nodiscard]]
    AVEL_FINL float trunc(float arg) {
        //TODO: Consider lower-level implementation
        return std::trunc(arg);
    }

    [[nodiscard]]
    AVEL_FINL float round(float arg) {
        //TODO: Consider lower-level implementation
        return std::round(arg);
    }

    [[nodiscard]]
    AVEL_FINL float nearbyint(float arg) {
        //TODO: Consider lower-level implementation
        return std::nearbyint(arg);
    }

    [[nodiscard]]
    AVEL_FINL float rint(float arg) {
        //TODO: Consider lower-level implementation
        return std::rint(arg);
    }

    //=====================================================
    // Floating-point manipulation functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL float frexp(float arg, std::int32_t* exp) {
        //TODO: Consider lower-level implementation
        return std::frexp(arg, exp);
        /*
        if (arg == 0.0) {
            *exp = 0;
            return arg;
        }

        auto bits = bit_cast<std::uint32_t>(arg);
        auto exponent_bits = (bits >> 23) & 0xFF;

        if (exponent_bits == 255) {
            return arg;
        }

        auto target_exponent = (126 << 23);
        auto ret = bits & (0x807FFFFF) | target_exponent;

        if (exponent_bits != 0) {
            *exp = exponent_bits - 126;
            return bit_cast<float>(ret);
        } else {
            auto mantissa_mask = 0x007FFFFF;
            auto significand = bits & mantissa_mask;
            auto offset = countl_zero(significand) - 8;
            ret &= 0xFF800000;
            ret |= mantissa_mask & (significand << offset);
            *exp = -125 - offset;
            return bit_cast<float>(ret);
        }
        */
    }

    [[nodiscard]]
    AVEL_FINL float ldexp(float arg, std::int32_t exp) {
        //TODO: Consider lower-level implementation
        return std::ldexp(arg, exp);

        /*
        if (arg == 0.0f || exp == 0) {
            return arg;
        }

        auto bits = bit_cast<std::uint32_t>(arg);
        auto exponent = bits >> 23 & 0xFF;
        if (exponent == 255) {
            return arg;
        } else if (exponent == 0) {
            //Subnormal number
        } else {
            //Normal number
        }
        */
    }

    [[nodiscard]]
    AVEL_FINL float modf(float num, float* iptr) {
        //TODO: Consider lower-level implementation
        return std::modf(num, iptr);
    }

    [[nodiscard]]
    AVEL_FINL float scalbn(float x, std::int32_t exp) {
        return ldexp(x, exp);
    }

    [[nodiscard]]
    AVEL_FINL std::int32_t ilogb(float arg) {
        //TODO: Consider lower-level implementation
        return std::ilogb(arg);

        /*
        if (arg == 0.0f) {
            return FP_ILOGB0;
        }

        if (std::isinf(arg)) {
            return 0x7FFFFFFF;
        }

        if (isnan(arg)) {
            return FP_ILOGBNAN;
        }

        auto bits = bit_cast<std::int32_t>(arg);
        bits >>= 23;
        bits &= 0xFF;

        if (bits > 0) {
            return bits - 127;
        } else {
            auto offset = countl_zero(bits << 9);
            return -127 - offset;
        }
        */
    }

    [[nodiscard]]
    AVEL_FINL float logb(float arg) {
        //TODO: Manually implement
        return std::logb(arg);
    }

    [[nodiscard]]
    AVEL_FINL float copysign(float mag, float sgn) {
        #if defined(AVEL_AVX512F) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))
        __asm__(
            "vpternlogd $228, %[m], %[y], %[x]\n"
            : // Outputs
            [x] "+x"(mag)
            : // Inputs
            [y] "x"(sgn),
            [m] "x"(avel::bit_cast<float>(0x7fffffff))
        );

        return mag;

        #elif defined(AVEL_AVX) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))
        __asm__(
            "vpand %[m], %[x], %[x]\n"
            "vpandn %[y], %[m], %[m]\n"
            "vpor %[m], %[x], %[x]\n"
            : // Outputs
            [x] "+x"(mag)
            : // Inputs
            [y] "x"(sgn),
            [m] "x"(avel::bit_cast<float>(0x7fffffff))
        );

        return mag;

        #elif defined(AVEL_SSE2) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))
        __asm__(
            "pand %[m], %[x]\n"
            "pandn %[y], %[m]\n"
            "por %[m], %[x]\n"
            : // Outputs
            [x] "+x"(mag),
            [y] "+x"(sgn)
            : // Inputs
            [m] "x"(avel::bit_cast<float>(0x7fffffff))
        );

        return mag;

        #else
        auto mag_bits = avel::bit_cast<std::uint32_t>(mag);
        auto sgn_bits = avel::bit_cast<std::uint32_t>(sgn);

        mag_bits ^= (sgn_bits ^ mag_bits) & 0x80000000;
        return avel::bit_cast<float>(mag_bits);

        #endif
    }

    //=====================================================
    // Floating-point classification
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::int32_t fpclassify(float arg) {
        //TODO: Consider lower-level implementation
        std::uint32_t bits = bit_cast<std::int32_t>(arg);
        std::uint32_t bits2 = bits + bits;

        if (bits2 == 0x00000000) {
            return FP_ZERO;
        }

        if (bits2 < 0x01000000) {
            return FP_SUBNORMAL;
        }

        if (bits2 < 0xff000000) {
            return FP_NORMAL;
        }

        if (bits2 == 0xff000000) {
            return FP_INFINITE;
        }

        return FP_NAN;
    }

    [[nodiscard]]
    AVEL_FINL bool isfinite(float arg) {
        std::uint32_t arg_bits = bit_cast<std::uint32_t>(arg);
        std::uint32_t abs_arg = arg_bits & 0x7fffffff;
        return abs_arg < 0x7f800000;
    }

    [[nodiscard]]
    AVEL_FINL bool isinf(float arg) {
        std::uint32_t arg_bits = bit_cast<std::uint32_t>(arg);
        return (arg_bits == 0x7f800000) | (arg_bits == 0xff800000);
    }

    [[nodiscard]]
    AVEL_FINL bool isnan(float arg) {
        return arg != arg;
    }

    [[nodiscard]]
    AVEL_FINL bool isnormal(float arg) {
        std::uint32_t arg_bits = avel::bit_cast<std::uint32_t>(arg);
        std::uint32_t arg_bits2 = arg_bits + arg_bits;
        return (0x1000000 <= arg_bits2) & (arg_bits2 < 0xff000000);
    }

    [[nodiscard]]
    AVEL_FINL bool signbit(float arg) {
        return std::signbit(arg);
    }

    //=====================================================
    // Floating-point comparisons
    //=====================================================

    [[nodiscard]]
    AVEL_FINL bool isgreater(float x, float y) {
        return std::isgreater(x, y);
    }

    [[nodiscard]]
    AVEL_FINL bool isgreaterequal(float x, float y) {
        return std::isgreaterequal(x, y);
    }

    [[nodiscard]]
    AVEL_FINL bool isless(float x, float y) {
        return std::isless(x, y);
    }

    [[nodiscard]]
    AVEL_FINL bool islessequal(float x, float y) {
        return std::islessequal(x, y);
    }

    [[nodiscard]]
    AVEL_FINL bool islessgreater(float x, float y) {
        return std::islessgreater(x, y);
    }

    [[nodiscard]]
    AVEL_FINL bool isunordered(float x, float y) {
        return std::isunordered(x, y);
    }

}

#endif //AVEL_IMPL_SCALAR32f_HPP
