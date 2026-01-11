#ifndef AVEL_VECTORS_COMMON_HPP
#define AVEL_VECTORS_COMMON_HPP

#define AVEL_VECTOR_MASK_BINARY_BITWISE_OPERATORS \
\
        [[nodiscard]]\
        AVEL_FINL friend Vector_mask operator&(Vector_mask lhs, Vector_mask rhs) {\
            lhs &= rhs;\
            return lhs;\
        }\
\
        [[nodiscard]]\
        AVEL_FINL friend Vector_mask operator&&(Vector_mask lhs, Vector_mask rhs) {\
            return lhs & rhs;\
        }\
\
        [[nodiscard]]\
        AVEL_FINL friend Vector_mask operator|(Vector_mask lhs, Vector_mask rhs) {\
            lhs |= rhs;\
            return lhs;\
        }\
\
        [[nodiscard]]\
        AVEL_FINL friend Vector_mask operator||(Vector_mask lhs, Vector_mask rhs) {\
            return lhs | rhs;\
        }\
\
        [[nodiscard]]\
        AVEL_FINL friend Vector_mask operator^(Vector_mask lhs, Vector_mask rhs) {\
            lhs ^= rhs;\
            return lhs;\
        }\



#define AVEL_VECTOR_ARITHMETIC_OPERATORS \
        [[nodiscard]]\
        AVEL_FINL friend Vector operator+(Vector lhs, Vector rhs) {\
            lhs += rhs;\
            return lhs;\
        }\
\
        [[nodiscard]]\
        AVEL_FINL friend Vector operator-(Vector lhs, Vector rhs) {\
            lhs -= rhs;\
            return lhs;\
        }\
\
        [[nodiscard]]\
        AVEL_FINL friend Vector operator*(Vector lhs, Vector rhs) {\
            lhs *= rhs;\
            return lhs;\
        }\
\
        [[nodiscard]]\
        AVEL_FINL friend Vector operator/(Vector lhs, Vector rhs) {\
            lhs /= rhs;\
            return lhs;\
        }\
\
        [[nodiscard]]\
        AVEL_FINL friend Vector operator%(Vector lhs, Vector rhs) {\
            lhs %= rhs;\
            return lhs;\
        }\



#define AVEL_VECTOR_INCREMENT_DECREMENT_OPERATORS \
        AVEL_FINL Vector& operator++() {\
            *this += Vector{1};\
            return *this;\
        }\
\
        AVEL_FINL Vector operator++(int) {\
            auto temp = *this;\
            *this += Vector{1};\
            return temp;\
        }\
\
        AVEL_FINL Vector& operator--() {\
            *this -= Vector{1};\
            return *this;\
        }\
\
        AVEL_FINL Vector operator--(int) {\
            auto temp = *this;\
            *this -= Vector{1};\
            return temp;\
        }\



#define AVEL_VECTOR_BINARY_BITWISE_OPERATORS \
        [[nodiscard]]\
        AVEL_FINL friend Vector operator&(Vector lhs, Vector rhs) {\
            lhs &= rhs;\
            return lhs;\
        }\
\
        [[nodiscard]]\
        AVEL_FINL friend Vector operator|(Vector lhs, Vector rhs) {\
            lhs |= rhs;\
            return lhs;\
        }\
\
        [[nodiscard]]\
        AVEL_FINL friend Vector operator^(Vector lhs, Vector rhs) {\
            lhs ^= rhs;\
            return lhs;\
        }\
\
        [[nodiscard]]\
        AVEL_FINL friend Vector operator<<(Vector lhs, long long rhs) {\
            lhs <<= rhs;\
            return lhs;\
        }\
\
        [[nodiscard]]\
        AVEL_FINL friend Vector operator>>(Vector lhs, long long rhs) {\
            lhs >>= rhs;\
            return lhs;\
        }\
\
        [[nodiscard]]\
        AVEL_FINL friend Vector operator<<(Vector lhs, Vector rhs) {\
            lhs <<= rhs;\
            return lhs;\
        }\
\
        [[nodiscard]]\
        AVEL_FINL friend Vector operator>>(Vector lhs, Vector rhs) {\
            lhs >>= rhs;\
            return lhs;\
        }\



#define AVEL_SIGNED_VECTOR_BIT_FUNCTIONS(VEC_ALIAS, MASK_ALIAS, UVEC_ALIAS) \
    [[nodiscard]]\
    AVEL_FINL VEC_ALIAS popcount(VEC_ALIAS x) {\
        return VEC_ALIAS{popcount(UVEC_ALIAS(x))};\
    }\
\
    [[nodiscard]]\
    AVEL_FINL VEC_ALIAS countl_zero(VEC_ALIAS x) {\
        return VEC_ALIAS{countl_zero(UVEC_ALIAS{x})};\
    }\
\
    [[nodiscard]]\
    AVEL_FINL VEC_ALIAS countl_one(VEC_ALIAS x) {\
        return VEC_ALIAS{countl_one(UVEC_ALIAS{x})};\
    }\
\
    [[nodiscard]]\
    AVEL_FINL VEC_ALIAS countr_zero(VEC_ALIAS x) {\
        return VEC_ALIAS{countr_zero(UVEC_ALIAS{x})};\
    }\
\
    [[nodiscard]]\
    AVEL_FINL VEC_ALIAS countr_one(VEC_ALIAS x) {\
        return VEC_ALIAS{countr_one(UVEC_ALIAS{x})};\
    }\
\
    [[nodiscard]]\
    AVEL_FINL MASK_ALIAS has_single_bit(VEC_ALIAS x) {\
        return MASK_ALIAS{has_single_bit(UVEC_ALIAS(x))};\
    }\
\
    [[nodiscard]]\
    AVEL_FINL MASK_ALIAS no_bits(VEC_ALIAS v) {\
        return MASK_ALIAS{no_bits(UVEC_ALIAS(v))};\
    }\
\
    [[nodiscard]]\
    AVEL_FINL MASK_ALIAS no_bits_of(VEC_ALIAS v, VEC_ALIAS m) {\
        return MASK_ALIAS{no_bits_of(UVEC_ALIAS(v), UVEC_ALIAS(m))};\
    }\
\
    [[nodiscard]]\
    AVEL_FINL MASK_ALIAS any_bits(VEC_ALIAS v) {\
        return MASK_ALIAS{any_bits(UVEC_ALIAS(v))};\
    }\
\
    [[nodiscard]]\
    AVEL_FINL MASK_ALIAS any_bits_of(VEC_ALIAS v, VEC_ALIAS m) {\
        return MASK_ALIAS{any_bits_of(UVEC_ALIAS(v), UVEC_ALIAS(m))};\
    }\
\
    [[nodiscard]]\
    AVEL_FINL MASK_ALIAS all_bits(VEC_ALIAS v) {\
        return MASK_ALIAS{all_bits(UVEC_ALIAS(v))};\
    }\
\
    [[nodiscard]]\
    AVEL_FINL MASK_ALIAS all_bits_of(VEC_ALIAS v, VEC_ALIAS m) {\
        return MASK_ALIAS{all_bits_of(UVEC_ALIAS(v), UVEC_ALIAS(m))};\
    }\

#endif //AVEL_VECTORS_COMMON_HPP
