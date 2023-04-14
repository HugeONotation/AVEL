#ifndef AVEL_VEC16X32I_HPP
#define AVEL_VEC16X32I_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec16x32i = Vector<std::int32_t, 16>;
    using arr16x32i = std::array<std::int32_t, 16>;
    using mask16x32i = Vector_mask<std::int32_t, 16>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec16x32i> div(vec16x32i numerator, vec16x32i denominator);
    vec16x32i broadcast_mask(mask16x32i m);
    vec16x32i blend(mask16x32i m, vec16x32i a, vec16x32i b);
    vec16x32i negate(vec16x32i m, vec16x32i x);





    template<>
    class Vector_mask<std::int32_t, 16> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 16;

        //=================================================
        // Type aliases
        //=================================================

        using primitive = __mmask16;

        //=================================================
        // Constructor
        //=================================================

        AVEL_FINL explicit Vector_mask(Vector_mask<std::uint32_t, 16> v):
            content(v) {}

        AVEL_FINL explicit Vector_mask(Vector_mask<float, 16> v);

        AVEL_FINL explicit Vector_mask(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector_mask(bool x):
            content(from_bool(x)) {}

        AVEL_FINL explicit Vector_mask(const std::array<bool, 16>& arr) {
            std::uint64_t lo_half;
            std::memcpy(&lo_half, arr.data() + 0, 8);

            std::uint64_t hi_half;
            std::memcpy(&lo_half, arr.data() + 8, 8);

            //Assumes that the bitwise representation of true has the low bit
            //set, an assumption that holds when targeting the Itanium ABI
            std::uint64_t bit_mask = 0x0101010101010101;

            unsigned t0 = _pext_u64(lo_half, bit_mask);
            unsigned t1 = _pext_u64(hi_half, bit_mask);

            content = _mm512_int2mask((t1 << 8) | t0);
        }

        Vector_mask() = default;
        Vector_mask(const Vector_mask&) = default;
        Vector_mask(Vector_mask&&) = default;
        ~Vector_mask() = default;

        //=================================================
        // Assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator=(bool x) {
            content = from_bool(x);
            return *this;
        }

        Vector_mask& operator=(const Vector_mask&) = default;
        Vector_mask& operator=(Vector_mask&&) = default;

        //=================================================
        // Comparison operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend bool operator==(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_AVX512F)
            auto tmp = _kxor_mask16(decay(lhs), decay(rhs));
            return _kortestz_mask16_u8(tmp, tmp);
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_AVX512F)
            auto tmp = _kxor_mask16(decay(lhs), decay(rhs));
            return !_kortestz_mask16_u8(tmp, tmp);
            #endif
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator&=(Vector_mask rhs) {
            content = _kand_mask16(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            content = _kor_mask16(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            content = _kxor_mask16(content, rhs.content);
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        AVEL_FINL Vector_mask operator~() const {
            return Vector_mask{_knot_mask16(content)};
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator&(Vector_mask lhs, Vector_mask rhs) {
            lhs &= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator&&(Vector_mask lhs, Vector_mask rhs) {
            return lhs & rhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator|(Vector_mask lhs, Vector_mask rhs) {
            lhs |= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator||(Vector_mask lhs, Vector_mask rhs) {
            return lhs | rhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator^(Vector_mask lhs, Vector_mask rhs) {
            lhs ^= rhs;
            return lhs;
        }

        //=================================================
        // Conversion operators
        //=================================================

        AVEL_FINL operator primitive() const {
            return content;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

        //=================================================
        // Helper functions
        //=================================================

        AVEL_FINL static primitive from_bool(bool x) {
            static const primitive full_masks[2] {
                _cvtu32_mask16(0),
                _cvtu32_mask16(-1)
            };

            return full_masks[x];
        }

    };

    //=====================================================
    // Mask functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(mask16x32i m) {
        return popcount(_mm512_mask2int(m));
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask16x32i m) {
        return _mm512_mask2int(m);
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask16x32i m) {
        return 0xFFFF == _mm512_mask2int(m);
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask16x32i m) {
        return !all(m);
    }



    template<>
    class Vector<std::int32_t, 16> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 16;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::int32_t;

        using primitive = __m512i;

        using mask = Vector_mask<scalar, width>;

        template<class U>
        using rebind_type = Vector<U, width>;

        template<int M>
        using rebind_width = Vector<scalar, M>;

        //=================================================
        // Constructors
        //=================================================

        AVEL_FINL explicit Vector(vec16x32u v):
            content(v) {}

        AVEL_FINL explicit Vector(Vector<float, width> v);

        AVEL_FINL explicit Vector(mask m):
            content(_mm512_mask_blend_epi32(m, _mm512_setzero_epi32(), _mm512_set1_epi32(1))) {}

        AVEL_FINL explicit Vector(const primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(const scalar x):
            content(_mm512_set1_epi32(x)) {}

        AVEL_FINL explicit Vector(const std::array<scalar, width>& array):
            content(_mm512_loadu_si512(array.data())) {}

        Vector() = default;
        Vector(const Vector&) = default;
        Vector(Vector&&) = default;
        ~Vector() = default;

        //=================================================
        // Static creation functions
        //=================================================

        AVEL_FINL static Vector zeros() {
            return Vector{_mm512_setzero_si512()};
        }

        AVEL_FINL static Vector ones() {
            primitive reg = _mm512_undefined_epi32();
            return Vector{_mm512_ternarylogic_epi32(reg, reg, reg, 0xFF)};
        }

        //=================================================
        // Assignment operators
        //=================================================

        Vector& operator=(const Vector&) = default;
        Vector& operator=(Vector&&) = default;

        AVEL_FINL Vector& operator=(primitive p) {
            this->content = p;
            return *this;
        }

        AVEL_FINL Vector& operator=(scalar x) {
            content = _mm512_set1_epi32(x);
            return *this;
        }

        //=================================================
        // Comparison operators
        //=================================================

        AVEL_FINL mask operator==(const Vector vec) const {
            return mask{_mm512_cmpeq_epi32_mask(content, vec.content)};
        }

        AVEL_FINL mask operator!=(const Vector vec) const {
            return ~(*this == vec);
        }

        AVEL_FINL mask operator<(const Vector vec) const {
            return mask{_mm512_cmplt_epi32_mask(content, vec.content)};
        }

        AVEL_FINL mask operator<=(const Vector vec) const {
            return ~mask{*this > vec};
        }

        AVEL_FINL mask operator>(const Vector vec) const {
            return mask{_mm512_cmpgt_epi32_mask(content, vec.content)};
        }

        AVEL_FINL mask operator>=(const Vector vec) const {
            return ~mask{*this < vec};
        }

        //=================================================
        // Unary arithmetic operators
        //=================================================

        AVEL_FINL Vector operator+() {
            return *this;
        }

        AVEL_FINL Vector operator-() const {
            return zeros() - *this;
        }

        //=================================================
        // Arithmetic assignment operators
        //=================================================

        AVEL_FINL Vector& operator+=(Vector rhs) {
            content = _mm512_add_epi32(content, rhs);
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            content = _mm512_sub_epi32(content, rhs);
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            content = _mm512_mullo_epi32(content, rhs);
            return *this;
        }

        AVEL_FINL Vector& operator/=(Vector rhs) {
            auto results = div(*this, rhs);
            content = results.quot;
            return *this;
        }

        AVEL_FINL Vector& operator%=(const Vector rhs) {
            auto results = div(*this, rhs);
            content = results.rem;
            return *this;
        }

        //=================================================
        // Arithmetic operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend Vector operator+(Vector lhs, Vector rhs) {
            lhs += rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator-(Vector lhs, Vector rhs) {
            lhs -= rhs;
            return lhs;
        }

        [[nodiscard]]
       AVEL_FINL friend Vector operator*(Vector lhs, Vector rhs) {
            lhs *= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator/(Vector lhs, Vector rhs) {
            lhs /= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator%(Vector lhs, Vector rhs) {
            lhs %= rhs;
            return lhs;
        }

        //=================================================
        // Increment/Decrement operators
        //=================================================

        AVEL_FINL Vector& operator++() {
            *this += Vector{1};
            return *this;
        }

        AVEL_FINL Vector operator++(int) {
            auto temp = *this;
            *this += Vector{1};
            return temp;
        }

        AVEL_FINL Vector& operator--() {
            *this -= Vector{1};
            return *this;
        }

        AVEL_FINL Vector operator--(int) {
            auto temp = *this;
            *this -= Vector{1};
            return temp;
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector& operator&=(Vector vec) {
            content = _mm512_and_si512(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector vec) {
            content = _mm512_or_si512(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector vec) {
            content = _mm512_xor_si512(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector operator<<=(long long s) {
            content = _mm512_sll_epi32(content, _mm_cvtsi64_si128(s));
            return *this;
        }

        AVEL_FINL Vector operator>>=(long long s) {
            content = _mm512_sra_epi32(content, _mm_cvtsi64_si128(s));
            return *this;
        }

        AVEL_FINL Vector operator<<=(vec16x32i s) {
            content = _mm512_sllv_epi32(content, primitive(s));
            return *this;
        }

        AVEL_FINL Vector operator>>=(vec16x32i s) {
            content = _mm512_srav_epi32(content, primitive(s));
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator~() const {
            return Vector{_mm512_andnot_si512(content, ones().content)};
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator&(Vector lhs, Vector rhs) {
            lhs &= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator|(Vector lhs, Vector rhs) {
            lhs |= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator^(Vector lhs, Vector rhs) {
            lhs ^= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator<<(Vector lhs, std::uint32_t rhs) {
            lhs <<= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator>>(Vector lhs, std::uint32_t rhs) {
            lhs >>= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator<<(Vector lhs, Vector rhs) {
            lhs <<= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator>>(Vector lhs, Vector rhs) {
            lhs >>= rhs;
            return lhs;
        }

        //=================================================
        // Conversions
        //=================================================

        [[nodiscard]]
        AVEL_FINL operator primitive() const {
            return content;
        }

        [[nodiscard]]
        AVEL_FINL explicit operator mask() const {
            #if defined(AVEL_AVX512F)
            return mask{_mm512_test_epi32_mask(content, content)};
            #endif
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

    };

    //=====================================================
    // Delayed definitions
    //=====================================================



    //=====================================================
    // Arrangement operations
    //=====================================================

    template<std::uint32_t N>
    AVEL_FINL std::uint32_t extract(vec16x32i v) {
        static_assert(N <= vec16x32i::width, "Specified index does not exist");
        typename std::enable_if<N <= vec16x32i::width, int>::type dummy_variable = 0;

        //return _mm512_extract_epi32(decay(v), N);
    }

    template<std::uint32_t N>
    AVEL_FINL vec16x32i insert(vec16x32i v, std::uint32_t x) {
        static_assert(N <= vec16x32i::width, "Specified index does not exist");
        typename std::enable_if<N <= vec16x32i::width, int>::type dummy_variable = 0;

        //return vec16x32i{_mm512_insert_epi32(decay(v), x, N)};
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec16x32i broadcast_bits(mask16x32i m) {
        return vec16x32i{_mm512_movm_epi32(m)};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32i blend(vec16x32i a, vec16x32i b, mask16x32i m) {
        return vec16x32i{_mm512_mask_blend_epi32(m, a, b)};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32i max(vec16x32i a, vec16x32i b) {
        return vec16x32i{_mm512_max_epi32(a, b)};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32i min(vec16x32i a, vec16x32i b) {
        return vec16x32i{_mm512_min_epi32(a, b)};
    }

    [[nodiscard]]
    AVEL_FINL std::pair<vec16x32i, vec16x32i> minmax(vec16x32i a, vec16x32i b) {
        return {
            vec16x32i{_mm512_min_epi32(a, b)},
            vec16x32i{_mm512_max_epi32(a, b)}
        };
    }

    [[nodiscard]]
    AVEL_FINL vec16x32i clamp(vec16x32i x, vec16x32i lo, vec16x32i hi) {
        return vec16x32i{min(max(x, lo), hi)};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32i midpoint(vec16x32i a, vec16x32i b) {
        const vec16x32u offset{0x80000000};

        #if defined(AVEL_AVX512DQ)
        auto x = static_cast<vec16x32u>(a) ^ offset;
        auto y = static_cast<vec16x32u>(b) ^ offset;

        return vec16x32i{midpoint(x, y) ^ offset};
        #else
        auto x = static_cast<vec16x32u>(a) + offset;
        auto y = static_cast<vec16x32u>(b) + offset;

        return vec16x32i{midpoint(x, y) + offset};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x32i average(vec16x32i a, vec16x32i b) {
        const vec16x32u addition_mask{0x80000000};

        auto x = vec16x32u{a} ^ addition_mask;
        auto y = vec16x32u{b} ^ addition_mask;

        return vec16x32i{average(x, y) ^ addition_mask};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32i abs(vec16x32i v) {
        return vec16x32i{_mm512_abs_epi32(v)};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32i neg_abs(vec16x32i v) {
        return -vec16x32i{_mm512_abs_epi32(v)};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x32i load<vec16x32i>(const std::int32_t* ptr) {
        return vec16x32i{_mm512_loadu_si512(ptr)};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x32i aligned_load<vec16x32i>(const std::int32_t* ptr) {
        return vec16x32i{_mm512_load_si512(ptr)};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x32u gather<vec16x32u>(const std::uint32_t* ptr, vec16x32i indices) {
        return vec16x32u{_mm512_i32gather_epi32(indices, ptr, sizeof(std::uint32_t))};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x32i gather<vec16x32i>(const std::int32_t* ptr, vec16x32i indices) {
        return vec16x32i{_mm512_i32gather_epi32(indices, ptr, sizeof(std::int32_t))};
    }

    AVEL_FINL void store(std::int32_t* ptr, vec16x32i v) {
        _mm512_storeu_si512(ptr, v);
    }

    AVEL_FINL void aligned_store(std::int32_t* ptr, vec16x32i v) {
        _mm512_store_si512(ptr, v);
    }

    AVEL_FINL void scatter(std::uint32_t* ptr, vec16x32i indices, vec16x32u v) {
        _mm512_i32scatter_epi32(avel::bit_cast<int*>(ptr), indices, decay(v), sizeof(std::uint32_t));
    }

    AVEL_FINL void scatter(std::int32_t* ptr, vec16x32i indices, vec16x32i v) {
        _mm512_i32scatter_epi32(ptr, indices, v, sizeof(std::int32_t));
    }



    [[nodiscard]]
    AVEL_FINL std::array<std::int32_t, 16> to_array(vec16x32i v) {
        alignas(64) std::array<std::int32_t, 16> array{};
        aligned_store(array.data(), v);
        return array;
    }

    //=====================================================
    // Integer vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec16x32i> div(vec16x32i numerator, vec16x32i denominator) {
        vec16x32i quotient{};

        mask16x32i sign_mask0 = (numerator < vec16x32i{});
        mask16x32i sign_mask1 = (denominator < vec16x32i{});

        mask16x32i sign_mask2 = sign_mask0 ^ sign_mask1;

        //TODO: Compute i more appropriately

        std::int32_t i = 31;

        for (; (i-- > 0) && any(mask16x32i(numerator));) {
            mask16x32i b = ((numerator >> i) >= denominator);
            numerator -= (broadcast_bits(b) & (denominator << i));
            quotient |= (vec16x32i{b} << i);
        }

        //Adjust quotient's sign. Should be xor of operands' signs
        quotient = blend(quotient, -numerator, sign_mask2);

        //Adjust numerator's sign. Should be same sign as it was originally
        numerator = blend(numerator, -numerator, sign_mask0);

        return {quotient, numerator};
    }

    [[nodiscard]]
    AVEL_FINL vec16x32u popcount(vec16x32i v) {
        return popcount(bit_cast<vec16x32u>(v));
    }

    [[nodiscard]]
    AVEL_FINL vec16x32i byteswap(vec16x32i v) {
        return byteswap(bit_cast<vec16x32i>(v));
    }

    [[nodiscard]]
    AVEL_FINL vec16x32u countl_zero(vec16x32i x) {
        return countl_zero(bit_cast<vec16x32u>(x));
    }

    [[nodiscard]]
    AVEL_FINL vec16x32u countl_one(vec16x32i x) {
        return countl_zero(bit_cast<vec16x32u>(x));
    }

    [[nodiscard]]
    AVEL_FINL vec16x32u countr_zero(vec16x32i x) {
        return countr_zero(bit_cast<vec16x32u>(x));
    }

    [[nodiscard]]
    AVEL_FINL vec16x32u countr_one(vec16x32i x) {
        return countr_one(bit_cast<vec16x32u>(x));
    }

    [[nodiscard]]
    AVEL_FINL vec16x32u bit_width(vec16x32i x) {
        return bit_width(bit_cast<vec16x32u>(x));
    }

    [[nodiscard]]
    AVEL_FINL vec16x32u bit_floor(vec16x32i x) {
        return countr_one(bit_cast<vec16x32u>(x));
    }

    [[nodiscard]]
    AVEL_FINL vec16x32u bit_ceil(vec16x32i x) {
        return bit_ceil(bit_cast<vec16x32u>(x));
    }

    [[nodiscard]]
    AVEL_FINL mask16x32i has_single_bit(vec16x32i x) {
        return mask16x32i(has_single_bit(bit_cast<vec16x32u>(x)));
    }

    [[nodiscard]]
    AVEL_FINL vec16x32i rotl(vec16x32i v, std::uint32_t s) {
        return bit_cast<vec16x32i>(rotl(bit_cast<vec16x32u>(v), s));
    }

    [[nodiscard]]
    AVEL_FINL vec16x32i rotl(vec16x32i v, vec16x32u s) {
        return bit_cast<vec16x32i>(rotl(bit_cast<vec16x32u>(v), bit_cast<vec16x32u>(s)));
    }

    [[nodiscard]]
    AVEL_FINL vec16x32i rotr(vec16x32i v, std::uint32_t s) {
        return bit_cast<vec16x32i>(rotr(bit_cast<vec16x32u>(v), s));
    }

    [[nodiscard]]
    AVEL_FINL vec16x32i rotr(vec16x32i v, vec16x32u s) {
        return bit_cast<vec16x32i>(rotr(bit_cast<vec16x32u>(v), bit_cast<vec16x32u>(s)));
    }

}

#endif //AVEL_VEC16X32I_HPP
