#ifndef ZERO_EXTEND_8U_16U_HPP
#define ZERO_EXTEND_8U_16U_HPP

namespace avel::benchmarks::zero_extend_8u_16u {

    //=====================================================
    // scalar 8u to 16u benchmarks
    //=====================================================

    std::uint16_t scalar_native_impl(std::uint8_t x) {
        return x;
    }

    auto scalar_native = scalar_conversion_test_bench<std::uint8_t, std::uint16_t, scalar_native_impl>;

    BENCHMARK(zero_extend_8u_16u::scalar_native);

    //=====================================================
    // Vector vec16x8u to vec8x16u benchmarks
    //=====================================================


    //=====================================================
    // Vector vec32x8u to vec16x16u benchmarks
    //=====================================================



    //=====================================================
    // Vector vec64x8u to vec32x16u benchmarks
    //=====================================================



}

#endif //ZERO_EXTEND_8U_16U_HPP
