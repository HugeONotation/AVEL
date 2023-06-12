#ifndef AVEL_FIXED_POINT_HPP
#define AVEL_FIXED_POINT_HPP

#include "Scalar.hpp"

#include "Misc.hpp"
#include "./impl/Constants.hpp"

#include <cstdint>
#include <cstdlib>

namespace avel {

    template<class Integer, std::uint32_t Fractional_bits>
    class Fixed_point;

    template<std::uint32_t Fractional_bits>
    class Fixed_point<std::uint8_t, Fractional_bits>;

    template<std::uint32_t Fractional_bits>
    class Fixed_point<std::uint16_t, Fractional_bits> {
    public:

        //=================================================
        // Static asserts
        //=================================================

        static_assert(
            Fractional_bits <= 16,
            "Number of fractional bits must not exceed bits in backing type"
        );

        //=================================================
        // Static constants
        //=================================================

        static constexpr std::uint32_t fractional_bits = Fractional_bits;
        static constexpr std::uint32_t whole_bits = 16 - Fractional_bits;

        //=================================================
        // Type aliases
        //=================================================

        using backing_type = std::uint16_t;

    private:

        //=================================================
        // Instance Members
        //=================================================

        std::uint16_t content;

    public:

        //=================================================
        // -ctors
        //=================================================

        template<class U, std::uint32_t F>
        Fixed_point(Fixed_point<U, F> x);

        Fixed_point(float x);

        Fixed_point(double x);

        Fixed_point() = default;
        Fixed_point(const Fixed_point&) = default;
        Fixed_point(Fixed_point&&) = default;
        ~Fixed_point() = default;

        //=================================================
        // Assignment operators
        //=================================================

        Fixed_point& operator=(const Fixed_point&) = default;
        Fixed_point& operator=(Fixed_point&&) = default;

        //=================================================
        // Comparison Operators
        //=================================================

        [[nodiscard]]
        friend bool operator==(Fixed_point lhs, Fixed_point rhs) {
            return lhs.content == rhs.content;
        }

        [[nodiscard]]
        friend bool operator!=(Fixed_point lhs, Fixed_point rhs) {
            return lhs.content != rhs.content;
        }

        [[nodiscard]]
        friend bool operator<(Fixed_point lhs, Fixed_point rhs) {
            return lhs.content < rhs.content;
        }

        [[nodiscard]]
        friend bool operator<=(Fixed_point lhs, Fixed_point rhs) {
            return lhs.content <= rhs.content;
        }

        [[nodiscard]]
        friend bool operator>(Fixed_point lhs, Fixed_point rhs) {
            return lhs.content > rhs.content;
        }

        [[nodiscard]]
        friend bool operator>=(Fixed_point lhs, Fixed_point rhs) {
            return lhs.content >= rhs.content;
        }

        //=================================================
        // Arithmetic Assignment
        //=================================================

        Fixed_point& operator+=(Fixed_point rhs) {
            auto z = std::uint32_t{content} + std::uint32_t{rhs.content};
            content = (z >= 0xFFFF) ? 0xFFFF : z;

            return *this;
        }

        Fixed_point& operator-=(Fixed_point rhs) {
            auto z = std::uint32_t{content} - std::uint32_t{rhs.content};
            content = (z > 0xFFFF) ? 0x0000 : z;

            return *this;
        }

        Fixed_point& operator*=(Fixed_point rhs) {
            auto product = std::uint32_t{content} * std::uint32_t{rhs.content};
            product += (1 << fractional_bits);
            product >>= fractional_bits;
            if (product >= 0xFFFF) {
                product = 0xFFFF;
            }
            content = product;

            return *this;
        }

        Fixed_point& operator/=(Fixed_point rhs) {
            //TODO: Handle overflow
            auto numerator = std::uint32_t(content) << fractional_bits;
            auto denominator = rhs.content;

            std::uint16_t quotient = numerator / denominator;
            content = quotient;

            return *this;
        }

        //=================================================
        // Arithmetic Operators
        //=================================================

        friend Fixed_point operator+(Fixed_point lhs, Fixed_point rhs) {
            lhs += rhs;
            return lhs;
        }

        friend Fixed_point operator-(Fixed_point lhs, Fixed_point rhs) {
            lhs += rhs;
            return lhs;
        }

        friend Fixed_point operator*(Fixed_point lhs, Fixed_point rhs) {
            lhs *= rhs;
            return lhs;
        }

