#ifndef AVEL_FIXED_POINT16U_HPP
#define AVEL_FIXED_POINT16U_HPP

namespace avel {



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

#endif //AVEL_FIXED_POINT16U_HPP
