#ifndef AVEL_DENOMINATOR32U_HPP
#define AVEL_DENOMINATOR32U_HPP

namespace avel {

    using Denom32u = Denominator<std::uint32_t>;

    template<>
    class alignas(16) Denominator<std::uint32_t> {
    public:

        template<class U>
        friend class Denominator;

        //=================================================
        // -ctors
        //=================================================

        explicit Denominator(std::uint32_t d):
            Denominator(d, 32 - countl_zero(d - 1)) {}

    private:

        explicit Denominator(std::uint32_t d, std::uint32_t l):
            m((std::uint64_t((1 << l) - d) << 32) / std::uint64_t(d) + 1),
            sh1(min(l, std::uint32_t(1))),
            sh2(l - sh1),
            d(d) {}

    public:

        //=================================================
        // Arithmetic Operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend avel::div_type<std::uint32_t> div(std::uint32_t n, Denominator denom) {
            std::uint32_t t1 = std::uint64_t(denom.m) * std::uint64_t(n) >> 32;
            std::uint32_t q = (t1 + ((n - t1) >> denom.sh1)) >> denom.sh2;
            std::uint32_t r = n - (q * denom.d);
            return {q, r};
        }

        [[nodiscard]]
        AVEL_FINL friend std::uint32_t operator/(std::uint32_t lhs, Denominator rhs) {
            return div(lhs, rhs).quot;
        }

        [[nodiscard]]
        AVEL_FINL friend std::uint32_t operator%(std::uint32_t lhs, Denominator rhs) {
            return div(lhs, rhs).rem;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        std::uint32_t m = 0;
        bool sh1 = false;
        std::uint32_t sh2 = 0;
        std::uint32_t d = 0;

    };

    AVEL_FINL std::uint32_t& operator/=(std::uint32_t& lhs, Denom32u rhs) {
        lhs = lhs / rhs;
        return lhs;
    }

    AVEL_FINL std::uint32_t& operator%=(std::uint32_t& lhs, Denom32u rhs) {
        lhs = lhs % rhs;
        return lhs;
    }

}

#endif //AVEL_DENOMINATOR32U_HPP