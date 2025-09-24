#ifndef AVEL_MULHI_UINT16_HPP
#define AVEL_MULHI_UINT16_HPP

namespace avel::benchmarks::mulhi_scalar16u {

    std::uint16_t native_implementation(std::uint16_t x, std::uint16_t y) {
        return (std::uint32_t(x) * std::uint32_t(y)) >> 16;
    }

    auto native = scalar_test_bench<std::uint16_t, native_implementation>;

    BENCHMARK(mulhi_scalar16u::native);

}

#if defined(AVEL_SSE2)

namespace avel::benchmarks::mulhi_vec8x16u {

    vec8x16u hardware_implementation(vec8x16u x, vec8x16u y) {
        return vec8x16u{_mm_mulhi_epu16(decay(x), decay(y))};
    }

    auto hardware = vector_test_bench<vec8x16u, hardware_implementation>;

    BENCHMARK(mulhi_vec8x16u::hardware);

}

#endif





#if defined(AVEL_AVX2)

namespace avel::benchmarks::mulhi_vec16x16u {

    vec16x16u hardware_implementation(vec16x16u x, vec16x16u y) {
        return vec16x16u{_mm256_mulhi_epu16(decay(x), decay(y))};
    }

    auto hardware = vector_test_bench<vec16x16u, hardware_implementation>;

    BENCHMARK(mulhi_vec16x16u::hardware);

}

#endif





#if defined(AVEL_AVX512BW)

namespace avel::benchmarks::mulhi_vec32x16u {

    vec32x16u hardware_implementation(vec32x16u x, vec32x16u y) {
        return vec32x16u{_mm512_mulhi_epu16(decay(x), decay(y))};
    }

    auto hardware = vector_test_bench<vec32x16u, hardware_implementation>;

    BENCHMARK(mulhi_vec32x16u::hardware);

}

#endif


#endif //AVEL_MULHI_UINT16_HPP
