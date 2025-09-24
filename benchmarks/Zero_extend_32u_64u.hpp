#ifndef ZERO_EXTEND_32U_64U_HPP
#define ZERO_EXTEND_32U_64U_HPP

namespace avel::benchmarks::zero_extend_32u_64u {

    //=====================================================
    // scalar 32u to 64u benchmarks
    //=====================================================

    std::uint64_t scalar_native_impl(std::uint32_t x) {
        return x;
    }

    auto scalar_native = scalar_conversion_test_bench<std::uint32_t, std::uint64_t, scalar_native_impl>;

    BENCHMARK(zero_extend_32u_64u::scalar_native);

}

#endif //ZERO_EXTEND_32U_64U_HPP
