//=========================================================
// x86 feature macros
//=========================================================

//#define AVEL_FORCE_INLINE
//#define AVEL_AUTO_DETECT
//#define AVEL_GFNI
//#define AVEL_AVX512VBMI2
//#define AVEL_AVX512VBMI
//#define AVEL_AVX512VPOPCNTDQ
//#define AVEL_AVX512BITALG
//#define AVEL_AVX512BW
//#define AVEL_AVX512VL
//#define AVEL_AVX512DQ
//#define AVEL_AVX512CD
//#define AVEL_AVX512F
//#define AVEL_FMA
//#define AVEL_AVX2
//#define AVEL_AVX
//#define AVEL_SSE4_2
//#define AVEL_SSE4_1
//#define AVEL_SSSE3
//#define AVEL_SSE3
//#define AVEL_SSE2
//#define AVEL_BMI2
//#define AVEL_BMI
//#define AVEL_LZCNT
//#define AVEL_POPCNT
//#define AVEL_X86

//=========================================================
// Arm feature macros
//=========================================================

//#define AVEL_AARCH64
//#define AVEL_NEON

//=========================================================
// Misc. Includes
//=========================================================

#include <avel/Avel.hpp>

#include <cstdint>
#include <bit>

#include "AVEL_benchmarks.hpp"

//=========================================================
// Benchmark includes
//=========================================================

// #include "Bitwise_not_8u.hpp"
// #include "Bitwise_not_16u.hpp"
// #include "Bitwise_not_32u.hpp"
// #include "Bitwise_not_64u.hpp"

// #include "Bitwise_and_8u.hpp"
// #include "Bitwise_and_16u.hpp"
// #include "Bitwise_and_32u.hpp"
// #include "Bitwise_and_64u.hpp"

// #include "Bitwise_or_8u.hpp"
// #include "Bitwise_or_16u.hpp"
// #include "Bitwise_or_32u.hpp"
// #include "Bitwise_or_64u.hpp"

// #include "Bitwise_xor_8u.hpp"
// #include "Bitwise_xor_16u.hpp"
// #include "Bitwise_xor_32u.hpp"
// #include "Bitwise_xor_64u.hpp"

// #include "Bit_ceil_8u.hpp"
// #include "Bit_ceil_16u.hpp"
// #include "Bit_ceil_32u.hpp"
// #include "Bit_ceil_64u.hpp"

// #include "Bit_floor_8u.hpp"
// #include "Bit_floor_16u.hpp"
// #include "Bit_floor_32u.hpp"
// #include "Bit_floor_64u.hpp"

// #include "Bit_width_8u.hpp"
// #include "Bit_width_16u.hpp"
// #include "Bit_width_32u.hpp"
// #include "Bit_width_64u.hpp"

// #include "Popcount_8u.hpp"
// #include "Popcount_16u.hpp"
// #include "Popcount_32u.hpp"
// #include "Popcount_64u.hpp"

//#include "Byteswap_16u.hpp"
//#include "Byteswap_32u.hpp"
//#include "Byteswap_64u.hpp"

//#include "Add_8u.hpp"
//#include "Add_16u.hpp"
//#include "Add_32u.hpp"
//#include "Add_64u.hpp"

//#include "Sub_8u.hpp"
//#include "Sub_16u.hpp"
//#include "Sub_32u.hpp"
//#include "Sub_64u.hpp"

//#include "Mul_8u.hpp"
//#include "Mul_16u.hpp"
//#include "Mul_32u.hpp"
//#include "Mul_64u.hpp"

//#include "Mulhi_8u.hpp"
//#include "Mulhi_16u.hpp"
//#include "Mulhi_32u.hpp"
//#include "Mulhi_64u.hpp"

//#include "Mulhi_8i.hpp"
//#include "Mulhi_16i.hpp"
//#include "Mulhi_32i.hpp"
//#include "Mulhi_64i.hpp"

//#include "Div_8u.hpp"
//#include "Div_16u.hpp"
//#include "Div_32u.hpp"
//#include "Div_64u.hpp"

//#include "Isqrt_uint8.hpp"
//#include "Isqrt_uint16.hpp"
//#include "Isqrt_uint32.hpp"
//#include "Isqrt_uint64.hpp"

//#include "fpclassify_32f.hpp"
//#include "fpclassify_64f.hpp"

//#include "Mask_from_bool_8i.hpp"
//#include "Mask_from_bool_16i.hpp"
//#include "Mask_from_bool_32i.hpp"
//#include "Mask_from_bool_64i.hpp"

//TODO: Consider using conversions to 32-bit and 64-bit integers

//#include "scalbn_32f.hpp"

//#include "Compress_8u.hpp"
//#include "Compress_16u.hpp"
//#include "Compress_32u.hpp"
//#include "Compress_64u.hpp"

//#include "Exclusive_prefix_sum_8u.hpp"
//#include "Exclusive_prefix_sum_16u.hpp"
//#include "Exclusive_prefix_sum_32u.hpp"
//#include "Exclusive_prefix_sum_64u.hpp"

//#include "Memory_access.hpp"

// TODO: Use RegisterBenchmark() to programmatically register benchmarks

#include "AVEL_benchmarks_include.hpp"

BENCHMARK_MAIN();
