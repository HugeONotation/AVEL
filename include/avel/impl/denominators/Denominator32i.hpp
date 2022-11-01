#ifndef AVEL_DENOMINATOR32I_HPP
#define AVEL_DENOMINATOR32I_HPP

namespace avel {

    using Denom32i = Denominator<std::int32_t>;

    template<>
    class alignas(16) Denominator<std::int32_t> {
    public:

        template<class U>
        friend class Denominator;

        //=================================================
        // -ctors
        //=================================================

        explicit Denominator(std::int32_t d):
            Denominator(d, max(32 - countl_zero(abs(d) - 1), 1)) {}

    private:

        explicit Denominator(std::int32_t d, std::int32_t l):
            mp((std::int64_t(0x80000000) << l) / abs(d) - 0xffffffff),
            d_sign(d >> 31),
            sh(l - 1),
            d(d) {}

    public:

        //=================================================
        // Arithmetic Operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend avel::div_type<std::int32_t> div(std::int32_t n, Denominator denom) {
            std::int32_t q0 = n + (std::int64_t(denom.mp) * std::int64_t(n) >> 32);
            q0 = (q0 >> denom.sh) - (n >> 31);
            std::int32_t q = (q0 ^ denom.d_sign) - denom.d_sign;
            std::int32_t r = n - (q * denom.d);
            return {q, r};
        }

        [[nodiscard]]
        AVEL_FINL friend std::int32_t operator/(std::int32_t lhs, Denominator rhs) {
            return div(lhs, rhs).quot;
        }

        [[nodiscard]]
        AVEL_FINL friend std::int32_t operator%(std::int32_t lhs, Denominator rhs) {
            return div(lhs, rhs).rem;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        std::int32_t mp = 0;
        std::int32_t d_sign = 0;
        std::uint32_t sh = 0;
        std::uint32_t d = 0;

    };

    AVEL_FINL std::int32_t& operator/=(std::int32_t& lhs, Denom32i rhs) {
        lhs = lhs / rhs;
        return lhs;
    }

    AVEL_FINL std::int32_t& operator%=(std::int32_t& lhs, Denom32i rhs) {
        lhs = lhs % rhs;
        return lhs;
    }

}

#endif //AVEL_DENOMINATOR32I_HPP
