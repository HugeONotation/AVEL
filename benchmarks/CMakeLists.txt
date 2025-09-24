cmake_minimum_required(VERSION 3.16)


add_executable(AVEL_benchmarks
    AVEL_benchmarks.cpp
    Bitwise_not_8u.hpp
    Bitwise_not_16u.hpp
    Bitwise_not_32u.hpp
    Bitwise_not_64u.hpp
    Extract_8u.hpp
    Extract_16u.hpp
    Extract_32u.hpp
    Extract_64u.hpp
    Insert_8u.hpp
    Insert_16u.hpp
    Insert_32u.hpp
    Insert_64u.hpp
    Bitwise_rotl_vector_8u.hpp
    Bitwise_rotl_vector_16u.hpp
    Bitwise_rotl_vector_32u.hpp
    Bitwise_rotl_vector_64u.hpp
    Bitwise_rotr_vector_8u.hpp
    Bitwise_rotr_vector_16u.hpp
    Bitwise_rotr_vector_32u.hpp
    Bitwise_rotr_vector_64u.hpp
    Midpoint_8u.hpp
    Midpoint_16u.hpp
    Midpoint_32u.hpp
    Midpoint_64u.hpp
    Midpoint_8i.hpp
    Midpoint_16i.hpp
    Midpoint_32i.hpp
    Midpoint_64i.hpp
    Has_single_bit_8u.hpp
    Has_single_bit_16u.hpp
    Has_single_bit_32u.hpp
    Has_single_bit_64u.hpp
    Bit_width_8u.hpp
    Bit_width_16u.hpp
    Bit_width_32u.hpp
    Bit_width_64u.hpp
    Bit_floor_8u.hpp
    Bit_floor_16u.hpp
    Bit_floor_32u.hpp
    Bit_floor_64u.hpp
    Countl_sign_8u.hpp
    Countl_sign_16u.hpp
    Countl_sign_32u.hpp
    Countl_sign_64u.hpp
    Countr_one_8u.hpp
    Countr_one_16u.hpp
    Countr_one_32u.hpp
    Countr_one_64u.hpp
    Countr_zero_8u.hpp
    Countr_zero_16u.hpp
    Countr_zero_32u.hpp
    Countr_zero_64u.hpp
    Countl_one_8u.hpp
    Countl_one_16u.hpp
    Countl_one_32u.hpp
    Countl_one_64u.hpp
    Average_8u.hpp
    Average_16u.hpp
    Average_32u.hpp
    Average_64u.hpp
    Average_8i.hpp
    Average_16i.hpp
    Average_32i.hpp
    Average_64i.hpp
    Mulhi_8i.hpp
    Mulhi_16i.hpp
    Mulhi_32i.hpp
    Mulhi_64i.hpp
    Zero_extend_8u_16u.hpp
    Zero_extend_8u_32u.hpp
    Zero_extend_8u_64u.hpp
    Zero_extend_16u_32u.hpp
    Zero_extend_32u_64u.hpp
    Reduce_add_8u.hpp
    Reduce_add_16u.hpp
    Reduce_add_32u.hpp
    Reduce_add_64u.hpp
)

target_link_libraries(AVEL_benchmarks PRIVATE AVEL benchmark benchmark_main)
target_include_directories(AVEL_benchmarks PRIVATE ./)
target_compile_features(AVEL_benchmarks PRIVATE cxx_std_20)
target_compile_options(AVEL_benchmarks PRIVATE)
target_link_options(AVEL_benchmarks PRIVATE "")


include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)


if(supported)
    set_property(TARGET AVEL_benchmarks PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
    message(STATUS "IPO / LTO not supported: <${error}>")
endif()
