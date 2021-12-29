namespace avel {

    using vec4x32i = Vector<std::int32_t, 4>;

    template<>
    class alignas(16) Vector_mask<std::int32_t, 4> {
    public:

        //=================================================
        // Type aliases
        //=================================================

        using primitive = avel::mask_primitive<std::int32_t, 4>::type;

        //=================================================
        // Constructor
        //=================================================

        AVEL_FINL explicit Vector_mask(const primitive content):
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
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator&=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content = _kand_mask8(content, rhs.content);
            #else
            content = _mm_and_si128(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content = _kor_mask8(content, rhs.content);
            #else
            content = _mm_or_si128(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content = _kxor_mask8(content, rhs.content);
            #else
            content = _mm_xor_si128(content, rhs.content);
            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        AVEL_FINL Vector_mask operator~() const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_knot_mask8(content)};
            #else
            primitive tmp = _mm_undefined_si128();
            return Vector_mask{_mm_andnot_si128(content, _mm_cmpeq_epi32(tmp, tmp))};
            #endif
        }

        AVEL_FINL Vector_mask operator&(Vector_mask rhs) const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_kand_mask8(content, rhs.content)};
            #else
            return Vector_mask{_mm_and_si128(content, rhs.content)};
            #endif
        }

        AVEL_FINL Vector_mask operator|(Vector_mask rhs) const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_kor_mask8(content, rhs.content)};
            #else
            return Vector_mask{_mm_or_si128(content, rhs.content)};
            #endif
        }

        AVEL_FINL Vector_mask operator^(Vector_mask rhs) const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_kxor_mask8(content, rhs.content)};
            #else
            return Vector_mask{_mm_xor_si128(content, rhs.content)};
            #endif
        }

        //=================================================
        // Accessor
        //=================================================

        AVEL_FINL bool operator[](int i) const {
            #if defined(AVEL_AVX512VL)
            unsigned mask = _cvtmask16_u32(__mmask16(content));
            return mask & (1 << i);
            #else
            int mask = _mm_movemask_epi8(content);
            //TODO: Check correctness
            return mask & (1 << (4 * i));
            #endif
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
            #if defined(AVEL_AVX512VL)
            static const primitive full_masks[2] {
                0x00,
                0xFF
            };

            return full_masks[x];
            #else
            static const primitive full_masks[2] {
                {0,  0},
                {-1, -1}
            };

            return full_masks[x];
            #endif
        }

    };


    template<>
    class alignas(16) Vector<std::int32_t, 4> {
    public:

        //=================================================
        // Type aliases
        //=================================================

        using scalar_type = std::int32_t;

        using primitive = avel::vector_primitive<std::int32_t, 4>::type;

        constexpr static unsigned width = 4;

        using mask = Vector_mask<std::int32_t, 4>;

        template<class U>
        using rebind_type = Vector<U, 4>;

        template<int M>
        using rebind_width = Vector<std::int32_t, M>;

        //=================================================
        // Constructors
        //=================================================

        AVEL_FINL explicit Vector(const primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(const scalar_type x):
            content(_mm_set1_epi32(x)) {}

        AVEL_FINL explicit Vector(const scalar_type* x):
            content(_mm_loadu_si128(reinterpret_cast<const primitive*>(x))) {}

        AVEL_FINL explicit Vector(const std::array<scalar_type, width>& array):
            content(_mm_loadu_si128(reinterpret_cast<const primitive*>(array.data()))) {}

        AVEL_FINL explicit Vector(Vector<std::uint32_t, width> v):
            content(primitive(v)) {}

        Vector() = default;
        Vector(const Vector&) = default;
        Vector(Vector&&) = default;
        ~Vector() = default;

        //=================================================
        // Static creation functions
        //=================================================

        AVEL_FINL static Vector zeros() {
            return Vector{_mm_setzero_si128()};
        }

        AVEL_FINL static Vector ones() {
            const primitive zeroes = _mm_setzero_si128();
            return Vector{_mm_cmpeq_epi32(zeroes, zeroes)};
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

        AVEL_FINL Vector& operator=(scalar_type x) {
            content = _mm_set1_epi32(x);
            return *this;
        }

        //=================================================
        // Comparison operators
        //=================================================

        AVEL_FINL mask operator==(const Vector vec) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpeq_epi32_mask(content, vec.content)};
            #else
            return mask{_mm_cmpeq_epi32(content, vec.content)};
            #endif
        }

        AVEL_FINL mask operator!=(const Vector vec) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpneq_epi32_mask(content, vec.content)};
            #else
            return ~(*this == vec);
            #endif
        }

        AVEL_FINL mask operator<(const Vector vec) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmplt_epi32_mask(content, vec.content)};
            #else
            return mask{_mm_cmplt_epi32(content, vec.content)};
            #endif
        }

        AVEL_FINL mask operator<=(const Vector vec) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmple_epi32_mask(content, vec.content)};
            #else
            return ~mask{*this > vec};
            #endif
        }

        AVEL_FINL mask operator>(const Vector vec) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpgt_epi32_mask(content, vec.content)};
            #else
            return mask{_mm_cmpgt_epi32(content, vec.content)};
            #endif
        }

        AVEL_FINL mask operator>=(const Vector vec) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmpge_epi32_mask(content, vec.content)};
            #else
            return ~mask{*this < vec};
            #endif
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
            content = _mm_add_epi32(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector vec) {
            content = _mm_sub_epi32(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector vec) {
            content = _mm_mul_epi32(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator/=(Vector vec) {
            //TODO: Provide better implementation
            alignas(alignof(scalar_type) * width) scalar_type array0[width];
            alignas(alignof(scalar_type) * width) scalar_type array1[width];

            _mm_store_si128(reinterpret_cast<primitive*>(array0), content);
            _mm_store_si128(reinterpret_cast<primitive*>(array1), vec.content);

            for (int i = 0; i < width; ++i) {
                array0[i] = array0[i] / array1[i];
            }

            content = _mm_load_si128(reinterpret_cast<const primitive*>(array0));

            return *this;
        }

        AVEL_FINL Vector& operator%=(const Vector vec) {
            //TODO: Provide better implementation
            alignas(alignof(scalar_type) * width) scalar_type array0[width];
            alignas(alignof(scalar_type) * width) scalar_type array1[width];

            _mm_store_si128(reinterpret_cast<primitive*>(array0), content);
            _mm_store_si128(reinterpret_cast<primitive*>(array1), vec.content);

            for (int i = 0; i < width; ++i) {
                array0[i] = array0[i] % array1[i];
            }

            content = _mm_load_si128(reinterpret_cast<const primitive*>(array0));

            return *this;
        }

        //=================================================
        // Arithmetic operators
        //=================================================

        AVEL_FINL Vector operator+(const Vector vec) const {
            return Vector{_mm_add_epi32(content, vec.content)};
        }

        AVEL_FINL Vector operator-(const Vector vec) const {
            return Vector{_mm_sub_epi32(content, vec.content)};
        }

       AVEL_FINL  Vector operator*(const Vector vec) const {
            return Vector{_mm_mul_epi32(content, vec.content)};
        }

        AVEL_FINL Vector operator/(const Vector vec) const {
            //TODO: Provide better implementation
            alignas(alignof(scalar_type) * width) scalar_type array0[width];
            alignas(alignof(scalar_type) * width) scalar_type array1[width];

            _mm_store_si128(reinterpret_cast<primitive*>(array0), content);
            _mm_store_si128(reinterpret_cast<primitive*>(array1), vec.content);

            for (int i = 0; i < width; ++i) {
                array0[i] = array0[i] / array1[i];
            }

            return Vector{_mm_load_si128(reinterpret_cast<const primitive*>(array0))};
        }

        AVEL_FINL Vector operator%(const Vector vec) const {
            //TODO: Provide better implementation
            alignas(alignof(scalar_type) * width) scalar_type array0[width];
            alignas(alignof(scalar_type) * width) scalar_type array1[width];

            _mm_store_si128(reinterpret_cast<primitive*>(array0), content);
            _mm_store_si128(reinterpret_cast<primitive*>(array1), vec.content);

            for (int i = 0; i < width; ++i) {
                array0[i] = array0[i] % array1[i];
            }

            return Vector{_mm_load_si128(reinterpret_cast<const primitive*>(array0))};
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
            content = _mm_and_si128(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector vec) {
            content = _mm_or_si128(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector vec) {
            content = _mm_xor_si128(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator<<=(std::uint32_t s) {
            content = _mm_sll_epi32(content, _mm_loadu_si32(&s));
            return *this;
        }

        AVEL_FINL Vector& operator>>=(std::uint32_t s) {
            content = _mm_sra_epi32(content, _mm_loadu_si32(&s));
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector<std::uint32_t, width> s) {
            #if defined(AVEL_AVX2)
            content = _mm_sllv_epi32(content, primitive(s));
            #else
            alignas(alignof(Vector)) auto v = as_array();
            auto t = s.as_array();

            for (int i = 0; i < width; ++i) {
                if (32 <= t[i]) {
                    v[i] = 0x00;
                } else {
                    v[i] = v[i] << t[i];
                }
            }

            content = _mm_load_si128((const primitive*)v.data());
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector<std::uint32_t, width> s) {
            #if defined(AVEL_AVX2)
            content = _mm_srav_epi32(content, primitive(s));
            #else
            alignas(alignof(Vector)) auto v = as_array();
            auto t = s.as_array();

            #if __cplusplus < 202002L
            for (int i = 0; i < width; ++i) {
                // If the amount shifted is exactly the integer width, the
                // behavior is still undefined. This problem is apparent on
                // Clang so must be explicitly handled.
                bool is_too_big = t[i] >= (CHAR_BIT * sizeof(std::uint32_t));

                // TODO: This branch could be eliminated using some bitwise
                // manipulation

                if (is_too_big) {
                    // Fill with sign bit
                    v[i] = std::int32_t(-(std::uint32_t(v[i]) >> 31));
                    continue;
                }

                // Shifting a negative value pre C++20 is undefined behaviors
                // By shifting as unsigned integer this is avoided
                // Since right shift should shift in ones, and unsigned right
                // shift shifts in zeroes, double negation allows for "shifting
                // in ones".
                // All supported platforms perform conversion between signed
                // and unsigned types as expected
                v[i] = std::int32_t(~(std::uint32_t(~v[i]) >> t[i]));
            }
            #else
            for (int i = 0; i < width; ++i) {
                if (32 <= s[i]) {
                    v[i] = 0x00;
                } else {
                    v[i] = v[i] << s[i];
                }
            }
            #endif
            content = _mm_load_si128((const primitive*)v.data());

            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        AVEL_FINL Vector operator~() const {
            return Vector{_mm_andnot_si128(content, ones().content)};
        }

        AVEL_FINL Vector operator&(Vector vec) const {
            return Vector{_mm_and_si128(content, vec.content)};
        }

        AVEL_FINL Vector operator|(Vector vec) const {
            return Vector{_mm_or_si128(content, vec.content)};
        }

        AVEL_FINL Vector operator^(Vector vec) const {
            return Vector{_mm_xor_si128(content, vec.content)};
        }

        AVEL_FINL Vector operator<<(std::uint64_t s) const {
            return Vector{_mm_sll_epi32(content, _mm_loadu_si64(&s))};
        }

        AVEL_FINL Vector operator>>(std::uint64_t s) const {
            return Vector{_mm_sra_epi32(content, _mm_loadu_si64(&s))};
        }

        AVEL_FINL Vector operator<<(Vector<std::uint32_t, width> vec) const {
            #if defined(AVEL_AVX2)
            return Vector{_mm_sllv_epi32(content, primitive(vec))};
            #else
            auto v = as_array();
            auto s = vec.as_array();

            for (int i = 0; i < width; ++i) {
                if (32 <= s[i]) {
                    v[i] = 0x00;
                } else {
                    v[i] = v[i] << s[i];
                }
            }

            return Vector{v.data()};
            #endif
        }

        AVEL_FINL Vector operator>>(Vector<std::uint32_t, width> vec) const {
            #if defined(AVEL_AVX2)
            return Vector{_mm_srav_epi32(content, primitive(vec))};
            #else
            auto v = as_array();
            auto s = vec.as_array();

            #if __cplusplus < 202002L
            for (int i = 0; i < width; ++i) {
                // If the amount shifted is exactly the integer width, the
                // behavior is still undefined. This problem is apparent on
                // Clang so must be explicitly handled.
                bool is_too_big = s[i] >= (CHAR_BIT * sizeof(std::uint32_t));

                // TODO: This branch could be eliminated using some bitwise
                // manipulation

                if (is_too_big) {
                    // Fill with sign bit
                    v[i] = std::int32_t(-(std::uint32_t(v[i]) >> 31));
                    continue;
                }

                // Shifting a negative value pre C++20 is undefined behaviors
                // By shifting as unsigned integer this is avoided
                // Since right shift should shift in ones, and unsigned right
                // shift shifts in zeroes, double negation allows for "shifting
                // in ones".
                // All supported platforms perform conversion between signed
                // and unsigned types as expected
                v[i] = std::int32_t(~(std::uint32_t(~v[i]) >> s[i]));
            }
            #else
            for (int i = 0; i < width; ++i) {
                if (32 <= s[i]) {
                    v[i] = 0x00;
                } else {
                    v[i] = v[i] << s[i];
                }
            }
            #endif
            return Vector{v.data()};

            #endif
        }

        //=================================================
        // Conversions
        //=================================================

        AVEL_FINL std::array<scalar_type, width> as_array() const {
            alignas(alignof(Vector)) std::array<scalar_type, width> array{};

            _mm_store_si128(reinterpret_cast<primitive*>(array.data()), content);

            return array;
        }

        AVEL_FINL operator primitive() const {
            return content;
        }

        AVEL_FINL explicit operator mask() const {
            return *this == zeros();
        }

        AVEL_FINL explicit operator Vector<std::uint32_t, width>() const {
            return Vector<std::uint32_t, width>{content};
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

    };

}
