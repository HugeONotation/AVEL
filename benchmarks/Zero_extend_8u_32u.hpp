#ifndef ZERO_EXTEND_8U_32U_HPP
#define ZERO_EXTEND_8U_32U_HPP

namespace avel::benchmarks::zero_extend_8u_32u {

    //=====================================================
    // scalar 8u to 32u benchmarks
    //=====================================================

    std::uint32_t scalar_native_impl(std::uint8_t x) {
        return x;
    }

    auto scalar_native = scalar_conversion_test_bench<std::uint8_t, std::uint32_t, scalar_native_impl>;

    BENCHMARK(zero_extend_8u_32u::scalar_native);

}

#endif //ZERO_EXTEND_8U_32U_HPP
