#ifndef ZERO_EXTEND_16U_32U_HPP
#define ZERO_EXTEND_16U_32U_HPP

namespace avel::benchmarks::zero_extend_16u_32u {

    //=====================================================
    // scalar 16u to 64u benchmarks
    //=====================================================

    std::uint32_t scalar_native_impl(std::uint16_t x) {
        return x;
    }

    auto scalar_native = scalar_conversion_test_bench<std::uint16_t, std::uint32_t, scalar_native_impl>;

    BENCHMARK(zero_extend_16u_32u::scalar_native);

}

#endif //ZERO_EXTEND_16U_32U_HPP
