#ifndef AVEL_COUNTL_ZERO_16U_HPP
#define AVEL_COUNTL_ZERO_16U_HPP

namespace avel::benchmarks::countl_zero_16u {

    //=====================================================
    // scalar 16u benchmarks
    //=====================================================

    #if __cplusplus >= 202002L

    std::uint16_t scalar_native_impl(std::uint16_t x) {
        return std::countl_zero(x);
    }

    auto scalar_native = scalar_test_bench<std::uint16_t, scalar_native_impl>;

    BENCHMARK(countl_zero_16u::scalar_native);

    #endif



    std::uint16_t scalar_bitwise_accumulate_impl(std::uint16_t x) {
        std::uint16_t ret = 0;

        std::int8_t tmp = x;
        for (int i = 0; i < 16; ++i) {
            ret += (tmp < 0);
            tmp <<= 1;
        }

        return ret;
    }

    auto scalar_bitwise_accumulate = scalar_test_bench<std::uint16_t, scalar_bitwise_accumulate_impl>;

    BENCHMARK(countl_zero_16u::scalar_bitwise_accumulate);

    //=====================================================
    // vec8x16u
    //=====================================================

    #if defined(AVEL_SSE2)

    #endif

    //=====================================================
    // vec16x16u
    //=====================================================

    #if defined(AVEL_AVX2)

    #endif

    //=====================================================
    // vec32x16u
    //=====================================================

    #if defined(AVEL_AVX512BW)

    vec32x16u vec32x16u_vpermi2b_impl(vec32x16u v) {
        //TODO: Implement
        return {};
    }

    auto vec32x16u_vpermi2b = vector_test_bench<vec32x16u, vec32x16u_vpermi2b_impl>;

    //BENCHMARK(countl_zero_16u::vec32x16u_vpermi2b);

    #endif

}

#endif //AVEL_COUNTL_ZERO_16U_HPP
