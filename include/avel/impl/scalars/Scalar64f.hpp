#ifndef AVEL_SCALAR64F_HPP
#define AVEL_SCALAR64F_HPP

namespace avel {

    //=====================================================
    // Forward declarations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL double trunc(double x);

    [[nodiscard]]
    AVEL_FINL double round(double x);

    [[nodiscard]]
    AVEL_FINL double copysign(double mag, double sgn);

    [[nodiscard]]
    AVEL_FINL bool isnan(double v);

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL double keep(bool m, double v) {
        if (m) {
            return v;
        } else {
            return 0.0f;
        }
    }

    [[nodiscard]]
    AVEL_FINL double clear(bool m, double v) {
        if (m) {
            return 0.0f;
        } else {
            return v;
        }
    }

    [[nodiscard]]
    AVEL_FINL double blend(bool m, double a, double b) {
        if (m) {
            return a;
        } else {
            return b;
        }
    }

    [[nodiscard]]
    AVEL_FINL double byteswap(double x) {
        return bit_cast<double>(byteswap(bit_cast<std::uint64_t>(x)));
    }

    [[nodiscard]]
    AVEL_FINL double max(double x, double y) {
        if (x < y) {
            return y;
        } else {
            return x;
        }
    }

    [[nodiscard]]
    AVEL_FINL double min(double x, double y) {
        if (x < y) {
            return x;
        } else {
            return y;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::array<double, 2> minmax(double x, double y) {
        if (x < y) {
            return {x, y};
        } else {
            return {y, x};
        }
    }

    [[nodiscard]]
    AVEL_FINL double clamp(double x, double lo, double hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL double negate(bool m, double x) {
        //TODO: Manual approach?
        if (m) {
            return -x;
        } else {
            return x;
        }
    }

    [[nodiscard]]
    AVEL_FINL double abs(double x) {
        return bit_cast<double>(bit_cast<std::int64_t>(x) & 0x7fffffffffffffffull);
    }

    [[nodiscard]]
    AVEL_FINL double neg_abs(double x) {
        return bit_cast<double>(bit_cast<std::int64_t>(x) | 0x8000000000000000ull);
    }

    //=====================================================
    // Cmath functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL double fmax(double a, double b) {
        #if defined(AVEL_AVX512DQ) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))
        __asm__(
            "vrangesd $5, %[y], %[x], %[x]"
            : // Outputs
            [x] "+x"(a)
            : // Inputs
            [y] "x"(b)
        );

        return a;

        #elif defined(AVEL_AVX) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))
        double scratch_x;
        double scratch_y;

        __asm__(
            "vmaxsd    %[b], %[a], %[x] \n"
            "vxorps    %[y], %[y], %[y] \n"
            "vcmpsd     $3, %[y], %[b], %[b]\n"
            "vblendvpd %[b], %[a], %[x], %[a]\n"
            : // Outputs
            [a] "+x"(a),
            [b] "+x"(b),
            [x] "+x"(scratch_x),
            [y] "+x"(scratch_y)
            : // Inputs
        );

        return a;

        #elif defined(AVEL_SSE4_1) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))
        double scratch_x;
        double scratch_y;

        register double a_input asm("xmm0") = a;

        __asm__(
            "movaps   %%xmm0, %[x]\n"
            "movaps   %%xmm0, %[y]\n"
            "maxsd    %[b], %[y]\n"
            "xorps    %%xmm0, %%xmm0\n"
            "cmpsd    $3, %[b], %%xmm0\n"
            "blendvpd %%xmm0, %[x], %[y]"

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
            "maxsd   %[b], %[y]\n"
            "cmpsd   $3, %[b], %[b] \n"
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
    AVEL_FINL double fmin(double a, double b) {
        #if defined(AVEL_AVX512DQ) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))
        __asm__(
            "vrangesd $4, %[y], %[x], %[x]"
            : // Outputs
            [x] "+x"(a)
            : // Inputs
            [y] "x"(b)
        );

        return a;

        #elif defined(AVEL_AVX) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))
        double scratch_x;
        double scratch_y;

        __asm__(
            "vminsd    %[b], %[a], %[x] \n"
            "vxorps    %[y], %[y], %[y] \n"
            "vcmpsd     $3, %[y], %[b], %[b]\n"
            "vblendvpd %[b], %[a], %[x], %[a]\n"
            : // Outputs
            [a] "+x"(a),
            [b] "+x"(b),
            [x] "+x"(scratch_x),
            [y] "+x"(scratch_y)
            : // Inputs
        );

        return a;

        #elif defined(AVEL_SSE4_1) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))
        double scratch_x;
        double scratch_y;

        register double a_input asm("xmm0") = a;

        __asm__(
            "movaps   %%xmm0, %[x]\n"
            "movaps   %%xmm0, %[y]\n"
            "minsd    %[b], %[y]\n"
            "xorps    %%xmm0, %%xmm0\n"
            "cmpsd    $3, %[b], %%xmm0\n"
            "blendvpd %%xmm0, %[x], %[y]"

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
            "minsd   %[b], %[y]\n"
            "cmpsd   $3, %[b], %[b] \n"
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
    AVEL_FINL double fdim(double x, double y) {
        return avel::max(x - y, 0.0);
    }

    [[nodiscard]]
    AVEL_FINL double fmod(double x, double y) {
        return std::fmod(x, y);

        /*
        if (avel::isnan(x) | avel::isnan(y) | avel::isinf(x) | y == 0.0f) {
            return NAN;
        }

        if (avel::isinf(y)) {
            return x;
        }

        if (avel::abs(x) < avel::abs(y)) {
            return x;
        }

        //TODO: Complete implementation
        return 0.0f;
        */
    }

    [[nodiscard]]
    AVEL_FINL double frac(double x) {
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
    AVEL_FINL double sqrt(double v) {
        return std::sqrt(v);
    }

    //=====================================================
    // Nearest Integer Functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL double ceil(double arg) {
        //TODO: Consider lower-level implementation
        return std::ceil(arg);
    }

    [[nodiscard]]
    AVEL_FINL double floor(double arg) {
        //TODO: Consider lower-level implementation
        return std::floor(arg);
    }

    [[nodiscard]]
    AVEL_FINL double trunc(double arg) {
        //TODO: Consider lower-level implementation
        return std::trunc(arg);
    }

    [[nodiscard]]
    AVEL_FINL double round(double arg) {
        //TODO: Consider lower-level implementation
        return std::round(arg);
    }

    [[nodiscard]]
    AVEL_FINL double nearbyint(double arg) {
        //TODO: Consider lower-level implementation
        return std::nearbyint(arg);
    }

    [[nodiscard]]
    AVEL_FINL double rint(double arg) {
        //TODO: Consider lower-level implementation
        return std::rint(arg);
    }

    //=====================================================
    // floating-point manipulation functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL double frexp(double arg, std::int64_t* exp) {
        //TODO: Consider lower-level implementation
        std::int32_t e;
        auto ret = std::frexp(arg, &e);
        *exp = e;
        return ret;
    }

    [[nodiscard]]
    AVEL_FINL double ldexp(double arg, std::int64_t exp) {
        //TODO: Consider lower-level implementation
        exp = avel::clamp(exp, std::int64_t(INT_MIN), std::int64_t(INT_MAX));
        return std::ldexp(arg, exp);
    }

    [[nodiscard]]
    AVEL_FINL double modf(double num, double* iptr) {
        //TODO: Consider lower-level implementation
        return std::modf(num, iptr);
    }

    [[nodiscard]]
    AVEL_FINL double scalbn(double x, std::int64_t exp) {
        return avel::ldexp(x, exp);
    }

    [[nodiscard]]
    AVEL_FINL std::int64_t ilogb(double arg) {
        //TODO: Consider lower-level implementation
        return static_cast<std::int64_t>(std::ilogb(arg));
    }

    [[nodiscard]]
    AVEL_FINL double logb(double arg) {
        return std::logb(arg);
    }

    [[nodiscard]]
    AVEL_FINL double copysign(double mag, double sgn) {
        #if defined(AVEL_AVX512F) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))
        __asm__(
            "vpternlogq $228, %[m], %[y], %[x]\n"
            : // Outputs
            [x] "+x"(mag)
            : // Inputs
            [y] "x"(sgn),
            [m] "x"(avel::bit_cast<double>(0x7fffffffffffffff))
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
            [m] "x"(avel::bit_cast<double>(0x7fffffffffffffff))
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
            [m] "x"(avel::bit_cast<double>(0x7fffffffffffffff))
        );

        return mag;

        #else
        auto mag_bits = avel::bit_cast<std::uint64_t>(mag);
        auto sgn_bits = avel::bit_cast<std::uint64_t>(sgn);

        mag_bits ^= (sgn_bits ^ mag_bits) & 0x8000000000000000ull;
        return avel::bit_cast<double>(mag_bits);

        #endif
    }

    //=====================================================
    // doubleing-point classification
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::int64_t fpclassify(double arg) {
        //TODO: Consider lower-level implementation
        std::uint64_t arg_bits = avel::bit_cast<std::uint64_t>(arg);
        std::uint64_t bits2 = arg_bits + arg_bits;

        if (bits2 == 0x0000000000000000) {
            return FP_ZERO;
        }

        if (bits2 < 0x20000000000000) {
            return FP_SUBNORMAL;
        }

        if (bits2 < 0xffe0000000000000) {
            return FP_NORMAL;
        }

        if (bits2 == 0xffe0000000000000) {
            return FP_INFINITE;
        }

        return FP_NAN;
    }

    [[nodiscard]]
    AVEL_FINL bool isfinite(double arg) {
        std::uint64_t arg_bits = avel::bit_cast<std::uint64_t>(arg);
        std::uint64_t abs_arg = arg_bits & 0x7fffffffffffffff;
        return abs_arg < 0x7ff0000000000000;
    }

    [[nodiscard]]
    AVEL_FINL bool isinf(double arg) {
        std::uint64_t arg_bits = avel::bit_cast<std::uint64_t>(arg);
        return (arg_bits == 0x7ff0000000000000) | (arg_bits == 0xfff0000000000000);
    }

    [[nodiscard]]
    AVEL_FINL bool isnan(double arg) {
        return arg != arg;
    }

    [[nodiscard]]
    AVEL_FINL bool isnormal(double arg) {
        std::uint64_t arg_bits = avel::bit_cast<std::uint64_t>(arg);
        std::uint64_t arg_bits2 = arg_bits + arg_bits;
        return (0x20000000000000 <= arg_bits2) & (arg_bits2 < 0xffe0000000000000);
    }

    [[nodiscard]]
    AVEL_FINL bool signbit(double arg) {
        return avel::bit_cast<std::int64_t>(arg) & 0x8000000000000000ull;
    }

    //=====================================================
    // doubleing-point comparisons
    //=====================================================

    [[nodiscard]]
    AVEL_FINL bool isgreater(double x, double y) {
        return std::isgreater(x, y);
    }

    [[nodiscard]]
    AVEL_FINL bool isgreaterequal(double x, double y) {
        return std::isgreaterequal(x, y);
    }

    [[nodiscard]]
    AVEL_FINL bool isless(double x, double y) {
        return std::isless(x, y);
    }

    [[nodiscard]]
    AVEL_FINL bool islessequal(double x, double y) {
        return std::islessequal(x, y);
    }

    [[nodiscard]]
    AVEL_FINL bool islessgreater(double x, double y) {
        return std::islessgreater(x, y);
    }

    [[nodiscard]]
    AVEL_FINL bool isunordered(double x, double y) {
        return std::isunordered(x, y);
    }

}

#endif //AVEL_SCALAR64F_HPP
