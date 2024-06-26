#ifndef AVEL_SCALAR64I_HPP
#define AVEL_SCALAR64I_HPP

namespace avel {

    //=====================================================
    // Forward declarations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::int64_t abs(std::int64_t x);

    //=====================================================
    // Bit manipulation
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::int64_t set_bits<std::int64_t>(bool x) {
        return -std::int64_t(x);
    }

    [[nodiscard]]
    AVEL_FINL std::int64_t popcount(std::int64_t x) {
        return std::int64_t(popcount(std::uint64_t(x)));
    }

    [[nodiscard]]
    AVEL_FINL std::int64_t byteswap(std::int64_t x) {
        return std::int64_t(byteswap(std::uint64_t(x)));
    }

    [[nodiscard]]
    AVEL_FINL std::int64_t countl_zero(std::int64_t x) {
        return std::int64_t(countl_zero(std::uint64_t(x)));
    }

    [[nodiscard]]
    AVEL_FINL std::int64_t countl_one(std::int64_t x) {
        return std::int64_t(countl_zero(std::uint64_t(~x)));
    }

    [[nodiscard]]
    AVEL_FINL std::int64_t countr_zero(std::int64_t x) {
        return std::int64_t(countr_zero(std::uint64_t(x)));
    }

    [[nodiscard]]
    AVEL_FINL std::int64_t countr_one(std::int64_t x) {
        return std::int64_t(countr_one(std::uint64_t(x)));
    }

    [[nodiscard]]
    AVEL_FINL std::int64_t countl_sign(std::int64_t x) {
        #if defined(AVEL_ARM) && (defined(AVEL_GCC) || defined(AVEL_CLANG))
        std::int64_t ret;
        __asm__(
            "cls %[r], %[x]"
            : // Outputs
            [r] "=r"(ret)
            : // Inputs
            [x] "r"(x)
        );
        return ret;

        #else
        return avel::countl_zero(std::uint64_t(x ^ (x >> 1))) - 1;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::int64_t bit_width(std::int64_t x) {
        return bit_width(std::uint64_t(x));
    }

    [[nodiscard]]
    AVEL_FINL bool has_single_bit(std::int64_t v) {
        return has_single_bit(std::uint64_t(v));
    }

    [[nodiscard]]
    AVEL_FINL std::int64_t rotl(std::int64_t x, long long s) {
        return std::int64_t(rotl(std::uint64_t(x), std::uint64_t(s)));
    }

    [[nodiscard]]
    AVEL_FINL std::int64_t rotr(std::int64_t x, long long s) {
        return std::int64_t(rotr(std::uint64_t(x), std::uint64_t(s)));
    }

    //=====================================================
    // General operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::int64_t negate(bool m, std::int64_t x) {
        std::int64_t mask = -m;
        return (x ^ mask) - mask;
    }

    [[nodiscard]]
    AVEL_FINL std::int64_t abs(std::int64_t x) {
        if (x < 0) {
            return -x;
        } else {
            return x;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::int64_t neg_abs(std::int64_t x) {
        if (x < 0) {
            return x;
        } else {
            return -x;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::int64_t keep(bool m, std::int64_t x) {
        if (m) {
            return x;
        } else {
            return 0;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::int64_t clear(bool m, std::int64_t x) {
        if (m) {
            return 0;
        } else {
            return x;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::int64_t blend(bool m, std::int64_t a, std::int64_t b) {
        if (m) {
            return a;
        } else {
            return b;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::int64_t min(std::int64_t a, std::int64_t b) {
        if (a < b) {
            return a;
        } else {
            return b;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::int64_t max(std::int64_t a, std::int64_t b) {
        if (a < b) {
            return b;
        } else {
            return a;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::array<std::int64_t, 2> minmax(std::int64_t a, std::int64_t b) {
        if (a < b) {
            return {a, b};
        } else {
            return {b, a};
        }
    }

    [[nodiscard]]
    AVEL_FINL std::int64_t clamp(std::int64_t x, std::int64_t lo, std::int64_t hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL std::int64_t midpoint(std::int64_t a, std::int64_t b) {
        std::int64_t avg = ((a ^ b) >> 1) + (a & b);
        std::int64_t bias = (b < a) & (a ^ b) & 0x1;

        return avg + bias;
    }

    [[nodiscard]]
    AVEL_FINL std::int64_t average(std::int64_t a, std::int64_t b) {
        #if defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICX)
        return (__int128(a) + __int128(b)) / 2;

        #else
        std::int64_t avg = (a & b) + ((a ^ b) >> 1);
        std::int64_t c = (a < -b) & (a ^ b);

        return avg + c;
        #endif
    }

    //=====================================================
    // Comparison operators
    //=====================================================

    [[nodiscard]]
    AVEL_FINL bool cmp_equal(std::uint64_t x, std::int64_t y) {
        return (x == y) && !(y < 0);
    }

    [[nodiscard]]
    AVEL_FINL bool cmp_equal(std::int64_t x, std::uint64_t y) {
        return (x == y) && !(x < 0);
    }

    [[nodiscard]]
    AVEL_FINL bool cmp_not_equal(std::uint64_t x, std::int64_t y) {
        return (x != y) || (y < 0);
    }

    [[nodiscard]]
    AVEL_FINL bool cmp_not_equal(std::int64_t x, std::uint64_t y) {
        return (x != y) || (x < 0);
    }

    [[nodiscard]]
    AVEL_FINL bool cmp_less(std::uint64_t x, std::int64_t y) {
        auto a = std::int64_t(x) < std::int64_t(y);
        auto b = std::int64_t(x | y) < 0;
        return !b && a;
    }

    [[nodiscard]]
    AVEL_FINL bool cmp_less(std::int64_t x, std::uint64_t y) {
        auto a = std::int64_t(x) < std::int64_t(y);
        auto b = std::int64_t(x | y) < 0;
        return b || a;
    }

    [[nodiscard]]
    AVEL_FINL bool cmp_less_equal(std::uint64_t x, std::int64_t y) {
        auto a = std::int64_t(x) <= std::int64_t(y);
        auto b = std::int64_t(x | y) < 0;
        return !b && a;
    }

    [[nodiscard]]
    AVEL_FINL bool cmp_less_equal(std::int64_t x, std::uint64_t y) {
        auto a = std::int64_t(x) <= std::int64_t(y);
        auto b = std::int64_t(x | y) < 0;
        return b || a;
    }

    [[nodiscard]]
    AVEL_FINL bool cmp_greater(std::uint64_t x, std::int64_t y) {
        return cmp_less(y, x);
    }

    [[nodiscard]]
    AVEL_FINL bool cmp_greater(std::int64_t x, std::uint64_t y) {
        return cmp_less(y, x);
    }

    [[nodiscard]]
    AVEL_FINL bool cmp_greater_equal(std::uint64_t x, std::int64_t y) {
        return cmp_less_equal(y, x);
    }

    [[nodiscard]]
    AVEL_FINL bool cmp_greater_equal(std::int64_t x, std::uint64_t y) {
        return cmp_less_equal(y, x);
    }

}

#endif //AVEL_SCALAR64I_HPP
