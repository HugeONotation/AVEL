namespace avel {

    using vec4x32f = Vector<float, 4>;

    using mask4x32f = Vector_mask<float, 4>;

    AVEL_FINL vec4x32f trunc(vec4x32f x);
    AVEL_FINL vec4x32f abs(vec4x32f v);
    AVEL_FINL vec4x32f blend(vec4x32f, vec4x32f, mask4x32f);
    AVEL_FINL mask4x32f isunordered(vec4x32f, vec4x32f);
    AVEL_FINL mask4x32f signbit(vec4x32f x);

    template<>
    class Vector_mask<float, 4> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static unsigned width = 4;

        //=================================================
        // Type aliases
        //=================================================

        #if defined(AVEL_AVX512VL)
        using primitive = __mmask8;
        #elif defined(AVEL_SSE)
        using primitive = __m128;
        #endif

        //=================================================
        // Constructor
        //=================================================

        //AVEL_FINL explicit Vector_mask(Vector_mask<std::uint32_t, 4> v);

        //AVEL_FINL explicit Vector_mask(Vector_mask<std::int32_t, 4> v);

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
        // Comparison operators
        //=================================================

        AVEL_FINL bool operator==(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            return (_cvtmask16_u32(content) == _cvtmask16_u32(rhs.content));
            #elif defined(AVEL_SSE2)
            return 0xF == _mm_movemask_ps(_mm_cmpeq_ps(content, rhs.content));
            #endif
        }

        AVEL_FINL bool operator!=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            return (_cvtmask16_u32(content) != _cvtmask16_u32(rhs.content));
            #elif defined(AVEL_SSE2)
            return 0xF != _mm_movemask_ps(_mm_cmpeq_ps(content, rhs.content));
            #endif
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator&=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content = _kand_mask8(content, rhs.content);
            #else
            content = _mm_and_ps(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content = _kor_mask8(content, rhs.content);
            #else
            content = _mm_or_ps(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL)
            content = _kxor_mask8(content, rhs.content);
            #else
            content = _mm_xor_ps(content, rhs.content);
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
            return Vector_mask{_mm_sub_ps(_mm_setzero_ps(), content)};
            #endif
        }

        AVEL_FINL Vector_mask operator&(Vector_mask rhs) const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_kand_mask8(content, rhs.content)};
            #else
            return Vector_mask{_mm_and_ps(content, rhs.content)};
            #endif
        }

        AVEL_FINL Vector_mask operator|(Vector_mask rhs) const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_kor_mask8(content, rhs.content)};
            #else
            return Vector_mask{_mm_or_ps(content, rhs.content)};
            #endif
        }

        AVEL_FINL Vector_mask operator^(Vector_mask rhs) const {
            #if defined(AVEL_AVX512VL)
            return Vector_mask{_kxor_mask8(content, rhs.content)};
            #else
            return Vector_mask{_mm_xor_ps(content, rhs.content)};
            #endif
        }

        //=================================================
        // Accessors
        //=================================================

        AVEL_FINL bool operator[](int i) const {
            #if defined(AVEL_AVX512VL)
            unsigned mask = _cvtmask16_u32(__mmask16(content));
            return mask & (1 << i);
            #else
            int mask = _mm_movemask_ps(content);
            return mask & (1 << i);
            #endif
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

    //=====================================================
    // Mask functions
    //=====================================================

    std::uint32_t count(mask4x32f m) {
        #if defined(AVEL_AVX512VL)
        return popcount(_mm512_mask2int(m));
        #elif defined(AVEL_SSE2)
        return popcount(_mm_movemask_ps(m));
        #endif
    }

    bool any(mask4x32f m) {
        #if defined(AVEL_AVX512VL)
        return _mm512_mask2int(m);
        #elif defined(AVEL_SSE2)
        return _mm_movemask_ps(m);
        #endif
    }

    bool all(mask4x32f m) {
        #if defined(AVEL_AVX512VL)
        return 0x0F == _mm512_mask2int(m);
        #elif defined(AVEL_SSE2)
        return 0x0F == _mm_movemask_ps(m);
        #endif
    }

    //=====================================================
    // Delayed definitions
    //=====================================================

    AVEL_FINL mask4x32u::Vector_mask(mask4x32f x):
    #if defined(AVEL_AVX512VL)
        content(x) {}
    #elif defined(AVEL_SSE2)
        content(_mm_castps_si128(x)) {}
    #endif

    AVEL_FINL avel::mask4x32i::Vector_mask(mask4x32f x):
    #if defined(AVEL_AVX512VL)
        content(x) {}
    #elif defined(AVEL_SSE2)
        content(_mm_castps_si128(x)) {}
    #endif


    template<>
    class alignas(16) Vector<float, 4> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static unsigned width = 4;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = float;

        using primitive = __m128;

        using mask = Vector_mask<scalar, width>;

        template<class U>
        using rebind_type = Vector<U, width>;

        template<int M>
        using rebind_width = Vector<scalar, M>;

        //=================================================
        // Constructors
        //=================================================

        AVEL_FINL explicit Vector(vec4x32u v);

        AVEL_FINL explicit Vector(vec4x32i v):
            content(_mm_cvtepi32_ps(v)) {}

        AVEL_FINL explicit Vector(mask m):
            content(blend(Vector{0.0f}, Vector{1.0f}, m)) {}

        /*
        AVEL_FINL explicit Vector(float a, float b, float c, float d):
            content(_mm_setr_ps(a, b, c, d)) {}
        */

        AVEL_FINL explicit Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar x):
            content(_mm_set1_ps(x)) {}

        AVEL_FINL explicit Vector(const scalar* x):
            content(_mm_loadu_ps(x)) {}

        AVEL_FINL explicit Vector(const std::array<scalar, width>& a):
            content(_mm_loadu_ps(a.data())) {}

        Vector() = default;
        Vector(const Vector&) = default;
        Vector(Vector&&) = default;
        ~Vector() = default;

        //=================================================
        // Static creation functions
        //=================================================

        AVEL_FINL static Vector zeros() {
            return Vector{_mm_setzero_ps()};
        }

        AVEL_FINL static Vector ones() {
            const primitive zeroes = _mm_setzero_ps();
            return Vector{_mm_cmpeq_ps(zeroes, zeroes)};
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
            content = _mm_set1_ps(x);
            return *this;
        }

        //=================================================
        // Comparison operators
        //=================================================

        AVEL_FINL mask operator==(Vector vec) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmp_ps_mask(content, vec.content, _CMP_EQ_OQ)};
            #else
            return mask{_mm_cmpeq_ps(content, vec.content)};
            #endif
        }

        AVEL_FINL mask operator!=(Vector vec) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmp_ps_mask(content, vec.content, _CMP_NEQ_OQ)};
            #else
            return mask{_mm_cmpneq_ps(content, vec.content)};
            #endif
        }

        AVEL_FINL mask operator<(Vector vec) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmp_ps_mask(content, vec.content, _CMP_LT_OQ)};
            #else
            return mask{_mm_cmplt_ps(content, vec.content)};
            #endif
        }

        AVEL_FINL mask operator<=(Vector vec) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmp_ps_mask(content, vec.content, _CMP_LE_OQ)};
            #else
            return mask{_mm_cmple_ps(content, vec.content)};
            #endif
        }

        AVEL_FINL mask operator>(Vector vec) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmp_ps_mask(content, vec.content, _CMP_GT_OQ)};
            #else
            return mask{_mm_cmpgt_ps(content, vec.content)};
            #endif
        }

        AVEL_FINL mask operator>=(Vector vec) const {
            #if defined(AVEL_AVX512VL)
            return mask{_mm_cmp_ps_mask(content, vec.content, _CMP_GE_OQ)};
            #else
            return mask{_mm_cmpge_ps(content, vec.content)};
            #endif
        }

        //=================================================
        // Unary arithmetic operators
        //=================================================

        AVEL_FINL Vector operator+() const {
            return *this;
        }

        AVEL_FINL Vector operator-() const {
            return zeros() - *this;
        }

        //=================================================
        // Arithmetic assignment operators
        //=================================================

        AVEL_FINL Vector& operator+=(Vector vec) {
            content = _mm_add_ps(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector vec) {
            content = _mm_sub_ps(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector vec) {
            content = _mm_mul_ps(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator/=(Vector vec) {
            content = _mm_div_ps(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator%=(const Vector vec) {
            content = _mm_sub_ps(content, _mm_mul_ps(trunc(*this / vec), vec));
            return *this;
        }

        //=================================================
        // Arithmetic operators
        //=================================================

        AVEL_FINL Vector operator+(Vector vec) const {
            return Vector{_mm_add_ps(content, vec.content)};
        }

        AVEL_FINL Vector operator-(Vector vec) const {
            return Vector{_mm_sub_ps(content, vec.content)};
        }

        AVEL_FINL Vector operator*(Vector vec) const {
            return Vector{_mm_mul_ps(content, vec.content)};
        }

        AVEL_FINL Vector operator/(Vector vec) const {
            return Vector{_mm_div_ps(content, vec.content)};
        }

        AVEL_FINL Vector operator%(Vector vec) const {
            return Vector{_mm_sub_ps(content, _mm_mul_ps(trunc(*this / vec), vec))};
        }

        //=================================================
        // Increment/Decrement operators
        //=================================================

        AVEL_FINL Vector& operator++() {
            *this += Vector{1.0f};
            return *this;
        }

        AVEL_FINL Vector operator++(int) {
            auto temp = *this;
            *this += Vector{1.0f};
            return temp;
        }

        AVEL_FINL Vector& operator--() {
            *this -= Vector{1.0f};
            return *this;
        }

        AVEL_FINL Vector operator--(int) {
            auto temp = *this;
            *this -= Vector{1.0f};
            return temp;
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector& operator&=(Vector vec) {
            content = _mm_and_ps(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector vec) {
            content = _mm_or_ps(content, vec.content);
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector vec) {
            content = _mm_xor_ps(content, vec.content);
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        AVEL_FINL Vector operator~() const {
            return Vector{_mm_andnot_ps(content, ones().content)};
        }

        AVEL_FINL Vector operator&(Vector vec) const {
            return Vector{_mm_and_ps(content, vec.content)};
        }

        AVEL_FINL Vector operator|(Vector vec) const {
            return Vector{_mm_or_ps(content, vec.content)};
        }

        AVEL_FINL Vector operator^(Vector vec) const {
            return Vector{_mm_xor_ps(content, vec.content)};
        }

        //=================================================
        // Conversions
        //=================================================

        AVEL_FINL std::array<scalar, width> as_array() const {
            alignas(alignof(Vector)) std::array<scalar, width> array{};

            _mm_store_ps(array.data(), content);

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
    // Delayed definitions
    //=====================================================

    AVEL_FINL vec4x32u::Vector(vec4x32f v):
    #if defined(AVEL_AVX512VL)
        content(_mm_cvtps_epu32(v)) {}
    #elif defined(AVEL_SSE2)
        content([&] () {
            auto half = v * vec4x32f{0.5f};

            auto hi = vec4x32u{_mm_cvtps_epi32(half) << 1};
            auto lo = vec4x32u{_mm_cvtps_epi32(v)};

            return hi | lo;
        } ()) {}
    #endif

    AVEL_FINL vec4x32i::Vector(vec4x32f v):
        content(_mm_cvtps_epi32(v)) {}

    template<>
    AVEL_FINL vec4x32f bit_cast<vec4x32f, vec4x32u>(const vec4x32u& v) {
        return vec4x32f{_mm_castsi128_ps(v)};
    }

    template<>
    AVEL_FINL vec4x32u bit_cast<vec4x32u, vec4x32f>(const vec4x32f& v) {
        return vec4x32u{_mm_castps_si128(v)};
    }

    template<>
    AVEL_FINL vec4x32f bit_cast<vec4x32f, vec4x32i>(const vec4x32i& v) {
        return vec4x32f{_mm_castsi128_ps(v)};
    }

    template<>
    AVEL_FINL vec4x32i bit_cast<vec4x32i, vec4x32f>(const vec4x32f& v) {
        return vec4x32i{_mm_castps_si128(v)};
    }

    //=====================================================
    // Forward declarations
    //=====================================================

    AVEL_FINL mask4x32f isnan(vec4x32f v);

    //=====================================================
    // General vector operations
    //=====================================================

    AVEL_FINL vec4x32f blend(vec4x32f a, vec4x32f b, mask4x32f m) {
        #if defined(AVEL_AVX512VL)
            return vec4x32f{_mm_mask_blend_ps(m, a, b)};
        #elif defined(AVEL_SSE41)
        return vec4x32f{_mm_blendv_ps(a, b, m)};
        #else
        auto x = _mm_andnot_ps(m, b);
        auto y = _mm_and_ps(m, a);
        return vec4x32f{_mm_or_ps(x, y)};
        #endif
    }

    AVEL_FINL vec4x32f max(vec4x32f a, vec4x32f b) {
        return vec4x32f{_mm_max_ps(a, b)};
    }

    AVEL_FINL vec4x32f min(vec4x32f a, vec4x32f b) {
        return vec4x32f{_mm_min_ps(a, b)};
    }

    AVEL_FINL vec4x32f clamp(vec4x32f x, vec4x32f lo, vec4x32f hi) {
        return vec4x32f{min(max(x, lo), hi)};
    }

    AVEL_FINL vec4x32f abs(vec4x32f v) {
        return v & ~vec4x32f{float_sign_bit_mask};
    }

    template<>
    AVEL_FINL vec4x32f load<vec4x32f>(const float* ptr) {
        return vec4x32f{_mm_loadu_ps(ptr)};
    }

    template<>
    AVEL_FINL vec4x32f aligned_load<vec4x32f>(const float* ptr) {
        return vec4x32f{_mm_load_ps(ptr)};
    }

    template<>
    AVEL_FINL vec4x32f stream_load<vec4x32f>(const float* ptr) {
        #if defined(AVEL_SSE41)
        return vec4x32f{_mm_castsi128_ps(_mm_stream_load_si128((__m128i*) ptr))};
        #else
        return aligned_load<vec4x32f>(ptr);
        #endif
    }

    template<>
    AVEL_FINL vec4x32f gather<vec4x32f>(const float* ptr, vec4x32i o) {
        #if defined(AVEL_AVX2)
        return vec4x32f{_mm_i32gather_ps(ptr, o, sizeof(float))};
        #else
        auto offset_array = o.as_array();

        __m128 a = _mm_load_ss(ptr + offset_array[0]);
        __m128 b = _mm_load_ss(ptr + offset_array[1]);
        __m128 c = _mm_load_ss(ptr + offset_array[2]);
        __m128 d = _mm_load_ss(ptr + offset_array[3]);

        __m128 abab = _mm_unpacklo_ps(a, b);
        __m128 cdcd = _mm_unpacklo_ps(c, d);

        __m128 abcd = _mm_castpd_ps(_mm_unpacklo_pd(_mm_castps_pd(abab), _mm_castps_pd(cdcd)));

        return vec4x32f{abcd};
        #endif
    }

    template<>
    AVEL_FINL vec4x32f broadcast<vec4x32f>(float x) {
        #if defined(AVEL_AVX2)
        return vec4x32f{_mm_broadcastss_ps(_mm_set_ss(x))};
        #elif defined(AVEL_SSE)
        return vec4x32f{_mm_set1_ps(x)};
        #endif
    }

    AVEL_FINL void store(float* ptr, vec4x32f v) {
        _mm_storeu_ps(ptr, v);
    }

    AVEL_FINL void aligned_store(float* ptr, vec4x32f v) {
        _mm_store_ps(ptr, v);
    }

    AVEL_FINL void stream_store(float* ptr, vec4x32f v) {
        _mm_stream_ps(ptr, v);
    }

    AVEL_FINL void scatter(float* ptr, vec4x32i indices, vec4x32f v) {
        #if defined(AVEL_AVX512VL)
        _mm_i32scatter_ps(ptr, indices, v, sizeof(float));
        #else
        auto i = indices.as_array();

        _mm_store_ss(ptr + i[0], v);
        _mm_store_ss(ptr + i[1], _mm_permute_ps(v, 0x01));
        _mm_store_ss(ptr + i[2], _mm_permute_ps(v, 0x02));
        _mm_store_ss(ptr + i[3], _mm_permute_ps(v, 0x03));
        #endif
    }

    //=====================================================
    // Floating-point vector operations
    //=====================================================

    AVEL_FINL vec4x32f average(vec4x32f a, vec4x32f b) {
        static const auto half = vec4x32f{0.5f};
        return (a - a * half) + (b * half);
    }

    AVEL_FINL vec4x32f epsilon_increment(vec4x32f v);

    AVEL_FINL vec4x32f epsilon_decrement(vec4x32f v);

    AVEL_FINL vec4x32f epsilon_offset(vec4x32f v, vec4x32f o);

    //=====================================================
    // cmath basic operations
    //=====================================================

    AVEL_FINL vec4x32f fabs(vec4x32f v) {
        return abs(v);
    }

    AVEL_FINL vec4x32f fabsf(vec4x32f v) {
        return fabs(v);
    }

    AVEL_FINL vec4x32f fmod(vec4x32f x, vec4x32f y) {
        return x % y;
    }

    AVEL_FINL vec4x32f fmodf(vec4x32f x, vec4x32f y) {
        return fmod(x, y);
    }

    AVEL_FINL vec4x32f remainderf(vec4x32f v);

    AVEL_FINL vec4x32f remquof(vec4x32f v);

    AVEL_FINL vec4x32f fmadd(vec4x32f m, vec4x32f x, vec4x32f b) {
        #if defined(AVEL_FMA)
        return vec4x32f{_mm_fmadd_ps(m, x, b)};
        #else
        return vec4x32f{m * x + b};
        #endif
    }

    AVEL_FINL vec4x32f fmsubb(vec4x32f m, vec4x32f x, vec4x32f b) {
        #if defined(AVEL_FMA)
        return vec4x32f{_mm_fmsub_ps(m, x, b)};
        #else
        return vec4x32f{m * x - b};
        #endif
    }

    AVEL_FINL vec4x32f fnmadd(vec4x32f m, vec4x32f x, vec4x32f b) {
        #if defined(AVEL_FMA)
        return vec4x32f{_mm_fnmadd_ps(m, x, b)};
        #else
        return vec4x32f{-m * x + b};
        #endif
    }

    AVEL_FINL vec4x32f fnmsub(vec4x32f m, vec4x32f x, vec4x32f b) {
        #if defined(AVEL_FMA)
        return vec4x32f{_mm_fnmsub_ps(m, x, b)};
        #else
        return vec4x32f{-m * x - b};
        #endif
    }

    AVEL_FINL vec4x32f fma(vec4x32f m, vec4x32f x, vec4x32f b) {
        return fmadd(m, x, b);
    }

    AVEL_FINL vec4x32f fmaf(vec4x32f m, vec4x32f x, vec4x32f b) {
        return fma(m, x, b);
    }

    AVEL_FINL vec4x32f fmax(vec4x32f a, vec4x32f b) {
        //TODO: Handle case with two NaNs
        return max(a, max(b, vec4x32f(INFINITY)));
    }

    AVEL_FINL vec4x32f fminf(vec4x32f a, vec4x32f b) {
        //TODO: Handle case with two NaNs
        return min(a, min(b, vec4x32f(INFINITY)));
    }

    AVEL_FINL vec4x32f fmin(vec4x32f a, vec4x32f b) {
        return fminf(a, b);
    }

    AVEL_FINL vec4x32f fdim(vec4x32f a, vec4x32f b) {
        //TODO: Make faster?
        auto tmp = max(a - b, vec4x32f{0.0f});
        auto nan_mask = isnan(a) | isnan(b);
        return blend(tmp, vec4x32f{NAN}, nan_mask);
    }

    AVEL_FINL vec4x32f lerp(vec4x32f a, vec4x32f b, vec4x32f t);

    //=====================================================
    // Exponential functions
    //=====================================================

    AVEL_FINL vec4x32f exp(vec4x32f x) {
        //TODO: Replace with vectorized implementation
        auto array = x.as_array();

        for (auto& t: array) {
            t = std::exp(t);
        }

        return aligned_load<vec4x32f>(array.data());
    }

    AVEL_FINL vec4x32f exp2(vec4x32f x) {
        //TODO: Replace with vectorized implementation
        auto array = x.as_array();

        for (auto& t : array) {
            t = std::exp2(t);
        }

        return aligned_load<vec4x32f>(array.data());
    }

    AVEL_FINL vec4x32f expm1(vec4x32f x) {
        //TODO: Replace with vectorized implementation
        auto array = x.as_array();

        for (auto& t : array) {
            t = std::expm1(t);
        }

        return aligned_load<vec4x32f>(array.data());
    }

    AVEL_FINL vec4x32f log(vec4x32f x) {
        //TODO: Replace with vectorized implementation
        auto array = x.as_array();

        for (auto& t : array) {
            t = std::log(t);
        }

        return aligned_load<vec4x32f>(array.data());
    }

    AVEL_FINL vec4x32f log10(vec4x32f x) {
        //TODO: Replace with vectorized implementation
        auto array = x.as_array();

        for (auto& t : array) {
            t = std::log10(t);
        }

        return aligned_load<vec4x32f>(array.data());
    }

    AVEL_FINL vec4x32f log2(vec4x32f x) {
        //TODO: Replace with vectorized implementation
        auto array = x.as_array();

        for (auto& t : array) {
            t = std::log2(t);
        }

        return aligned_load<vec4x32f>(array.data());
    }

    AVEL_FINL vec4x32f log1p(vec4x32f x) {
        //TODO: Replace with vectorized implementation
        auto array = x.as_array();

        for (auto& t : array) {
            t = std::log1p(t);
        }

        return aligned_load<vec4x32f>(array.data());
    }

    //=====================================================
    // Power functions
    //=====================================================

    AVEL_FINL vec4x32f pow(vec4x32f base, vec4x32f exp) {
        //TODO: Replace with vectorized implementation
        auto base_array = base.as_array();
        auto exp_array = exp.as_array();

        for (unsigned i = 0; i < vec4x32f::width; ++i) {
            base_array[i] = std::pow(base_array[i], exp_array[i]);
        }

        return aligned_load<vec4x32f>(base_array.data());
    }

    AVEL_FINL vec4x32f sqrt(vec4x32f x) {
        return vec4x32f{_mm_sqrt_ps(x)};
    }

    AVEL_FINL vec4x32f cbrt(vec4x32f x) {
        //TODO: Replace with vectorized implementation
        auto array = x.as_array();

        for (auto& t : array) {
            t = std::cbrt(t);
        }

        return aligned_load<vec4x32f>(array.data());
    }

    AVEL_FINL vec4x32f hypot(vec4x32f x, vec4x32f y) {
        //TODO: Replace with vectorized implementation
        auto x_array = x.as_array();
        auto y_array = y.as_array();

        for (unsigned i = 0; i < vec4x32f::width; ++i) {
            x_array[i] = std::hypot(x_array[i], y_array[i]);
        }

        return aligned_load<vec4x32f>(x_array.data());
    }

    //=====================================================
    // Trigonometric functions
    //=====================================================

    AVEL_FINL std::array<vec4x32f, 2> sincos(vec4x32f angle) {
        //TODO: Replace with vectorized implementation
        auto array0 = angle.as_array();
        auto array1 = angle.as_array();

        for (unsigned i = 0; i < vec4x32f::width; ++i) {
            #if defined(AVEL_GCC)
            sincosf(array0[i], array0.data() + i, array1.data() + i);
            #elif defined(AVEL_CLANG)
            sincosf(array0[i], array0.data() + i, array1.data() + i);
            #else
            static_assert(false, "Default not implemented");
            #endif
        }

        return {
            aligned_load<vec4x32f>(array0.data()),
            aligned_load<vec4x32f>(array1.data())
        };
    }

    AVEL_FINL vec4x32f sin(vec4x32f angle) {
        //TODO: Replace with vectorized implementation
        auto array = angle.as_array();

        for (auto& t : array) {
            t = std::sin(t);
        }

        return aligned_load<vec4x32f>(array.data());
    }

    AVEL_FINL vec4x32f cos(vec4x32f angle) {
        //TODO: Replace with vectorized implementation
        auto array = angle.as_array();

        for (auto& t : array) {
            t = std::cos(t);
        }

        return aligned_load<vec4x32f>(array.data());
    }

    AVEL_FINL vec4x32f tan(vec4x32f angle) {
        //TODO: Replace with vectorized implementation
        auto array = angle.as_array();

        for (auto& t : array) {
            t = std::tan(t);
        }

        return aligned_load<vec4x32f>(array.data());
    }

    AVEL_FINL vec4x32f asin(vec4x32f x) {
        //TODO: Replace with vectorized implementation
        auto array = x.as_array();

        for (auto& t : array) {
            t = std::asin(t);
        }

        return aligned_load<vec4x32f>(array.data());
    }

    AVEL_FINL vec4x32f acos(vec4x32f x) {
        //TODO: Replace with vectorized implementation
        auto array = x.as_array();

        for (auto& t : array) {
            t = std::acos(t);
        }

        return aligned_load<vec4x32f>(array.data());
    }

    AVEL_FINL vec4x32f atan(vec4x32f x) {
        //TODO: Replace with vectorized implementation
        auto array = x.as_array();

        for (auto& t : array) {
            t = std::atan(t);
        }

        return aligned_load<vec4x32f>(array.data());
    }

    AVEL_FINL vec4x32f atan2(vec4x32f x, vec4x32f y);

    //=====================================================
    // Nearest Integer Floating-point operators
    //=====================================================

    AVEL_FINL vec4x32f ceil(vec4x32f x) {
        #if defined(AVELSSE41)
        return vec4x32f{_mm_round_ps(x, _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC)};
        #else

        alignas(alignof(vec4x32f)) auto arr = x.as_array();

        for (int i = 0; i < vec4x32f::width; ++i) {
            arr[0] = std::ceil(arr[1]);
        }

        return vec4x32f{arr.data()};

        #endif
    }

    AVEL_FINL vec4x32f floor(vec4x32f x) {
        #if defined(AVELSSE41)
        return vec4x32f{_mm_round_ps(x, _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC)};
        #else

        alignas(alignof(vec4x32f)) auto arr = x.as_array();

        for (int i = 0; i < vec4x32f::width; ++i) {
            arr[0] = std::floor(arr[1]);
        }

        return vec4x32f{arr.data()};

        #endif
    }

    AVEL_FINL vec4x32f trunc(vec4x32f x) {
        #if defined(AVEL_SSE41)
        return vec4x32f{_mm_round_ps(x, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC)};
        #else

        const __m128i full = vec4x32i::ones();
        const __m128i data = _mm_castps_si128(x);

        //Extract exponent value to low 8 bits in each element
        __m128i exponents = _mm_srli_epi32(_mm_slli_epi32(data, 1), 24);

        // Generate mask for which bits should be masked out to perform truncation
        __m128i mantissa_mask;
        {
            // Number of mantissa bits that should be masked out, strictly positive.
            // May be greater than 23, the number of explicit mantissa bits
            __m128i shift_amounts = _mm_subs_epu16(exponents, _mm_set1_epi32(118));

            // Evaluates to (-1, 0, 0, 0).
            __m128i mask0001 = _mm_andnot_si128(_mm_slli_si128(full, 4), full);


            //Extract individual shift amounts to lower 64-bit integer and shift individually
            __m128i s0 = _mm_and_si128(mask0001, shift_amounts);
            __m128i m0 = _mm_srl_epi32(full, s0);

            __m128i s1 = _mm_and_si128(mask0001, _mm_srli_si128(shift_amounts, 0x4));
            __m128i m1 = _mm_srl_epi32(full, s1);

            __m128i s2 = _mm_and_si128(mask0001, _mm_srli_si128(shift_amounts, 0x8));
            __m128i m2 = _mm_srl_epi32(full, s2);

            __m128i s3 = _mm_srli_si128(shift_amounts, 0xC);
            __m128i m3 = _mm_srl_epi32(full, s3);


            __m128i tmp0 = _mm_unpacklo_epi32(m0, m2);
            __m128i tmp1 = _mm_unpacklo_epi32(m1, m3);
            mantissa_mask = _mm_unpacklo_epi32(tmp0, tmp1);
        }

        //Fills vector elements with 126
        __m128i lo = _mm_slli_epi32(_mm_srli_epi32(full, 26), 1);

        //Create mask for values that should not be zeroed out because they have exponents greater than 126
        __m128i zmask = _mm_cmplt_epi32(lo, exponents);

        // Mask indicating which bits should be copied in returned values
        __m128i copy_mask = _mm_andnot_si128(mantissa_mask, zmask);

        // Copy only bits indicated by mask
        __m128i ret = _mm_and_si128(copy_mask, data);

        return vec4x32f{_mm_castsi128_ps(ret)};
        #endif
    }

    AVEL_FINL vec4x32f round(vec4x32f v) {
        #if defined(AVELSSE41)
        return vec4x32f{_mm_round_ps(x, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC)};
        #else

        alignas(alignof(vec4x32f)) auto arr = v.as_array();

        //TODO: Vectorize
        for (int i = 0; i < vec4x32f::width; ++i) {
            arr[0] = std::round(arr[1]);
        }

        return vec4x32f{arr.data()};
        #endif
    }

    AVEL_FINL vec4x32f nearbyint(vec4x32f v) {
        #if defined(AVEL_SSE41)
        return vec4x32f{_mm_round_ps(v, _MM_FROUND_CUR_DIRECTION)};
        #elif defined(AVEL_SSE2)
        int mode = std::fegetround();
        switch (mode) {
        case FE_DOWNWARD: return floor(v);
        case FE_TONEAREST: return round(v);
        case FE_TOWARDZERO: return trunc(v);
        case FE_UPWARD: return ceil(v);
        default:
            return vec4x32f{}; //Not sure how to handle such a case
        }
        #endif
    }

    //=====================================================
    // Floating-point manipulation
    //=====================================================

    AVEL_FINL vec4x32f frexp(vec4x32f v, vec4x32f* exp);

    AVEL_FINL vec4x32f ldexp(vec4x32f x, vec4x32f exp);

    AVEL_FINL vec4x32f modf(vec4x32f x, vec4x32f* iptr);

    AVEL_FINL vec4x32f scalebn(vec4x32f x, vec4x32f exp);

    AVEL_FINL vec4x32f ilog(vec4x32f x);

    AVEL_FINL vec4x32f logb(vec4x32f x);

    AVEL_FINL vec4x32f nextafter(vec4x32f from, vec4x32f to) {
        mask4x32f unordered_mask = isunordered(from, to);
        mask4x32f equality_mask = (from == to);
        mask4x32f comparison_mask = (from < to);

        // !((from < 0) ^ (to < 0)) and (|from| < |to|)
        // !(mask0 ^ mask1) and mask2
        mask4x32f mask0 = signbit(from);
        mask4x32f mask1 = signbit(to);
        mask4x32f mask2 = abs(from) < abs(to);

        mask4x32f mask3{~(mask0 ^ mask1) & mask2};


        vec4x32u addend = blend(vec4x32u{-1u}, vec4x32u{1u}, mask4x32u{mask3});
        vec4x32u x = bit_cast<vec4x32u>(from);
        x += addend;

        //TODO: Complete and test implementation

        vec4x32f ret = bit_cast<vec4x32f>(x);
        ret = blend(ret, to, equality_mask);
        return blend(ret, vec4x32f{NAN}, unordered_mask);
    }

    AVEL_FINL vec4x32f copysign(vec4x32f mag, vec4x32f sign) {
        alignas(16) std::uint32_t mask_data[1] = {
            0x80000000
        };

        auto mask = vec4x32f{_mm_set1_ps(mask_data[0])};

        return mask & sign | vec4x32f{_mm_andnot_ps(mask, mag)};
    }

    //=====================================================
    // Classification subroutines
    //=====================================================

    AVEL_FINL vec4x32u fpclassify(vec4x32f v) {
        const vec4x32u fp_infinite{int(FP_INFINITE)};
        const vec4x32u fp_nan{int(FP_NAN)};
        const vec4x32u fp_normal{int(FP_NORMAL)};
        const vec4x32u fp_subnormal{int(FP_SUBNORMAL)};
        const vec4x32u fp_zero{int(FP_ZERO)};

        #if defined(AVEL_AVX512DQ) && defined(AVEL_AVX512VL)
        mask4x32u infinite_mask {_mm_fpclass_ps_mask(v, 0x08 | 0x10)};
        mask4x32u nan_mask      {_mm_fpclass_ps_mask(v, 0x01 | 0x80)};
        mask4x32u subnormal_mask{_mm_fpclass_ps_mask(v, 0x20)};
        mask4x32u zero_mask     {_mm_fpclass_ps_mask(v, 0x02 | 0x04)};
        mask4x32u normal_mask   {~(infinite_mask | nan_mask | subnormal_mask | zero_mask)};
        #elif defined(AVEL_SSE)


        mask4x32u infinite_mask{};
        mask4x32u nan_mask{};
        mask4x32u normal_mask{};
        mask4x32u subnormal_mask{};
        mask4x32u zero_mask{};
        #endif

        return
            blend(vec4x32u{}, fp_infinite,  infinite_mask) |
            blend(vec4x32u{}, fp_nan,       infinite_mask) |
            blend(vec4x32u{}, fp_normal,    infinite_mask) |
            blend(vec4x32u{}, fp_subnormal, infinite_mask) |
            blend(vec4x32u{}, fp_zero,      infinite_mask);
    }

    AVEL_FINL mask4x32f isnan(vec4x32f v) {
        return (v != v);
    }

    AVEL_FINL mask4x32f isfininte(vec4x32f v) {
        #if defined(AVEL_AVX512VL)
        return  vec4x32f{_mm_getexp_ps(v)} != vec4x32f{255.0f};
        #else
        vec4x32f m = vec4x32f{float_exponent_mask};
        return (v & m) == m;
        #endif
    }

    AVEL_FINL mask4x32f isnormal(vec4x32f v) {
        #if defined(AVEL_AVX512VL)
        vec4x32f tmp = vec4x32f{_mm_getexp_ps(v)};
        return  (tmp != vec4x32f::zeros()) & (tmp != vec4x32f{255.0f});
        #else
        vec4x32f tmp = (v & vec4x32f(float_sign_bit_mask));
        return  (tmp != vec4x32f::zeros()) & (tmp != vec4x32f{float_exponent_mask});
        #endif
    }

    AVEL_FINL mask4x32f isunordered(vec4x32f x, vec4x32f y) {
        #if defined(AVEL_AVX512VL)
        return mask4x32f{_mm_cmp_ps_mask(x, y, _CMP_ORD_Q)};
        #elif defined(AVEL_SSE)
        return mask4x32f{_mm_cmpunord_ps(x, y)};
        #endif
    }

    AVEL_FINL mask4x32f signbit(vec4x32f x) {
        #if defined(AVEL_AVX512VL)
        return mask4x32f{_mm_movepi32_mask(_mm_castps_si128(x))};
        #elif defined(AVEL_SSE)
        auto t0 = bit_cast<vec4x32u>(x);
        auto t1 = (t0 != vec4x32u{});
        auto t2 = bit_cast<vec4x32f>(t1);
        return mask4x32f{mask4x32f::primitive(t2)};
        #endif
    }

    //=====================================================
    // Comparison subroutines
    //=====================================================

    AVEL_FINL mask4x32f isgreater(vec4x32f x, vec4x32f y) {
        return x > y;
    }

    AVEL_FINL mask4x32f isgreaterequal(vec4x32f x, vec4x32f y) {
        return x >= y;
    }

    AVEL_FINL mask4x32f isless(vec4x32f x, vec4x32f y) {
        return x < y;
    }

    AVEL_FINL mask4x32f islessequal(vec4x32f x, vec4x32f y) {
        return x <= y;
    }

    AVEL_FINL mask4x32f islessgreater(vec4x32f x, vec4x32f y) {
        return x != y;
    }

}
