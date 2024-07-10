#ifndef AVEL_FIXED_POINT8U_HPP
#define AVEL_FIXED_POINT8U_HPP

namespace avel {

    template<std::size_t Fractional_bits, bool Saturate>
    class Fixed_point<std::uint8_t, Fractional_bits> {
    public:

        //=================================================
        // -ctors
        //=================================================

        //=================================================
        // Assignment operators
        //=================================================

        //=================================================
        // Arithmetic operators
        //=================================================

    private:

        //=================================================
        // Instance members
        //=================================================

    };

    //=========================================================================
    // Type aliases
    //=========================================================================

    using Q8_0 = avel::Fixed_point<std::uint8_t, 0>;
    using Q1_7 = avel::Fixed_point<std::uint8_t, 1>;
    using Q2_6 = avel::Fixed_point<std::uint8_t, 2>;
    using Q3_5 = avel::Fixed_point<std::uint8_t, 3>;
    using Q4_4 = avel::Fixed_point<std::uint8_t, 4>;
    using Q5_3 = avel::Fixed_point<std::uint8_t, 5>;
    using Q6_2 = avel::Fixed_point<std::uint8_t, 6>;
    using Q7_1 = avel::Fixed_point<std::uint8_t, 7>;

    //=========================================================================
    // Literals
    //=========================================================================

    Fixed_point<std::uint8_t, 1> operator ""_8q0(long double);
    Fixed_point<std::uint8_t, 2> operator ""_7q1(long double);
    Fixed_point<std::uint8_t, 3> operator ""_6q2(long double);
    Fixed_point<std::uint8_t, 4> operator ""_5q3(long double);
    Fixed_point<std::uint8_t, 5> operator ""_4q4(long double);
    Fixed_point<std::uint8_t, 6> operator ""_3q5(long double);
    Fixed_point<std::uint8_t, 7> operator ""_2q6(long double);
    Fixed_point<std::uint8_t, 7> operator ""_1q7(long double);
    Fixed_point<std::uint8_t, 8> operator ""_0q8(long double);

}

#endif //AVEL_FIXED_POINT8U_HPP