        friend Fixed_point operator/(Fixed_point lhs, Fixed_point rhs) {
            lhs /= rhs;
            return lhs;
        }

        //=================================================
        // Increment/Decrement Operators
        //=================================================

        Fixed_point& operator++() {
            if (content == 0xFFFF) {
                content += 1;
            }
            return *this;
        }

        Fixed_point operator++(int) {
            auto ret = *this;
            if (content == 0xFFFF) {
                content += 1;
            }
            return ret;
        }

        Fixed_point& operator--() {
            if (content == 0x0000) {
                content -= 1;
            }
            return *this;
        }

        Fixed_point& operator--(int) {
            auto ret = *this;
            if (content == 0x0000) {
                content -= 1;
            }
            return ret;
        }

        //=================================================
        // Unary Arithmetic Operators
        //=================================================

        Fixed_point operator+() const {
            return *this;
        }

        /*
        Fixed_point operator-() const {
            //TODO: Implement once signed fixed-point numbers are implemented
        }
        */

        //=================================================
        // Conversion Operators
        //=================================================

        [[nodiscard]]
        explicit operator float() const {
            float whole = content;

            constexpr float divisor = (1.0f / (1 << fractional_bits));
            return whole * divisor;
        }

        [[nodiscard]]
        explicit operator double() const {
            double whole = content;

            constexpr double divisor = (1.0 / (1 << fractional_bits));
            return whole * divisor;
        }

        [[nodiscard]]
        friend backing_type decay(Fixed_point x) {
            return x.content;
        }

    };

    static_assert(
        2 == sizeof(Fixed_point<std::uint16_t, 0>),
        "Fixed-point scalar was not of the expected size!"
    );

    /*
    template<std::uint32_t Fractional_bits>
    class Fixed_point<std::uint32_t, Fractional_bits> {
    public:

        //=================================================
        // Static asserts
        //=================================================

        static_assert(
            Fractional_bits < 32,
            "Number of fractional bits must be less than number of bits in backing type"
        );

        //=================================================
        // Static constants
        //=================================================

        static constexpr std::uint32_t fractional_bits = Fractional_bits;
        static constexpr std::uint32_t whole_bits = 32 - Fractional_bits;

        //=================================================
        // Type aliases
        //=================================================

        using primitive = std::uint32_t;

    private:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

    public:

        //=================================================
        // -ctors
        //=================================================

        template<class N>
        explicit Fixed_point(N n);

        explicit Fixed_point(float x):
            content([x] () {
                std::uint32_t bits = bit_cast<std::uint32_t>(x);
                std::uint32_t mantissa = bits & 0x007FFFFF;
                std::int32_t exponent = ((bits >> 23) & 0x000000FF) - 127;
                std::uint32_t sign_bit = bits >> 31;

                if (sign_bit) {
                    return 0x00;
                }

                if (exponent >= whole_bits) {
                    return 0xFFFFFFFF;
                }

                if (exponent >= ) {
                    return mantissa << ;
                } else {
                    return mantissa >> ;
                }

                //TODO: Complete implementation
            } ()) {}

        Fixed_point() = default;
        Fixed_point(const Fixed_point&) = default;
        Fixed_point(Fixed_point&&) = default;
        ~Fixed_point() = default;

        //=================================================
        // Assignment operators
        //=================================================

        Fixed_point& operator=(const Fixed_point&) = default;
        Fixed_point& operator=(Fixed_point&&) = default;

        //=================================================
        // Arithmetic assignment operators
        //=================================================

        Fixed_point& operator+=(Fixed_point rhs) {
            content += rhs.content;
            return *this;
        }

        Fixed_point& operator-=(Fixed_point rhs) {
            content -= rhs.content;
            return *this;
        }

        Fixed_point& operator*=(Fixed_point rhs) {
            std::uint64_t x = content;
            std::uint64_t y = rhs.content;

            content = (x * y) >> Fractional_bits;

            return *this;
        }

        Fixed_point& operator/=(Fixed_point rhs) {
            content = div(*this, rhs).quot.content;
            return *this;
        }

        Fixed_point& operator%=(Fixed_point rhs) {
            content = div(*this, rhs).rem.content;
            return *this;
        }

        //=================================================
        // Arithmetic operators
        //=================================================

        [[nodiscard]]
        friend Fixed_point operator+(Fixed_point lhs, Fixed_point rhs) {
            lhs += rhs;
            return lhs;
        }

        [[nodiscard]]
        friend Fixed_point operator-(Fixed_point lhs, Fixed_point rhs) {
            lhs -= rhs;
            return lhs;
        }

        [[nodiscard]]
        friend Fixed_point operator*(Fixed_point lhs, Fixed_point rhs) {
            lhs *= rhs;
            return lhs;
        }

        [[nodiscard]]
        friend Fixed_point operator/(Fixed_point lhs, Fixed_point rhs) {
            lhs /= rhs;
            return lhs;
        }

        [[nodiscard]]
        friend Fixed_point operator%(Fixed_point lhs, Fixed_point rhs) {
            lhs %= rhs;
            return lhs;
        }

        //=================================================
        // Increment/Decrement operators
        //=================================================

        Fixed_point& operator++() {
            content += 1;
            return *this;
        }

        Fixed_point operator++(int) {
            auto tmp = *this;
            content += 1;
            return tmp;
        }

        Fixed_point& operator--() {
            content -= 1;
            return *this;
        }

        Fixed_point operator--(int) {
            auto tmp = *this;
            content -= 1;
            return tmp;
        }

        //=================================================
        // Conversion operators
        //=================================================

        [[nodiscard]]
        explicit operator float() const {
            std::uint32_t exponent = 127 + 32 - countl_zero(content) - fractional_bits;

            std::uint32_t mantissa;
            //() & float_mantissa_mask_bits;
            //TODO: Complete implementation

            std::uint32_t ret = (exponent << 23) | mantissa;
            return bit_cast<float>(ret);
        }

        [[nodiscard]]
        explicit operator double() const {
            std::uint64_t exponent = 1023 + 32 - countl_zero(content) - fractional_bits;


            //TODO: Implement;
        }

        template<class U, std::uint32_t F2>
        [[nodiscard]]
        explicit operator Fixed_point<U, F2>() {
            //TODO: Implement;
        }

        //=================================================
        // Free functions
        //=================================================

        [[nodiscard]]
        div_type<Fixed_point> div(Fixed_point numerator, Fixed_point denominator) {
            //TODO: Fix this broken implementation
            std::uint32_t n = (numerator.content << fractional_bits) / denominator.content;
            std::uint32_t d = (numerator.content << fractional_bits) % denominator.content;

            return {
                n,
                d
            };
        }

    };
    */

