#ifndef AVEL_VECTOR_PRIMITIVES_SSE2_HPP
#define AVEL_VECTOR_PRIMITIVES_SSE2_HPP

//=========================================================
// Vector primitives
//=========================================================

template<>
struct vector_primitive<std::uint64_t, 2> {
    using type = __m128i;
};

template<>
struct vector_primitive<std::int64_t, 2> {
    using type = __m128i;
};


template<>
struct vector_primitive<std::uint32_t, 4> {
    using type = __m128i;
};

template<>
struct vector_primitive<std::int32_t, 4> {
    using type = __m128i;
};


template<>
struct vector_primitive<std::uint16_t, 8> {
    using type = __m128i;
};

template<>
struct vector_primitive<std::int16_t, 8> {
    using type = __m128i;
};


template<>
struct vector_primitive<std::uint8_t, 16> {
    using type = __m128i;
};

template<>
struct vector_primitive<std::int8_t, 16> {
    using type = __m128i;
};


template<>
struct vector_primitive<float, 4> {
    using type = __m128;
};

template<>
struct vector_primitive<double, 2> {
    using type = __m128d;
};

//=========================================================
// Mask primitives
//=========================================================

template<>
struct mask_primitive<std::uint8_t, 16> {
    using type = __m128i;
};

template<>
struct mask_primitive<std::int8_t, 16> {
    using type = __m128i;
};


template<>
struct mask_primitive<std::uint16_t, 8> {
    using type = __m128i;
};

template<>
struct mask_primitive<std::int16_t, 8> {
    using type = __m128i;
};


template<>
struct mask_primitive<std::uint32_t, 4> {
    using type = __m128i;
};

template<>
struct mask_primitive<std::int32_t, 4> {
    using type = __m128i;
};


template<>
struct mask_primitive<std::uint64_t, 2> {
    using type = __m128i;
};

template<>
struct mask_primitive<std::int64_t, 2> {
    using type = __m128i;
};


template<>
struct mask_primitive<float, 4> {
    using type = __m128;
};

template<>
struct mask_primitive<double, 8> {
    using type = __m128d;
};

#endif //AVEL_VECTOR_PRIMITIVES_SSE2_HPP
