#ifndef AVEL_COUNTL_SIGN_8U_HPP
#define AVEL_COUNTL_SIGN_8U_HPP

namespace avel::benchmarks::countl_sign_8i {

    //=====================================================
    // scalar 8i benchmarks
    //=====================================================

    #if __cplusplus >= 202002L

    std::int8_t scalar_countl_zero_impl(std::int8_t x) {
        return std::countl_zero(std::uint8_t(x ^ (x >> 1))) - 1;
    }

    auto scalar_countl_zero = scalar_test_bench<std::int8_t, scalar_countl_zero_impl>;

    BENCHMARK(countl_sign_8i::scalar_countl_zero);

    #endif



    std::int8_t scalar_bitwise_accumulate_impl(std::int8_t x) {
        //TODO: Check correctness
        std::uint8_t ret = 0;

        bool is_negative = x < 0;

        std::int8_t tmp = is_negative ? ~x : x;
        for (int i = 0; i < 8; ++i) {
            ret += (tmp < 0);
            tmp <<= 1;
        }

        ret -= is_negative;

        return ret;
    }

    auto scalar_bitwise_accumulate = scalar_test_bench<std::int8_t, scalar_bitwise_accumulate_impl>;

    BENCHMARK(countl_sign_8i::scalar_bitwise_accumulate);

}

#endif //AVEL_COUNTL_SIGN_8U_HPP