    Fixed_point<std::uint8_t, 1> operator ""_8q0(long double);
    Fixed_point<std::uint8_t, 2> operator ""_7q1(long double);
    Fixed_point<std::uint8_t, 3> operator ""_6q2(long double);
    Fixed_point<std::uint8_t, 4> operator ""_5q3(long double);
    Fixed_point<std::uint8_t, 5> operator ""_4q4(long double);
    Fixed_point<std::uint8_t, 6> operator ""_3q5(long double);
    Fixed_point<std::uint8_t, 7> operator ""_2q6(long double);
    Fixed_point<std::uint8_t, 7> operator ""_1q7(long double);
    Fixed_point<std::uint8_t, 8> operator ""_0q8(long double);

    Fixed_point<std::uint16_t, 1> operator ""_16q0(long double);
    Fixed_point<std::uint16_t, 1> operator ""_15q1(long double);
    Fixed_point<std::uint16_t, 1> operator ""_14q2(long double);
    Fixed_point<std::uint16_t, 1> operator ""_13q3(long double);
    Fixed_point<std::uint16_t, 1> operator ""_12q4(long double);
    Fixed_point<std::uint16_t, 1> operator ""_11q5(long double);
    Fixed_point<std::uint16_t, 1> operator ""_10q6(long double);
    Fixed_point<std::uint16_t, 1> operator ""_9q7(long double);
    Fixed_point<std::uint16_t, 1> operator ""_8q8(long double);
    Fixed_point<std::uint16_t, 2> operator ""_7q9(long double);
    Fixed_point<std::uint16_t, 3> operator ""_6q10(long double);
    Fixed_point<std::uint16_t, 4> operator ""_5q11(long double);
    Fixed_point<std::uint16_t, 5> operator ""_4q12(long double);
    Fixed_point<std::uint16_t, 6> operator ""_3q13(long double);
    Fixed_point<std::uint16_t, 7> operator ""_2q14(long double);
    Fixed_point<std::uint16_t, 7> operator ""_1q15(long double);
    Fixed_point<std::uint16_t, 8> operator ""_0q16(long double);

}

#endif //AVEL_FIXED_POINT_HPP
