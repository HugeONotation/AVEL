namespace avel {

    using vec16x32u = Vector<std::uint32_t, 16>;

    using mask16x32u = Vector_mask<std::uint32_t, 16>;

    template<>
    class Vector_mask<std::uint32_t, 16> {
    public:

        //=================================================
        // Type aliases
        //=================================================

        using primitive = avel::mask_primitive<std::uint32_t, 16>::type;

        //=================================================
        // Constructor
        //=================================================

        AVEL_FINL explicit Vector_mask(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector_mask(bool x):
            content(from_bool(x)) {}

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

        AVEL_FINL bool operator==(Vector_mask rhs) const noexcept {
            return (_cvtmask16_u32(content) == _cvtmask16_u32(rhs));
        }

        AVEL_FINL bool operator!=(Vector_mask rhs) const noexcept {
            return (_cvtmask16_u32(content) != _cvtmask16_u32(rhs));
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

        AVEL_FINL Vector_mask operator&(Vector_mask rhs) const {
            return Vector_mask{_kand_mask16(content, rhs.content)};
        }

        AVEL_FINL Vector_mask operator|(Vector_mask rhs) const {
            return Vector_mask{_kor_mask16(content, rhs.content)};
        }

        AVEL_FINL Vector_mask operator^(Vector_mask rhs) const {
            return Vector_mask{_kxor_mask16(content, rhs.content)};
        }

        //=================================================
        // Accessor
        //=================================================

        bool operator[](int i) const {
            return _cvtmask16_u32(content) & (1 << i);
        }

        //=================================================
        // Conversion operators
        //=================================================

        AVEL_FINL operator primitive() const {
            return content;
        }

        AVEL_FINL operator bool() const {
            return 0xFFFF == _mm512_mask2int(content);
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

        //=================================================
        // Helper functions
        //=================================================

        static primitive from_bool(bool x) {
            static const primitive full_masks[2] {
                _cvtu32_mask16(0),
                _cvtu32_mask16(-1)
            };

            return full_masks[x];
        }

    };


    template<>
    class Vector<std::uint32_t, 16> {
    public:

        //=================================================
        // Type aliases
        //=================================================

        constexpr static unsigned width = 16;

        using scalar_type = std::uint32_t;

        using primitive = avel::vector_primitive<scalar_type, width>::type;

        using mask = Vector_mask<scalar_type, width>;

        template<class U>
        using rebind_type = Vector<U, width>;

        template<int M>
        using rebind_width = Vector<scalar_type, M>;

        //=================================================
        // Constructors
        //=================================================

        AVEL_FINL explicit Vector(Vector<std::int32_t, width> v);

        AVEL_FINL explicit Vector(Vector<float, width> v);

        AVEL_FINL explicit Vector(mask m):
            content(_mm512_mask_blend_epi32(m, _mm512_setzero_epi32(), _mm512_set1_epi32(1))) {}

        /*
        AVEL_FINL Vector(
            std::uint32_t a, std::uint32_t b, std::uint32_t c, std::uint32_t d,
            std::uint32_t e, std::uint32_t f, std::uint32_t g, std::uint32_t h,
            std::uint32_t i, std::uint32_t j, std::uint32_t k, std::uint32_t l,
            std::uint32_t m, std::uint32_t n, std::uint32_t o, std::uint32_t p
            ):
            content(_mm512_setr_epi32(
                a, b, c, d,
                e, f, g, h,
                i, j, k, l,
                m, n, o, p
            )) {}
        */

        AVEL_FINL explicit Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar_type x):
            content(_mm512_set1_epi32(x)) {}

        AVEL_FINL explicit Vector(const scalar_type* x):
            content(_mm512_loadu_si512(reinterpret_cast<const __m512i*>(x))) {}

        AVEL_FINL explicit Vector(const std::array<scalar_type, width>& array):
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

        AVEL_FINL Vector& operator=(const Vector&) = default;
        AVEL_FINL Vector& operator=(Vector&&) = default;

        AVEL_FINL Vector& operator=(primitive p) {
            this->content = p;
            return *this;
        }

        AVEL_FINL Vector& operator=(scalar_type x) {
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

        AVEL_FINL Vector& operator+=(Vector vec) {
            content = _mm512_add_epi32(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector vec) {
            content = _mm512_sub_epi32(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector vec) {
            content = _mm512_mul_epu32(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator/=(Vector vec) {
            Vector quotient{};

            for (std::uint32_t i = 32; (i-- > 0) && (*this != zeros());) {
                mask b = (*this >> i) >= vec;

                #if defined(AVEL_AVX512VL)
                *this = _mm512_mask_sub_epi32(*this, b, *this, (vec << i));
                #elif defined(AVEL_SSE2) || defined(AVEL_NEON)
                *this -= (Vector{rimitive(b)} & (vec << i));
                #endif

                *this -= (-Vector{b} & (vec << i));
                quotient |= (Vector{b} << i);
            }

            *this = quotient;

            return *this;
        }

        AVEL_FINL Vector& operator%=(const Vector vec) {
            for (std::uint32_t i = 32; (i-- > 0) && bool(*this != zeros());) {
                mask b = (*this >> i) >= vec;

                #if defined(AVEL_AVX512VL)
                *this = _mm512_mask_sub_epi32(*this, b, *this, (vec << i));
                #elif defined(AVEL_SSE2) || defined(AVEL_NEON)
                *this -= (Vector{primitive(b)} & (vec << i));
                #endif

                *this -= (-Vector{b} & (vec << i));
            }

            return *this;
        }

        //=================================================
        // Arithmetic operators
        //=================================================

        AVEL_FINL Vector operator+(const Vector vec) const {
            return Vector{_mm512_add_epi32(content, vec.content)};
        }

        AVEL_FINL Vector operator-(const Vector vec) const {
            return Vector{_mm512_sub_epi32(content, vec.content)};
        }

        AVEL_FINL Vector operator*(const Vector vec) const {
            return Vector{_mm512_mul_epu32(content, vec.content)};
        }

        AVEL_FINL Vector operator/(const Vector vec) const {
            //TODO: Provide better implementation
            alignas(alignof(scalar_type) * width) scalar_type array0[width];
            alignas(alignof(scalar_type) * width) scalar_type array1[width];

            _mm512_store_si512(reinterpret_cast<primitive*>(array0), content);
            _mm512_store_si512(reinterpret_cast<primitive*>(array1), vec.content);

            for (int i = 0; i < width; ++i) {
                array0[i] = array0[i] / array1[i];
            }

            return Vector{_mm512_load_si512(reinterpret_cast<const primitive*>(array0))};
        }

        AVEL_FINL Vector operator%(const Vector vec) const {
            //TODO: Provide better implementation
            alignas(alignof(scalar_type) * width) scalar_type array0[width];
            alignas(alignof(scalar_type) * width) scalar_type array1[width];

            _mm512_store_si512(reinterpret_cast<primitive*>(array0), content);
            _mm512_store_si512(reinterpret_cast<primitive*>(array1), vec.content);

            for (int i = 0; i < width; ++i) {
                array0[i] = array0[i] % array1[i];
            }

            return Vector{_mm512_load_si512(reinterpret_cast<const primitive*>(array0))};
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

        AVEL_FINL Vector& operator<<=(std::uint32_t s) {
            content = _mm512_sll_epi32(content, _mm_loadu_si32(&s));
            return *this;
        }

        AVEL_FINL Vector& operator>>=(std::uint32_t s) {
            content = _mm512_srl_epi32(content, _mm_loadu_si32(&s));
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector s) {
            content = _mm512_sllv_epi32(content, s.content);
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector s) {
            content = _mm512_srlv_epi32(content, s.content);
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        Vector operator~() const {
            return Vector{_mm512_andnot_si512(content, ones().content)};
        }

        Vector operator&(Vector vec) const {
            return Vector{_mm512_and_si512(content, vec.content)};
        }

        Vector operator|(Vector vec) const {
            return Vector{_mm512_or_si512(content, vec.content)};
        }

        Vector operator^(Vector vec) const {
            return Vector{_mm512_xor_si512(content, vec.content)};
        }

        AVEL_FINL Vector operator<<(std::uint64_t s) const {
            return Vector{_mm512_sll_epi32(content, _mm_loadu_si64(&s))};
        }

        AVEL_FINL Vector operator>>(std::uint64_t s) const {
            return Vector{_mm512_srl_epi32(content, _mm_loadu_si64(&s))};
        }

        AVEL_FINL Vector operator<<(Vector s) const {
            return Vector{_mm512_sllv_epi32(content, s.content)};
        }

        AVEL_FINL Vector operator>>(Vector s) const {
            return Vector{_mm512_srlv_epi32(content, s.content)};
        }

        //=================================================
        // Conversions
        //=================================================

        AVEL_FINL std::array<scalar_type, width> as_array() const {
            alignas(alignof(Vector)) std::array<scalar_type, width> array{};

            _mm512_store_si512(reinterpret_cast<__m512i*>(array.data()), content);

            return array;
        }

        AVEL_FINL operator primitive() const {
            return content;
        }

        AVEL_FINL explicit operator mask() const {
            return *this != zeros();
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

    };

    //=====================================================
    // General vector operations
    //=====================================================

    vec16x32u blend(vec16x32u a, vec16x32u b, mask16x32u m) {
        return vec16x32u{_mm512_mask_blend_epi32(m, a, b)};
    }

    vec16x32u max(vec16x32u a, vec16x32u b) {
        return vec16x32u{_mm512_max_epu32(a, b)};
    }

    vec16x32u min(vec16x32u a, vec16x32u b) {
        return vec16x32u{_mm512_min_epu32(a, b)};
    }

    vec16x32u midpoint(vec16x32u a, vec16x32u b) {
        return (a & b) + ((a ^ b) >> 1);
    }

    template<>
    vec16x32u load<vec16x32u>(const std::uint32_t* ptr) {
        return vec16x32u{_mm512_loadu_si512(ptr)};
    }

    template<>
    vec16x32u aligned_load<vec16x32u>(const std::uint32_t* ptr) {
        return vec16x32u{_mm512_load_si512(ptr)};
    }

    template<>
    vec16x32u stream_load<vec16x32u>(const std::uint32_t* ptr) {
        #if defined AVEL_GCC
        return vec16x32u{_mm512_stream_load_si512((void*)ptr)};
        #else
        return vec16x32u{_mm512_stream_load_si512(ptr)};
        #endif
    }

    //Definition of gather deferred until definition of vector of signed integers

    template<>
    AVEL_FINL vec16x32u broadcast<vec16x32u>(std::uint32_t x) {
        return vec16x32u{_mm512_set1_epi32(x)};
    }

    void store(std::uint32_t* ptr, vec16x32u v) {
        _mm512_storeu_si512(reinterpret_cast<__m512i*>(ptr), v);
    }

    void aligned_store(std::uint32_t* ptr, vec16x32u v) {
        _mm512_store_si512(reinterpret_cast<__m512i*>(ptr), v);
    }

    void stream_store(std::uint32_t* ptr, vec16x32u v) {
        _mm512_stream_si512(reinterpret_cast<__m512i*>(ptr), v);
    }

    //=====================================================
    // Bit operations
    //=====================================================

    AVEL_FINL vec16x32u pop_count(vec16x32u v) {
        #if defined(AVEL_AVX512VPOPCNTDQ)
        return vec16x32u{_mm512_popcnt_epi32(v)};
        #elif defined(AVEL_AVX512BITALG)
        auto tmp0 = _mm512_popcnt_epi16(v);
        auto tmp1 = _mm512_slli_epi32(tmp0, 16);

        auto tmp2 = _mm512_add_epi32(tmp0, tmp1);

        return vec16x32u{_mm512_srli_epi32(tmp2, 16)};
        #else
        // https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
        v = v - ((v >> 1) & vec16x32u{0x55555555});                    // reuse input as temporary
        v = (v & vec16x32u{0x33333333}) + ((v >> 2) & vec16x32u{0x33333333});     // temp
        v = ((v + (v >> 4) & vec16x32u{0xF0F0F0F}) * vec16x32u{0x1010101}) >> 24; // count
        return v;
        #endif
    }

    template<int S>
    AVEL_FINL vec16x32u rotl(vec16x32u v) {
        return vec16x32u{_mm512_rol_epi32(v, S)};
    }

    AVEL_FINL vec16x32u rotl(vec16x32u v, vec16x32u s) {
        return vec16x32u{_mm512_rolv_epi32(v, s)};
    }

    template<int S>
    AVEL_FINL vec16x32u rotr(vec16x32u v) {
        return vec16x32u{_mm512_ror_epi32(v, S)};
    }

    AVEL_FINL vec16x32u rotr(vec16x32u v, vec16x32u s) {
        return vec16x32u{_mm512_rorv_epi32(v, s)};
    }

}
