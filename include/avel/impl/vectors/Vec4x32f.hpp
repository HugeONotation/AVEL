#ifndef AVEL_VEC4X32F_HPP
#define AVEL_VEC4X32F_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec4x32f = Vector<float, 4>;
    using arr4x32f = std::array<float, 4>;
    using mask4x32f = Vector_mask<float, 4>;

    //=====================================================
    // Forward declarations
    //=====================================================

    AVEL_FINL vec4x32f trunc(vec4x32f x);
    AVEL_FINL vec4x32f abs(vec4x32f v);
    AVEL_FINL vec4x32f fmod(vec4x32f a, vec4x32f b);
    AVEL_FINL vec4x32f blend(mask4x32f, vec4x32f, vec4x32f);
    AVEL_FINL vec4x32f keep(mask4x32f, vec4x32f);
    AVEL_FINL vec4x32f round(vec4x32f);
    AVEL_FINL mask4x32f isunordered(vec4x32f, vec4x32f);
    AVEL_FINL mask4x32f signbit(vec4x32f x);
    AVEL_FINL mask4x32f isnan(vec4x32f v);
    AVEL_FINL mask4x32f isinf(vec4x32f v);
    AVEL_FINL vec4x32f copysign(vec4x32f mag, vec4x32f sign);





    template<>
    class Vector_mask<float, 4> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 4;

        //=================================================
        // Type aliases
        //=================================================

        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        using primitive = __mmask8;
        #elif defined(AVEL_SSE2)
        using primitive = __m128;
        #endif

        #if defined(AVEL_NEON)
        using primitive = uint32x4_t;
        #endif

    private:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

    public:

        //=================================================
        // Constructor
        //=================================================

        template<class U>
        AVEL_FINL explicit Vector_mask(Vector_mask<U, width> v):
            Vector_mask(convert<Vector_mask>(v)[0]) {}

        AVEL_FINL explicit Vector_mask(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector_mask(bool b):
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            content(b ? 0xf : 0x0) {}
        #elif defined(AVEL_SSE2)
            content(_mm_castsi128_ps(_mm_set1_epi32(b ? -1 : 0))) {}
        #endif
        #if defined(AVEL_NEON)
            content(vmovq_n_u32(b ? -1 : 0)) {}
        #endif

        AVEL_FINL explicit Vector_mask(const arr4xb& arr) {
            static_assert(
                sizeof(bool) == 1,
                "Implementation assumes bool occupy a single byte"
            );

            #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)) || defined(AVEL_AVX10_1)
            auto array_data = _mm_maskz_loadu_epi8(0xf, arr.data());
            content = primitive(_mm_test_epi8_mask(array_data, array_data));

            #elif defined(AVEL_AVX512VL)
            auto array_data = _mm_maskz_loadu_epi32(0x1, arr.data());
            auto expanded = _mm_cvtepi8_epi32(array_data);
            content = _mm_test_epi32_mask(expanded, expanded);

            #elif defined(AVEL_SSE2)
            auto array_data = _mm_cvtsi32_si128(avel::bit_cast<std::uint32_t>(arr));
            array_data = _mm_unpacklo_epi8(array_data, array_data);
            array_data = _mm_unpacklo_epi16(array_data, array_data);
            content = _mm_castsi128_ps(_mm_cmplt_epi32(_mm_setzero_si128(), array_data));

            #endif

            #if defined(AVEL_NEON)
            auto array_data = vsetq_lane_u32(
                bit_cast<std::uint32_t>(arr),
                vdupq_n_u32(0x00),
                0x00
            );

            auto zeros = vdupq_n_u8(0x00);
            auto t0 = vreinterpretq_u8_u32(array_data);
            auto mask0 = vcltq_u8(zeros, t0);
            auto mask1 = vzipq_u8(mask0, mask0).val[0];
            auto mask2 = vzipq_u8(mask1, mask1).val[0];

            content = vreinterpretq_u32_u8(mask2);
            #endif
        }

        Vector_mask() = default;
        Vector_mask(const Vector_mask&) = default;
        Vector_mask(Vector_mask&&) = default;
        ~Vector_mask() = default;

        //=================================================
        // Assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator=(bool b) {
            *this = Vector_mask{b};
            return *this;
        }

        AVEL_FINL Vector_mask& operator=(primitive p) {
            content = p;
            return *this;
        }

        Vector_mask& operator=(const Vector_mask&) = default;
        Vector_mask& operator=(Vector_mask&&) = default;

        //=================================================
        // Comparison operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend bool operator==(Vector_mask lhs, Vector_mask rhs) {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            return lhs.content == rhs.content;

            #elif defined(AVEL_SSE2)
            return !_mm_movemask_ps(_mm_xor_ps(lhs.content, rhs.content));
            #endif

            #if defined(AVEL_AARCH64)
            auto min = vminvq_u32(vceqq_u32(decay(lhs), decay(rhs)));
            return min == 0xFFFFFFFF;

            #elif defined(AVEL_NEON)
            return !(lhs != rhs);

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask lhs, Vector_mask rhs) {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            return lhs.content != rhs.content;

            #elif defined(AVEL_SSE2)
            return _mm_movemask_ps(_mm_xor_ps(lhs.content, rhs.content));
            #endif

            #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
            auto min = vminvq_u32(vceqq_u32(decay(lhs), decay(rhs)));
            return min == 0x00;

            #elif defined(AVEL_NEON)
            auto diff = vsubq_u64(vreinterpretq_u64_u32(decay(lhs)), vreinterpretq_u64_u32(decay(rhs)));
            auto shifted = vqshlq_n_u64(diff, 63);
            auto shiftedx4 = vreinterpretq_u32_u64(shifted);
            auto x = vgetq_lane_u32(shiftedx4, 0x1);
            auto y = vgetq_lane_u32(shiftedx4, 0x3);
            return (x | y) >> 31;

            #endif
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator&=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            content &= rhs.content;

            #elif defined(AVEL_SSE2)
            content = _mm_and_ps(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vandq_u32(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            content |= rhs.content;

            #elif defined(AVEL_SSE2)
            content = _mm_or_ps(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vorrq_u32(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            content ^= rhs.content;

            #elif defined(AVEL_SSE2)
            content = _mm_xor_ps(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = veorq_u32(content, decay(rhs));
            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        AVEL_FINL Vector_mask operator!() const {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            return Vector_mask{primitive(content ^ 0xf)};

            #elif defined(AVEL_SSE2)
            return Vector_mask{_mm_andnot_ps(content, _mm_castsi128_ps(_mm_set1_epi32(-1)))};
            #endif

            #if defined(AVEL_NEON)
            return Vector_mask{vmvnq_u32(content)};
            #endif
        }

        AVEL_VECTOR_MASK_BINARY_BITWISE_OPERATORS

        //=================================================
        // Conversion operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL explicit operator primitive() const {
            return content;
        }

    };

    //=====================================================
    // Mask functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(mask4x32f m) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return popcount(decay(m));

        #elif defined(AVEL_SSE2)
        return popcount(_mm_movemask_ps(decay(m)));
        #endif

        #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
        auto t0 = vnegq_s32(vreinterpretq_s32_u32(decay(m)));
        return static_cast<std::uint32_t>(vaddvq_s32(t0));

        #elif defined(AVEL_NEON)
        auto t0 = vreinterpretq_u32_s32(vnegq_s32(vreinterpretq_s32_u32(decay(m))));
        auto t1 = vpadd_u32(vget_low_u32(t0), vget_high_u32(t0));
        auto t2 = vpadd_u32(t1, t1);

        return static_cast<std::uint32_t>(vget_lane_u32(t2, 0));

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask4x32f m) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return _mm512_mask2int(decay(m));
        #elif defined(AVEL_SSE2)
        return _mm_movemask_ps(decay(m));
        #endif

        #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
        return vmaxvq_u32(decay(m)) != 0x00;

        #elif defined(AVEL_NEON)
        auto t0 = decay(m);
        auto t1 = vpmax_u32(vget_low_u32(t0), vget_high_u32(t0));
        auto t2 = vpmax_u32(t1, t1);
        return vget_lane_u32(t2, 0x0) != 0x00;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask4x32f m) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return 0x0F == _mm512_mask2int(decay(m));

        #elif defined(AVEL_SSE2)
        return 0x0F == _mm_movemask_ps(decay(m));
        #endif

        #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
        return vminvq_u8(vreinterpretq_u8_u32(decay(m))) == 0xFF;

        #elif defined(AVEL_NEON)
        auto t0 = decay(m);
        auto t1 = vpmin_u32(vget_low_u32(t0), vget_high_u32(t0));
        auto t2 = vpmin_u32(t1, t1);
        return vget_lane_u32(t2, 0x0) == 0xFFFFFFFF;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask4x32f m) {
        return !any(m);
    }

    template<std::uint32_t N>
    [[nodiscard]]
    AVEL_FINL bool extract(mask4x32f m) {
        static_assert(N < mask4x32f::width, "Specified index does not exist");
        typename std::enable_if<N < mask4x32f::width, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return decay(m) & (1 << N);

        #elif defined(AVEL_SSE2)
        return _mm_movemask_ps(decay(m)) & (1 << N);

        #endif

        #if defined(AVEL_NEON)
        return vgetq_lane_u32(decay(m), N);

        #endif
    }

    template<std::uint32_t N>
    [[nodiscard]]
    AVEL_FINL mask4x32f insert(mask4x32f m, bool b) {
        static_assert(N < mask4x32f::width, "Specified index does not exist");
        typename std::enable_if<N < mask4x32f::width, int>::type dummy_variable = 0;

        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        auto mask = b << N;
        return mask4x32f{__mmask8((decay(m) & ~mask) | mask)};

        #elif defined(AVEL_SSE4_1)
        auto mask = avel::bit_cast<float>(b ? -1 : 0);
        return mask4x32f{_mm_insert_ps(decay(m), _mm_set_ss(mask), N << 4)};

        #elif defined(AVEL_SSE2)
        auto mask = _mm_cvtsi32_si128(b ? -1 : 0);
        std::memcpy(reinterpret_cast<char*>(&m) + N * sizeof(float), &mask, sizeof(float));
        return m;

        #endif

        #if defined(AVEL_NEON)
        return mask4x32f{vsetq_lane_u32(b ? -1 : 0, decay(m), N)};

        #endif
    }





    template<>
    class Vector<float, 4> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 4;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = float;

        #if defined(AVEL_SSE2)
        using primitive = __m128;
        #endif

        #if defined(AVEL_NEON)
        using primitive = float32x4_t;
        #endif

        using mask = Vector_mask<scalar, width>;

        template<class U>
        using rebind_type = Vector<U, width>;

        template<std::uint32_t M>
        using rebind_width = Vector<scalar, M>;

    private:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

    public:

        //=================================================
        // Constructors
        //=================================================

        template<class U>
        AVEL_FINL explicit Vector(Vector<U, width> v):
            Vector(convert<Vector>(v)[0]) {}

        AVEL_FINL explicit Vector(mask m):
            content(keep(m, Vector{1.0f})) {}

        AVEL_FINL explicit Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar x):
        #if defined(AVEL_SSE2)
            content(_mm_set1_ps(x)) {}
        #endif
        #if defined(AVEL_NEON)
            content(vdupq_n_f32(x)) {}
        #endif

        AVEL_FINL explicit Vector(const arr4x32f& arr):
        #if defined(AVEL_SSE2)
            content(_mm_loadu_ps(arr.data())) {}
        #endif
        #if defined(AVEL_NEON)
            content(vld1q_f32(arr.data())) {}
        #endif

        Vector() = default;
        Vector(const Vector&) = default;
        Vector(Vector&&) = default;
        ~Vector() = default;

        //=================================================
        // Assignment operators
        //=================================================

        AVEL_FINL Vector& operator=(scalar x) {
            *this = Vector{x};
            return *this;
        }

        AVEL_FINL Vector& operator=(primitive p) {
            content = p;
            return *this;
        }

        Vector& operator=(const Vector&) = default;
        Vector& operator=(Vector&&) = default;

        //=================================================
        // Comparison operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend mask operator==(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            return mask{_mm_cmp_ps_mask(decay(lhs), decay(rhs), _CMP_EQ_OS)};

            #elif defined(AVEL_SSE2)
            return mask{_mm_cmpeq_ps(decay(lhs), decay(rhs))};
            #endif

            #if defined(AVEL_NEON)
            return mask{vceqq_f32(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            return mask{_mm_cmp_ps_mask(decay(lhs), decay(rhs), _CMP_NEQ_OS)};

            #elif defined(AVEL_SSE2)
            return mask{_mm_cmpneq_ps(decay(lhs), decay(rhs))};
            #endif

            #if defined(AVEL_NEON)
            return !(lhs == rhs);
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            return mask{_mm_cmp_ps_mask(decay(lhs), decay(rhs), _CMP_LT_OS)};

            #elif defined(AVEL_SSE2)
            return mask{_mm_cmplt_ps(decay(lhs), decay(rhs))};
            #endif

            #if defined(AVEL_NEON)
            return mask{vcltq_f32(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            return mask{_mm_cmp_ps_mask(decay(lhs), decay(rhs), _CMP_LE_OS)};

            #elif defined(AVEL_SSE2)
            return mask{_mm_cmple_ps(decay(lhs), decay(rhs))};
            #endif

            #if defined(AVEL_NEON)
            return mask{vcleq_f32(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            return mask{_mm_cmp_ps_mask(decay(lhs), decay(rhs), _CMP_GT_OS)};

            #elif defined(AVEL_SSE2)
            return mask{_mm_cmpgt_ps(decay(lhs), decay(rhs))};
            #endif

            #if defined(AVEL_NEON)
            return mask{vcgtq_f32(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            return mask{_mm_cmp_ps_mask(decay(lhs), decay(rhs), _CMP_GE_OS)};

            #elif defined(AVEL_SSE2)
            return mask{_mm_cmpge_ps(decay(lhs), decay(rhs))};
            #endif

            #if defined(AVEL_NEON)
            return mask{vcgeq_f32(decay(lhs), decay(rhs))};
            #endif
        }

        //=================================================
        // Unary arithmetic operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator+() const {
            return *this;
        }

        [[nodiscard]]
        AVEL_FINL Vector operator-() const {
            #if defined(AVEL_SSE2)
            return Vector{_mm_sub_ps(_mm_setzero_ps(), content)};
            #endif

            #if defined(AVEL_NEON)
            return Vector{vnegq_f32(content)};
            #endif
        }

        //=================================================
        // Arithmetic assignment operators
        //=================================================

        AVEL_FINL Vector& operator+=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_add_ps(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vaddq_f32(content, decay(rhs));
            #endif

            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_sub_ps(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vsubq_f32(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_mul_ps(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vmulq_f32(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator/=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_div_ps(content, rhs.content);
            #endif

            #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
            content = vdivq_f32(content, decay(rhs));

            #elif defined(AVEL_NEON)
            //TODO: Implement a better emulation
            auto recip = vrecpeq_f32(decay(rhs));
            content = vmulq_f32(content, recip);

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator%=(Vector rhs) {
            content = avel::fmod(*this, rhs).content;
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

        /*
        [[nodiscard]]
        AVEL_FINL friend Vector operator%(Vector lhs, Vector rhs) {
            lhs %= rhs;
            return lhs;
        }
        */

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
        // Conversions
        //=================================================

        [[nodiscard]]
        AVEL_FINL explicit operator primitive() const {
            return content;
        }

        [[nodiscard]]
        AVEL_FINL explicit operator mask() const {
            #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
            auto t = _mm_castps_si128(content);
            return mask{_mm_test_epi32_mask(t, t)};

            #elif defined(AVEL_AVX)
            return mask{_mm_cmp_ps(content, _mm_setzero_ps(), _CMP_NEQ_UQ)};

            #elif defined(AVEL_SSE2)
            return mask{_mm_cmpneq_ps(content, _mm_setzero_ps())};

            #endif

            #if defined(AVEL_NEON)
            return *this != Vector{0x00};
            #endif
        }

    };

    static_assert(
        4 * sizeof(float) == sizeof(vec4x32f),
        "Vector was not of the expected size!"
    );

    //=====================================================
    // Arrangement operations
    //=====================================================

    template<std::uint32_t N>
    AVEL_FINL float extract(vec4x32f v) {
        static_assert(N < vec4x32f::width, "Specified index does not exist");
        typename std::enable_if<N < vec4x32f::width, int>::type dummy_variable = 0;

        #if defined(AVEL_SSE4_1)
        return avel::bit_cast<float>(_mm_extract_ps(decay(v), N));

        #elif defined(AVEL_SSE2)
        switch (N) {
            case 3: return _mm_cvtss_f32(_mm_shuffle_ps(decay(v), decay(v), 0x03));
            case 2: return _mm_cvtss_f32(_mm_shuffle_ps(decay(v), decay(v), 0x02));
            case 1: return _mm_cvtss_f32(_mm_shuffle_ps(decay(v), decay(v), 0x01));
            case 0: return _mm_cvtss_f32(decay(v));
        }

        #endif

        #if defined(AVEL_NEON)
        return vgetq_lane_f32(decay(v), N);
        #endif
    }

    template<std::uint32_t N>
    AVEL_FINL vec4x32f insert(vec4x32f v, float x) {
        static_assert(N < vec4x32f::width, "Specified index does not exist");
        typename std::enable_if<N < vec4x32f::width, int>::type dummy_variable = 0;

        #if defined(AVEL_SSE4_1)
        return vec4x32f{_mm_insert_ps(decay(v), _mm_set_ss(x), N << 4)};

        #elif defined(AVEL_SSE2) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))
        auto tmp = decay(v);
        tmp[N] = x;
        return vec4x32f{tmp};

        #elif defined(AVEL_SSE2) && defined(AVEL_MSVC)
        // MSVC is generally just horrible at optimizing any implementation of 
        // this so just keep it simple
        std::memcpy(reinterpret_cast<char*>(&v) + N * sizeof(float), &x, sizeof(float));

        return v;

        #endif

        #if defined(AVEL_NEON)
        return vec4x32f{vsetq_lane_f32(x, decay(v), N)};

        #endif
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec4x32f x) {
        return count(mask4x32f{x});
    }

    [[nodiscard]]
    AVEL_FINL bool any(vec4x32f x) {
        return any(mask4x32f{x});
    }

    [[nodiscard]]
    AVEL_FINL bool all(vec4x32f x) {
        return all(mask4x32f{x});
    }

    [[nodiscard]]
    AVEL_FINL bool none(vec4x32f x) {
        return none(mask4x32f{x});
    }

    [[nodiscard]]
    AVEL_FINL vec4x32f keep(mask4x32f m, vec4x32f v) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return vec4x32f{_mm_maskz_mov_ps(decay(m), decay(v))};

        #elif defined(AVEL_SSE2)
        return vec4x32f{_mm_and_ps(decay(m), decay(v))};
        #endif

        #if defined(AVEL_NEON)
        return vec4x32f{vbslq_f32(decay(m), decay(v), vdupq_n_f32(0.0f))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32f clear(mask4x32f m, vec4x32f v) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return vec4x32f{_mm_maskz_mov_ps(decay(!m), decay(v))};

        #elif defined(AVEL_SSE2)
        return vec4x32f{_mm_andnot_ps(decay(m), decay(v))};
        #endif

        #if defined(AVEL_NEON)
        return vec4x32f{vbslq_f32(decay(m), vdupq_n_f32(0.0f), decay(v))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32f blend(mask4x32f m, vec4x32f a, vec4x32f b) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return vec4x32f{_mm_mask_blend_ps(decay(m), decay(b), decay(a))};

        #elif defined(AVEL_SSE4_1)
        return vec4x32f{_mm_blendv_ps(decay(b), decay(a), decay(m))};

        #elif defined(AVEL_SSE2)
        auto x = _mm_andnot_ps(decay(m), decay(b));
        auto y = _mm_and_ps(decay(m), decay(a));
        return vec4x32f{_mm_or_ps(x, y)};

        #endif

        #if defined(AVEL_NEON)
        return vec4x32f{vbslq_f32(decay(m), decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32f byteswap(vec4x32f v) {
        return bit_cast<vec4x32f>(byteswap(bit_cast<vec4x32u>(v)));
    }

    [[nodiscard]]
    AVEL_FINL vec4x32f max(vec4x32f a, vec4x32f b) {
        #if defined(AVEL_SSE2)
        return vec4x32f{_mm_max_ps(decay(b), decay(a))};
        #endif

        #if defined(AVEL_NEON)
        auto m = a < b;
        return blend(m, b, a);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32f min(vec4x32f a, vec4x32f b) {
        #if defined(AVEL_SSE2)
        return vec4x32f{_mm_min_ps(decay(b), decay(a))};
        #endif

        #if defined(AVEL_NEON)
        auto m = a < b;
        return blend(m, a, b);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec4x32f, 2> minmax(vec4x32f a, vec4x32f b) {
        #if defined(AVEL_SSE2)
        return std::array<vec4x32f, 2>{
            vec4x32f{_mm_min_ps(decay(b), decay(a))},
            vec4x32f{_mm_max_ps(decay(b), decay(a))}
        };
        #endif

        #if defined(AVEL_NEON)
        auto mask = a < b;
        return {
            blend(mask, a, b),
            blend(mask, b, a)
        };
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32f clamp(vec4x32f x, vec4x32f lo, vec4x32f hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec4x32f negate(mask4x32f m, vec4x32f v) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return vec4x32f{_mm_mask_sub_ps(decay(v), decay(m), _mm_setzero_ps(), decay(v))};

        #elif defined(AVEL_SSE2)
        auto negation_mask = _mm_and_ps(decay(m), _mm_set1_ps(float_sign_bit_mask));
        return vec4x32f{_mm_xor_ps(decay(v), negation_mask)};

        #endif

        #if defined(AVEL_NEON)
        auto negation_mask = vandq_u32(decay(m), vdupq_n_u32(float_sign_bit_mask_bits));
        return vec4x32f{vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(decay(v)), negation_mask))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32f abs(vec4x32f v) {
        #if defined(AVEL_SSE2)
        return vec4x32f{_mm_andnot_ps(_mm_set1_ps(float_sign_bit_mask), decay(v))};
        #endif

        #if defined(AVEL_NEON)
        return vec4x32f{vabsq_f32(decay(v))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32f neg_abs(vec4x32f v) {
        #if defined(AVEL_SSE2)
        return vec4x32f{_mm_or_ps(_mm_set1_ps(float_sign_bit_mask), decay(v))};
        #endif

        #if defined(AVEL_NEON)
        return vec4x32f{vsubq_f32(vdupq_n_f32(0.0f), vabsq_f32(decay(v)))};
        #endif
    }

    //=====================================================
    // Load/Store Operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x32f load<vec4x32f>(const float* ptr, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        auto mask = (n >= 4) ? -1 : (1 << n) - 1;
        return vec4x32f{_mm_maskz_loadu_ps(mask, ptr)};

        #elif defined(AVEL_SSE2)
        switch (n) {
            case 0: return vec4x32f{_mm_setzero_ps()};
            case 1: return vec4x32f{_mm_load_ss(ptr)};
            case 2: return vec4x32f{_mm_castpd_ps(_mm_load_sd(reinterpret_cast<const double*>(ptr)))};
            case 3: return vec4x32f{
                _mm_castpd_ps(
                    _mm_unpacklo_pd(
                        _mm_load_sd(reinterpret_cast<const double*>(ptr)),
                        _mm_castps_pd(_mm_load_ss(ptr + 2))
                    )
                )
            };
            default: return vec4x32f{_mm_loadu_ps(ptr)};
        }

        #endif

        #if defined(AVEL_NEON)
        vec4x32f ret{0.0f};
        switch (n) {
            default: ret = insert<3>(ret, ptr[3]);;
            case 3:  ret = insert<2>(ret, ptr[2]);
            case 2:  ret = insert<1>(ret, ptr[1]);
            case 1:  ret = insert<0>(ret, ptr[0]);
            case 0: ; //Nothing to do
        }

        return ret;
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x32f load<vec4x32f, vec4x32f::width>(const float* ptr) {
        #if defined(AVEL_SSE2)
        return vec4x32f{_mm_loadu_ps(ptr)};
        #endif

        #if defined(AVEL_NEON)
        return vec4x32f{vld1q_f32(ptr)};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x32f aligned_load<vec4x32f>(const float* ptr, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        auto mask = (n >= 4) ? -1 : (1 << n) - 1;
        return vec4x32f{_mm_maskz_load_ps(mask, ptr)};

        #elif defined(AVEL_SSE2)
        return load<vec4x32f>(ptr, n);

        #endif

        #if defined(AVEL_NEON)
        return load<vec4x32f>(ptr, n);

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x32f aligned_load<vec4x32f, vec4x32f::width>(const float* ptr) {
        #if defined(AVEL_SSE2)
        return vec4x32f{_mm_load_ps(ptr)};
        #endif

        #if defined(AVEL_NEON) && __cplusplus >= 202002
            return vec4x32f{vld1q_f32(std::assume_aligned<alignof(vec4x32f)>(ptr))};
        #elif defined(AVEL_NEON) && (defined(AVEL_GCC) || defined(AVEL_CLANG))
            auto* p = reinterpret_cast<const float*>(__builtin_assume_aligned(ptr, alignof(vec4x32f)));
            return vec4x32f{vld1q_f32(p)};
        #elif defined(AVEL_NEON)
            return vec4x32f{vld1q_f32(ptr)};
        #endif
    }



    template<>
    [[nodiscard]]
    AVEL_FINL vec4x32f gather<vec4x32f>(const float* ptr, vec4x32i indices, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        auto mask = (n >= 4) ? -1 : (1 << n) - 1;
        return vec4x32f{_mm_mmask_i32gather_ps(_mm_setzero_ps(), mask, decay(indices), ptr, sizeof(float))};

        #elif defined(AVEL_AVX2)
        n = min(n, vec4x32i::width);

        auto undef = _mm_undefined_si128();
        auto full = _mm_cmpeq_epi8(undef, undef);

        auto w = vec4x32i::width;
        auto h = vec4x32i::width / 2;

        auto lo = _mm_srl_epi64(full, _mm_cvtsi64_si128(32 * (h - min(h, n))));
        auto hi = _mm_srl_epi64(full, _mm_cvtsi64_si128(32 * (w - min(w, n))));
        auto mask = _mm_castsi128_ps(_mm_unpacklo_epi64(lo, hi));

        return vec4x32f{_mm_mask_i32gather_ps(_mm_setzero_ps(), ptr, decay(indices), mask, sizeof(float))};

        #elif defined(AVEL_SSE2)
        vec4x32f ret{0.0f};

        switch (n) {
            default: ret = insert<3>(ret, ptr[extract<3>(indices)]);
            case 3:  ret = insert<2>(ret, ptr[extract<2>(indices)]);
            case 2:  ret = insert<1>(ret, ptr[extract<1>(indices)]);
            case 1:  ret = insert<0>(ret, ptr[extract<0>(indices)]);
            case 0: ;
        }

        return ret;

        #endif

        #if defined(AVEL_NEON)
        vec4x32f ret{0.0f};

        switch (n) {
            default: ret = insert<3>(ret, ptr[extract<3>(indices)]);
            case 3:  ret = insert<2>(ret, ptr[extract<2>(indices)]);
            case 2:  ret = insert<1>(ret, ptr[extract<1>(indices)]);
            case 1:  ret = insert<0>(ret, ptr[extract<0>(indices)]);
            case 0: ;
        }

        return ret;

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec4x32f gather<vec4x32f>(const float* ptr, vec4x32i indices) {
        #if defined(AVEL_AVX2)
        return vec4x32f{_mm_i32gather_ps(ptr, decay(indices), sizeof(float))};

        #elif defined(AVEL_SSE2)
        auto offset_array = to_array(indices);

        auto a = _mm_load_ss(ptr + offset_array[0]);
        auto b = _mm_load_ss(ptr + offset_array[1]);
        auto c = _mm_load_ss(ptr + offset_array[2]);
        auto d = _mm_load_ss(ptr + offset_array[3]);

        auto abab = _mm_unpacklo_ps(a, b);
        auto cdcd = _mm_unpacklo_ps(c, d);

        auto abcd = _mm_castpd_ps(_mm_unpacklo_pd(_mm_castps_pd(abab), _mm_castps_pd(cdcd)));

        return vec4x32f{abcd};

        #endif

        #if defined(AVEL_NEON)
        auto ret = vdupq_n_f32(0.0f);

        ret = vsetq_lane_f32(ptr[extract<3>(indices)], ret, 0x3);
        ret = vsetq_lane_f32(ptr[extract<2>(indices)], ret, 0x2);
        ret = vsetq_lane_f32(ptr[extract<1>(indices)], ret, 0x1);
        ret = vsetq_lane_f32(ptr[extract<0>(indices)], ret, 0x0);

        return vec4x32f{ret};
        #endif
    }



    AVEL_FINL void store(float* ptr, vec4x32f v, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        auto mask = (n >= 4) ? -1 : (1 << n) - 1;
        _mm_mask_storeu_ps(ptr, mask, decay(v));

        #elif defined(AVEL_SSE2)
        n = min(n, vec4x32u::width);
        auto undef = _mm_undefined_si128();
        auto full = _mm_cmpeq_epi8(undef, undef);

        auto w = vec4x32u::width;
        auto h = vec4x32u::width / 2;

        auto lo = _mm_srl_epi64(full, _mm_cvtsi64_si128(32 * (h - min(h, n))));
        auto hi = _mm_srl_epi64(full, _mm_cvtsi64_si128(32 * (w - min(w, n))));
        auto mask = _mm_unpacklo_epi64(lo, hi);
        _mm_maskmoveu_si128(_mm_castps_si128(decay(v)), mask, reinterpret_cast<char *>(ptr));


        #endif

        #if defined(AVEL_NEON)
        switch (n) {
            default: ptr[3] = extract<3>(v);
            case 3:  ptr[2] = extract<2>(v);
            case 2:  ptr[1] = extract<1>(v);
            case 1:  ptr[0] = extract<0>(v);
            case 0: ;// Nothing to do
        }
        #endif
    }

    template<std::uint32_t N = vec4x32f::width>
    AVEL_FINL void store(float* ptr, vec4x32f v) {
        static_assert(N <= vec4x32f::width, "Cannot store more elements than width of vector");
        typename std::enable_if<N <= vec4x32f::width, int>::type dummy_variable = 0;

        store(ptr, v, N);
    }

    template<>
    AVEL_FINL void store<vec4x32f::width>(float* ptr, vec4x32f v) {
        #if defined(AVEL_SSE2)
        _mm_storeu_ps(ptr, decay(v));
        #endif

        #if defined(AVEL_NEON)
        vst1q_f32(ptr, decay(v));
        #endif
    }



    AVEL_FINL void aligned_store(float* ptr, vec4x32f v, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        auto mask = (n >= 4) ? -1 : (1 << n) - 1;
        _mm_mask_store_ps(ptr, mask, decay(v));

        #elif defined(AVEL_SSE2)
        store(ptr, v, n);

        #endif

        #if defined(AVEL_NEON)
        //TODO: Utilize std::assumed_aligned and alternatives
        store(ptr, v, n);
        #endif
    }

    template<std::uint32_t N = vec4x32f::width>
    AVEL_FINL void aligned_store(float* ptr, vec4x32f v) {
        static_assert(N <= vec4x32f::width, "Cannot store more elements than width of vector");
        typename std::enable_if<N <= vec4x32f::width, int>::type dummy_variable = 0;

        aligned_store(ptr, v, N);
    }

    template<>
    AVEL_FINL void aligned_store<vec4x32f::width>(float* ptr, vec4x32f v) {
        #if defined(AVEL_SSE2)
        _mm_store_ps(ptr, decay(v));
        #endif

        #if defined(AVEL_NEON)
        vst1q_f32(ptr, decay(v));
        #endif
    }



    AVEL_FINL void scatter(float* ptr, vec4x32f v, vec4x32i indices, std::uint32_t n) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        auto mask = (n >= 4) ? -1 : (1 << n) - 1;
        _mm_mask_i32scatter_ps(ptr, mask, decay(indices), decay(v), sizeof(std::uint32_t));

        #elif defined(AVEL_SSE2)
        switch (n) {
            default: ptr[extract<3>(indices)] = extract<3>(v);
            case 3:  ptr[extract<2>(indices)] = extract<2>(v);
            case 2:  ptr[extract<1>(indices)] = extract<1>(v);
            case 1:  ptr[extract<0>(indices)] = extract<0>(v);
            case 0: ; // Nothing to do
        }

        #endif

        #if defined(AVEL_NEON)
        switch (n) {
            default: ptr[extract<3>(indices)] = extract<3>(v);
            case 3:  ptr[extract<2>(indices)] = extract<2>(v);
            case 2:  ptr[extract<1>(indices)] = extract<1>(v);
            case 1:  ptr[extract<0>(indices)] = extract<0>(v);
            case 0: ; // Nothing to do
        }

        #endif
    }

    template<std::uint32_t N = vec4x32f::width>
    AVEL_FINL void scatter(float* ptr, vec4x32f v, vec4x32i indices) {
        scatter(ptr, v, indices, N);
    }

    template<>
    AVEL_FINL void scatter<0>(float* ptr, vec4x32f v, vec4x32i indices) {
        // Don't have to do anything
    }

    template<>
    AVEL_FINL void scatter<vec4x32f::width>(float* ptr, vec4x32f v, vec4x32i indices) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        _mm_i32scatter_ps(ptr, decay(indices), decay(v), sizeof(float));

        #elif defined(AVEL_AVX)
        auto i = to_array(indices);

        _mm_store_ss(ptr + i[0], decay(v));
        _mm_store_ss(ptr + i[1], _mm_permute_ps(decay(v), 0x01));
        _mm_store_ss(ptr + i[2], _mm_permute_ps(decay(v), 0x02));
        _mm_store_ss(ptr + i[3], _mm_permute_ps(decay(v), 0x03));

        #elif defined(AVEL_SSE2)
        auto i = to_array(indices);

        _mm_store_ss(ptr + i[0], decay(v));
        _mm_store_ss(ptr + i[1], _mm_shuffle_ps(decay(v), decay(v), 0x01));
        _mm_store_ss(ptr + i[2], _mm_shuffle_ps(decay(v), decay(v), 0x02));
        _mm_store_ss(ptr + i[3], _mm_shuffle_ps(decay(v), decay(v), 0x03));

        #endif

        #if defined(AVEL_NEON)
        ptr[extract<3>(indices)] = vgetq_lane_f32(decay(v), 0x3);
        ptr[extract<2>(indices)] = vgetq_lane_f32(decay(v), 0x2);
        ptr[extract<1>(indices)] = vgetq_lane_f32(decay(v), 0x1);
        ptr[extract<0>(indices)] = vgetq_lane_f32(decay(v), 0x0);

        #endif
    }



    [[nodiscard]]
    AVEL_FINL arr4x32f to_array(vec4x32f v) {
        alignas(16) std::array<float, 4> array{};
        aligned_store(array.data(), v);
        return array;
    }

    //=====================================================
    // General floating-point math operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec4x32f fmax(vec4x32f a, vec4x32f b) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        return vec4x32f{_mm_range_ps(decay(a), decay(b), 0x05)};

        #elif defined(AVEL_SSE2)
        return blend(avel::isnan(b), a, avel::max(b, a));
        #endif

        #if defined(AVEL_NEON)
        return vec4x32f{vmaxnmq_f32(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32f fmin(vec4x32f a, vec4x32f b) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)
        return vec4x32f{_mm_range_ps(decay(a), decay(b), 0x04)};

        #elif defined(AVEL_SSE2)
        return blend(avel::isnan(b), a, avel::min(b, a));
        #endif

        #if defined(AVEL_NEON)
        return vec4x32f{vminnmq_f32(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32f fdim(vec4x32f x, vec4x32f y) {
        return avel::max(x - y, vec4x32f{0.0f});
    }

    [[nodiscard]]
    AVEL_FINL vec4x32f frac(vec4x32f v) {
        return v - avel::trunc(v);
    }

    //=====================================================
    // Power functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec4x32f sqrt(vec4x32f x) {
        #if defined(AVEL_SSE2)
        return vec4x32f{_mm_sqrt_ps(decay(x))};
        #endif

        #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
        return vec4x32f{vsqrtq_f32(decay(x))};

        #elif defined(AVEL_NEON)
        //TODO: Utilize software emulation
        #endif
    }

    //=====================================================
    // Nearest integer function
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec4x32f ceil(vec4x32f v) {
        #if defined(AVEL_SSE4_1)
        return vec4x32f{_mm_round_ps(decay(v), _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC)};

        #elif defined(AVEL_SSE2)
        auto abs_v = abs(v);

        auto is_integral = _mm_cmple_ps(_mm_set1_ps(8388608.0f), decay(abs_v));
        auto is_nan = _mm_cmpunord_ps(decay(abs_v), decay(abs_v));
        auto is_output_self = _mm_or_ps(is_integral, is_nan);

        auto converted = _mm_cvttps_epi32(decay(v));
        auto reconstructed = _mm_cvtepi32_ps(converted);

        auto is_reconstruction_smaller = _mm_cmplt_ps(reconstructed, decay(v));
        auto corrected_result = _mm_add_ps(reconstructed, _mm_and_ps(is_reconstruction_smaller, _mm_set1_ps(1.0f)));

        return blend(mask4x32f{is_output_self}, v, vec4x32f{corrected_result});

        #endif

        #if defined(AVEL_NEON) && (defined(AVEL_AARCH32) || defined(AVEL_AARCH64))
        return vec4x32f{vrndpq_f32(decay(v))};

        #elif defined(AVEL_NEON)
        auto abs_v = abs(v);

        auto is_integral = vec4x32f{8388608.0f} < abs_v;
        auto is_nan = isnan(abs_v);
        auto is_output_self = is_integral | is_nan;

        auto converted = vcvtq_s32_f32(decay(v));
        auto reconstructed = vec4x32f{vcvtq_f32_s32(converted)};

        auto is_reconstruction_smaller = reconstructed < v;
        auto corrected_result = reconstructed + keep(is_reconstruction_smaller, vec4x32f{1.0f});

        return blend(mask4x32f{is_output_self}, v, vec4x32f{corrected_result});
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32f floor(vec4x32f v) {
        #if defined(AVEL_SSE4_1)
        return vec4x32f{_mm_round_ps(decay(v), _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC)};

        #elif defined(AVEL_SSE2)
        auto abs_v = abs(v);

        auto is_integral = _mm_cmple_ps(_mm_set1_ps(8388608.0f), decay(abs_v));
        auto is_nan = _mm_cmpunord_ps(decay(abs_v), decay(abs_v));
        auto is_output_self = _mm_or_ps(is_integral, is_nan);

        auto converted = _mm_cvttps_epi32(decay(v));
        auto reconstructed = _mm_cvtepi32_ps(converted);

        auto is_reconstruction_smaller = _mm_cmplt_ps(decay(v), reconstructed);
        auto corrected_result = _mm_sub_ps(reconstructed, _mm_and_ps(is_reconstruction_smaller, _mm_set1_ps(1.0f)));

        return blend(mask4x32f{is_output_self}, v, vec4x32f{corrected_result});

        #endif

        #if defined(AVEL_NEON) && (defined(AVEL_AARCH32) || defined(AVEL_AARCH64))
        return vec4x32f{vrndmq_f32(decay(v))};

        #elif defined(AVEL_NEON)
        //TODO: Perform software emulation
        return vec4x32f{};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32f trunc(vec4x32f v) {
        #if defined(AVEL_SSE4_1)
        return vec4x32f{_mm_round_ps(decay(v), _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC)};

        #elif defined(AVEL_SSE2)
        auto abs_v = abs(v);

        // TODO: Could potentially merge these checks together
        auto is_integral = _mm_cmple_ps(_mm_set1_ps(8388608.0f), decay(abs_v));
        auto is_nan = _mm_cmpunord_ps(decay(abs_v), decay(abs_v));
        auto is_output_self = _mm_or_ps(is_integral, is_nan);

        auto converted = _mm_cvttps_epi32(decay(v));
        auto reconstructed = _mm_cvtepi32_ps(converted);

        return blend(mask4x32f{is_output_self}, v, vec4x32f{reconstructed});

        #endif

        #if defined(AVEL_NEON) && (defined(AVEL_AARCH32) || defined(AVEL_AARCH64))
        return vec4x32f{vrndq_f32(decay(v))};

        #elif defined(AVEL_NEON)
        auto abs_v = abs(v);

        // TODO: Could potentially merge these checks together
        auto is_integral = vec4x32f{8388608.0f} < abs_v;
        auto is_nan = isnan(v);
        auto is_output_self = is_integral | is_nan;

        auto converted = vcvtq_s32_f32(decay(v));
        auto reconstructed = vcvtq_f32_s32(converted);

        return blend(is_output_self, v, vec4x32f{reconstructed});

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32f round(vec4x32f v) {
        #if defined(AVEL_SSE2)
        auto whole = trunc(v);
        auto frac = v - whole;

        auto offset = copysign(vec4x32f{1.0f}, v);
        auto should_offset = abs(frac) >= vec4x32f{0.5f};
        auto ret = whole + keep(should_offset, offset);

        return ret;

        /* Solution that works if the current rounding mode is set to nearest
         * Could potentially be used on older ARM implementations which don't support multiple rounding modes
        // The following constant is the value prior to 0.5
        auto offset = avel::copysign(vec4x32f{avel::bit_cast<float>(0x3effffff)}, v);
        auto offsetted = v + offset;
        return avel::trunc(offsetted);
        */
        #endif

        #if defined(AVEL_NEON) && (defined(AVEL_AARCH32) || defined(AVEL_AARCH64))
        return vec4x32f{vrndaq_f32(decay(v))};

        #elif defined(AVEL_NEON)
        auto whole = trunc(v);
        auto frac = v - whole;

        auto offset = copysign(vec4x32f{1.0f}, v);
        auto should_offset = abs(frac) >= vec4x32f{0.5f};
        auto ret = whole + keep(should_offset, offset);

        return ret;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32f nearbyint(vec4x32f v) {
        #if defined(AVEL_SSE4_1)
        return vec4x32f{_mm_round_ps(decay(v), _MM_FROUND_CUR_DIRECTION)};

        #elif defined(AVEL_SSE2)
        int mode = _MM_GET_ROUNDING_MODE();
        switch (mode) {
            case _MM_ROUND_DOWN:        return avel::floor(v);
            case _MM_ROUND_TOWARD_ZERO: return avel::trunc(v);
            case _MM_ROUND_UP:          return avel::ceil(v);
            case _MM_ROUND_NEAREST: {
                auto abs_v = abs(v);

                auto is_integral = _mm_cmple_ps(_mm_set1_ps(8388608.0f), decay(abs_v));
                auto is_nan = _mm_cmpunord_ps(decay(abs_v), decay(abs_v));
                auto is_output_self = _mm_or_ps(is_integral, is_nan);

                auto converted = _mm_cvtps_epi32(decay(v));
                auto reconstructed = _mm_cvtepi32_ps(converted);

                return blend(mask4x32f{is_output_self}, v, vec4x32f{reconstructed});
            }
        default:
            return vec4x32f{0.0f};
            // TODO: Find proper solution.
            // Not sure how to handle such a case.
            // Fall back to scalar code?
        }
        #endif

        #if defined(AVEL_NEON) && (defined(AVEL_AARCH32) || defined(AVEL_AARCH64))
        return vec4x32f{vrndiq_f32(decay(v))};

        #elif defined(AVEL_NEON)
        int mode = std::fegetround();
        switch (mode) {
            case FE_DOWNWARD:   return avel::floor(v);
            case FE_TOWARDZERO: return avel::trunc(v);
            case FE_UPWARD:     return avel::ceil(v);
            case FE_TONEAREST:   {
                auto abs_v = abs(v);

                auto is_integral = vec4x32f{8388608.0f} < abs_v;
                auto is_nan = isnan(v);
                auto is_output_self = is_integral | is_nan;

                auto converted = vcvtq_s32_f32(decay(v));
                auto reconstructed = vcvtq_f32_s32(converted);

                return blend(mask4x32f{is_output_self}, v, vec4x32f{reconstructed});
            }
        default:
            return vec4x32f{0.0f};
            // TODO: Find proper solution.
            // Not sure how to handle such a case.
            // Fall back to scalar code?
        }

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32f rint(vec4x32f v) {
        return nearbyint(v);
    }

    //=====================================================
    // Floating-point manipulation
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec4x32f frexp(vec4x32f v, vec4x32i* exp) {
        #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)) || defined(AVEL_AVX10_1)
        auto is_infinity = _mm_fpclass_ps_mask(decay(v), 0x10 | 0x08);
        auto is_non_zero = _mm_cmp_ps_mask(decay(v), _mm_setzero_ps(), _CMP_NEQ_UQ);

        auto exponents = _mm_getexp_ps(decay(v));
        exponents = _mm_add_ps(exponents, _mm_set1_ps(1.0f));
        *exp = _mm_maskz_cvttps_epi32(is_non_zero, exponents);

        auto ret = _mm_getmant_ps(decay(v), _MM_MANT_NORM_p5_1, _MM_MANT_SIGN_src);
        // Note: Returns -1 or 1 for -infinity and +infinity respectively

        ret = _mm_maskz_mov_ps(is_non_zero, ret);
        ret = _mm_mask_blend_ps(is_infinity, ret, decay(v));
        return vec4x32f{ret};

        #elif defined(AVEL_AVX512VL)
        auto is_infinity = _mm_cmp_ps_mask(decay(abs(v)), _mm_set1_ps(INFINITY), _CMP_EQ_OQ);
        auto is_non_zero = _mm_cmp_ps_mask(decay(v), _mm_setzero_ps(), _CMP_NEQ_UQ);

        auto exponents = _mm_getexp_ps(decay(v));
        exponents = _mm_add_ps(exponents, _mm_set1_ps(1.0f));
        *exp = _mm_maskz_cvttps_epi32(is_non_zero, exponents);

        auto ret = _mm_getmant_ps(decay(v), _MM_MANT_NORM_p5_1, _MM_MANT_SIGN_src);
        // Note: Returns -1 or 1 for -infinity and +infinity respectively

        ret = _mm_maskz_mov_ps(is_non_zero, ret);
        ret = _mm_mask_blend_ps(is_infinity, ret, decay(v));
        return vec4x32f{ret};


        #elif defined(AVEL_SSE2)
        auto v_bits = _mm_castps_si128(decay(v));

        auto is_v_zero = _mm_cmpeq_epi32(v_bits, _mm_setzero_si128());

        // Check if v is subnormal
        auto abs_mask = _mm_set1_epi32(float_sign_bit_mask_bits);
        auto flt_min_bits = _mm_set1_epi32(0x800000);
        auto is_subnormal = _mm_cmplt_epi32(_mm_andnot_si128(abs_mask, v_bits), flt_min_bits);

        // The first constant is the bit pattern for 1.0f
        // When the following two constants are reinterpreted as float, their sum is 2^24
        auto one_bits = _mm_set1_epi32(0x3f800000);
        auto offset_bits = _mm_set1_epi32(0xC000000);

        // Make v normal if it's subnormal
        auto multiplier_bits = _mm_add_epi32(one_bits, _mm_and_si128(is_subnormal, offset_bits));
        auto multiplier = _mm_castsi128_ps(multiplier_bits);
        auto v_corrected = _mm_mul_ps(decay(v), multiplier);
        auto v_corrected_bits = _mm_castps_si128(v_corrected);

        // Compute exponent to write out
        auto biased_exponent = _mm_srli_epi32(_mm_and_si128(v_corrected_bits, _mm_set1_epi32(float_exponent_mask_bits)), 23);
        auto bias_correction = _mm_add_epi32(_mm_set1_epi32(126), _mm_and_si128(is_subnormal, _mm_set1_epi32(24)));
        auto unbiased_exponent = _mm_sub_epi32(biased_exponent, bias_correction);

        // Clear out exponent value when the input is zero
        *exp = _mm_andnot_si128(is_v_zero, unbiased_exponent);

        // Change exponent to be -1, thereby remapping to [0.5, 1.0) range
        auto new_exponent = _mm_set1_epi32(126 << 23);
        auto cleared_exponent = _mm_andnot_si128(_mm_set1_epi32(float_exponent_mask_bits), v_corrected_bits);
        auto remapped_significand = _mm_or_si128(cleared_exponent, new_exponent);

        // Replace output value with original if output should just be self
        auto is_output_self = _mm_or_si128(is_v_zero, _mm_cmpeq_epi32(biased_exponent, _mm_set1_epi32(0xff)));
        auto ret = blend(mask4x32i{is_output_self}, vec4x32i{v_bits}, vec4x32i{remapped_significand});

        return vec4x32f{_mm_castsi128_ps(decay(ret))};

        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        //TODO: Handling of subnormals not necessary on ARMv7

        auto v_bits = vreinterpretq_s32_f32(decay(v));

        auto is_v_zero = vreinterpretq_s32_u32(vceqq_s32(v_bits, vdupq_n_s32(0x00)));

        // Check if v is subnormal
        auto abs_mask = vdupq_n_s32(float_sign_bit_mask_bits);
        auto flt_min_bits = vdupq_n_s32(0x800000);
        auto is_subnormal = vreinterpretq_s32_u32(vcltq_s32(vbicq_s32(v_bits, abs_mask), flt_min_bits));

        // The first constant is the bit pattern for 1.0f
        // When the following two constants are reinterpreted as float, their sum is 2^24
        auto one_bits = vdupq_n_s32(0x3f800000);
        auto offset_bits = vdupq_n_s32(0xC000000);

        // Make v normal if it's subnormal
        auto multiplier_bits = vaddq_s32(one_bits, vandq_s32(is_subnormal, offset_bits));
        auto multiplier = vreinterpretq_f32_s32(multiplier_bits);
        auto v_corrected = vmulq_f32(decay(v), multiplier);
        auto v_corrected_bits = vreinterpretq_s32_f32(v_corrected);

        // Compute exponent to write out
        auto biased_exponent = vshrq_n_s32(vandq_s32(v_corrected_bits, vdupq_n_s32(float_exponent_mask_bits)), 23);
        auto bias_correction = vaddq_s32(vdupq_n_s32(126), vandq_s32(is_subnormal, vdupq_n_s32(24)));
        auto unbiased_exponent = vsubq_s32(biased_exponent, bias_correction);

        // Clear out exponent value when the input is zero
        *exp = 	vbicq_s32(unbiased_exponent, is_v_zero);

        // Change exponent to be -1, thereby remapping to [0.5, 1.0) range
        auto new_exponent = vdupq_n_s32(126 << 23);
        auto cleared_exponent = vbicq_s32(v_corrected_bits, vdupq_n_s32(float_exponent_mask_bits));
        auto remapped_significand = vorrq_s32(cleared_exponent, new_exponent);

        // Replace output value with original if output should just be self
        auto is_output_self = vorrq_s32(is_v_zero, vreinterpretq_s32_u32(vceqq_s32(biased_exponent, vdupq_n_s32(0xff))));
        auto ret = blend(mask4x32i{vreinterpretq_u32_s32(is_output_self)}, vec4x32i{v_bits}, vec4x32i{remapped_significand});

        return vec4x32f{vreinterpretq_f32_s32(decay(ret))};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32f ldexp(vec4x32f arg, vec4x32i exp) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return vec4x32f{_mm_scalef_ps(decay(arg), _mm_cvtepi32_ps(decay(exp)))};

        #elif defined(AVEL_SSE2)
        //TODO: Optimize

        // Approach based on repeated multiplication by powers of two.
        auto bias = _mm_set1_epi32(127);

        auto bits = _mm_castps_si128(decay(arg));
        auto exponent_field = _mm_and_si128(_mm_set1_epi32(float_exponent_mask_bits), bits);
        vec4x32i arg_exponent{_mm_srli_epi32(exponent_field, 23)};

        // Perform two multiplications such that they should never lead to lossy rounding
        vec4x32i lower_bound0{vec4x32i{1} - arg_exponent};
        vec4x32i upper_bound0{vec4x32i{254} - arg_exponent};

        vec4x32i extracted_magnitude = clamp(exp, lower_bound0, upper_bound0);
        exp -= extracted_magnitude;

        auto exponent0 = bit_shift_right<1>(extracted_magnitude);
        auto exponent_field0 = _mm_slli_epi32(_mm_add_epi32(decay(exponent0), bias), 23);
        auto multiplicand0 = _mm_castsi128_ps(exponent_field0);

        auto exponent1 = extracted_magnitude - exponent0;
        auto exponent_field1 = _mm_slli_epi32(_mm_add_epi32(decay(exponent1), bias), 23);
        auto multiplicand1 = _mm_castsi128_ps(exponent_field1);

        // Perform one more multiplication in case the previous two weren't enough
        // If the number isn't enough then the program

        vec4x32i lower_bound1{-126};
        vec4x32i upper_bound1{+126};

        auto exponent2 = avel::clamp(exp, lower_bound1, upper_bound1);
        auto exponent_field2 = _mm_slli_epi32(_mm_add_epi32(decay(exponent2), bias), 23);
        auto multiplicand2 = _mm_castsi128_ps(exponent_field2);
        //exp -= exponent2;

        auto t0 = _mm_mul_ps(decay(arg), multiplicand0);
        auto t1 = _mm_mul_ps(t0, multiplicand1);
        auto ret = _mm_mul_ps(t1, multiplicand2);

        return vec4x32f{ret};

        #endif

        #if defined(AVEL_NEON)
        //TODO: Optimize
        //TODO: Handling of denormals not needed on ARMv7

        // Approach based on repeated multiplication by powers of two.
        auto bias = vdupq_n_s32(127);

        auto bits = vreinterpretq_s32_f32(decay(arg));
        auto exponent_field = vandq_s32(vdupq_n_s32(float_exponent_mask_bits), bits);
        vec4x32i arg_exponent{vshrq_n_s32(exponent_field, 23)};

        // Perform two multiplications such that they should never lead to lossy rounding
        vec4x32i lower_bound0{vec4x32i{1} - arg_exponent};
        vec4x32i upper_bound0{vec4x32i{254} - arg_exponent};

        vec4x32i extracted_magnitude = clamp(exp, lower_bound0, upper_bound0);
        exp -= extracted_magnitude;

        auto exponent0 = bit_shift_right<1>(extracted_magnitude);
        auto exponent_field0 = vshlq_n_s32(vaddq_s32(decay(exponent0), bias), 23);
        auto multiplicand0 = vreinterpretq_f32_s32(exponent_field0);

        auto exponent1 = extracted_magnitude - exponent0;
        auto exponent_field1 = vshlq_n_s32(vaddq_s32(decay(exponent1), bias), 23);
        auto multiplicand1 = vreinterpretq_f32_s32(exponent_field1);

        // Perform one more multiplication in case the previous two weren't enough
        // If the number isn't enough then the program

        vec4x32i lower_bound1{-126};
        vec4x32i upper_bound1{+126};

        auto exponent2 = avel::clamp(exp, lower_bound1, upper_bound1);
        auto exponent_field2 = vshlq_n_s32(vaddq_s32(decay(exponent2), bias), 23);
        auto multiplicand2 = vreinterpretq_f32_s32(exponent_field2);
        //exp -= exponent2;

        auto t0 = vmulq_f32(decay(arg), multiplicand0);
        auto t1 = vmulq_f32(t0, multiplicand1);
        auto ret = vmulq_f32(t1, multiplicand2);

        return vec4x32f{ret};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32f scalbn(vec4x32f x, vec4x32i exp) {
        return avel::ldexp(x, exp);
    }

    [[nodiscard]]
    AVEL_FINL vec4x32i ilogb(vec4x32f x) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        auto exp_fp = _mm_getexp_ps(decay(x));

        vec4x32f zero_ret{_mm_castsi128_ps(_mm_set1_epi32(FP_ILOGB0))};
        vec4x32f inf_ret {_mm_castsi128_ps(_mm_set1_epi32(INT_MAX))};
        vec4x32f nan_ret {_mm_castsi128_ps(_mm_set1_epi32(FP_ILOGBNAN))};

        // Return value when input is not edge case
        auto misc_ret_i = _mm_cvtps_epi32(exp_fp);
        misc_ret_i = _mm_maskz_mov_epi32(_mm_cmpneq_epi32_mask(misc_ret_i, _mm_set1_epi32(0x80000000)), misc_ret_i);

        vec4x32i zero_ret_i{_mm_castps_si128(_mm_fixupimm_ps(decay(zero_ret), exp_fp, _mm_set1_epi32(0x88808888), 0x00))};
        vec4x32i inf_ret_i {_mm_castps_si128(_mm_fixupimm_ps(decay(inf_ret),  exp_fp, _mm_set1_epi32(0x88088888), 0x00))};
        vec4x32i nan_ret_i {_mm_castps_si128(_mm_fixupimm_ps(decay(nan_ret),  exp_fp, _mm_set1_epi32(0x88888800), 0x00))};

        return (vec4x32i{misc_ret_i} | zero_ret_i) | (inf_ret_i | nan_ret_i);

        #elif defined(AVEL_SSE2)
        auto is_subnormal = avel::abs(x) < vec4x32f{FLT_MIN};
        auto multiplier = blend(is_subnormal, vec4x32f{16777216.0f}, vec4x32f{1.0f});

        auto x_corrected = x * multiplier;

        auto bits = avel::bit_cast<vec4x32i>(x_corrected);

        vec4x32i exponents = avel::bit_shift_right<23>(bits & vec4x32i{float_exponent_mask_bits});
        vec4x32i bias = blend(bit_cast<mask4x32i>(is_subnormal), vec4x32i{127 + 24}, vec4x32i{127});

        auto ret = exponents - bias;

        ret = blend(mask4x32i{_mm_castps_si128(decay(x == vec4x32f{0.0f}))}, vec4x32i{FP_ILOGB0}, ret);
        ret = blend(mask4x32i{_mm_castps_si128(decay(avel::isinf(x)))}, vec4x32i{INT_MAX}, ret);
        ret = blend(mask4x32i{_mm_castps_si128(decay(avel::isnan(x)))}, vec4x32i{FP_ILOGBNAN}, ret);

        return ret;

        #endif

        #if defined(AVEL_NEON)
        //TODO: No denormal handling necessary on ARMv7
        auto is_subnormal = avel::abs(x) < vec4x32f{FLT_MIN};
        auto multiplier = blend(is_subnormal, vec4x32f{16777216.0f}, vec4x32f{1.0f});

        auto x_corrected = x * multiplier;

        auto bits = avel::bit_cast<vec4x32i>(x_corrected);

        vec4x32i exponents = avel::bit_shift_right<23>(bits & vec4x32i{float_exponent_mask_bits});
        vec4x32i bias = blend(bit_cast<mask4x32i>(is_subnormal), vec4x32i{127 + 24}, vec4x32i{127});

        auto ret = exponents - bias;

        ret = blend(mask4x32i{decay(x == vec4x32f{0.0f})}, vec4x32i{FP_ILOGB0}, ret);
        ret = blend(mask4x32i{decay(avel::isinf(x))}, vec4x32i{INT_MAX}, ret);
        ret = blend(mask4x32i{decay(avel::isnan(x))}, vec4x32i{FP_ILOGBNAN}, ret);

        return ret;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32f logb(vec4x32f v) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return vec4x32f{_mm_getexp_ps(decay(v))};

        #elif defined(AVEL_SSE2)
        auto is_subnormal = avel::abs(v) < vec4x32f{FLT_MIN};
        auto multiplier = blend(is_subnormal, vec4x32f{16777216.0f}, vec4x32f{1.0f});

        auto v_corrected = v * multiplier;

        auto bits = avel::bit_cast<vec4x32i>(v_corrected);

        auto exponent_field = bits & vec4x32i{float_exponent_mask_bits};
        auto exponents_i = avel::bit_shift_right<23>(exponent_field);
        vec4x32f exponents{_mm_cvtepi32_ps(decay(exponents_i))};
        auto bias = blend(is_subnormal, vec4x32f{127 + 24}, vec4x32f{127});

        auto ret = exponents - bias;

        ret = blend(v == vec4x32f{0.0f}, vec4x32f{-INFINITY}, ret);
        ret = blend(avel::isinf(v), vec4x32f{INFINITY}, ret);
        ret = blend(avel::isnan(v), vec4x32f{NAN}, ret);

        return ret;
        #endif

        #if defined(AVEL_NEON)
        //TODO: No denormal handling necessary on ARMv7
        auto is_subnormal = avel::abs(v) < vec4x32f{FLT_MIN};
        auto multiplier = blend(is_subnormal, vec4x32f{16777216.0f}, vec4x32f{1.0f});

        auto v_corrected = v * multiplier;

        auto bits = avel::bit_cast<vec4x32i>(v_corrected);

        auto exponent_field = bits & vec4x32i{float_exponent_mask_bits};
        auto exponents_i = avel::bit_shift_right<23>(exponent_field);
        vec4x32f exponents{vcvtq_f32_s32(decay(exponents_i))};
        auto bias = blend(is_subnormal, vec4x32f{127 + 24}, vec4x32f{127});

        auto ret = exponents - bias;

        ret = blend(v == vec4x32f{0.0f}, vec4x32f{-INFINITY}, ret);
        ret = blend(avel::isinf(v), vec4x32f{INFINITY}, ret);
        ret = blend(avel::isnan(v), vec4x32f{NAN}, ret);

        return ret;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec4x32f copysign(vec4x32f mag, vec4x32f sign) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        auto mask = _mm_set1_epi32(float_sign_bit_mask_bits);
        auto ret = _mm_ternarylogic_epi32(_mm_castps_si128(decay(sign)), _mm_castps_si128(decay(mag)), mask, 0xe4);
        return vec4x32f{_mm_castsi128_ps(ret)};

        #elif defined(AVEL_SSE2)
        auto mask = _mm_set1_ps(float_sign_bit_mask);
        auto ret = _mm_or_ps(_mm_and_ps(mask, decay(sign)), _mm_andnot_ps(mask, decay(mag)));

        return vec4x32f{ret};
        #endif

        #if defined(AVEL_NEON)
        auto mask = vdupq_n_u32(float_sign_bit_mask_bits);
        auto ret = 	vbslq_f32(mask, decay(sign), decay(mag));

        return vec4x32f{ret};
        #endif
    }

    //=====================================================
    // Classification subroutines
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec4x32i fpclassify(vec4x32f v) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        // Approach based on testing of ranges of bit patterns to which the various categories belong

        auto v_bits = _mm_castps_si128(decay(v));

        // Take absolute value
        v_bits = _mm_and_si128(v_bits, _mm_set1_epi32(0x7fffffff));

        // Bit pattern for FLT_MIN
        const auto min_bits = _mm_set1_epi32(0x00800000);

        // Bit pattern for +INFINITY
        const auto inf_bits = _mm_set1_epi32(0x7f800000);

        // Masks for individual categories
        auto m0 = _mm_cmpeq_epi32(v_bits, _mm_setzero_si128());
        auto m1 = _mm_andnot_si128(m0, _mm_cmplt_epi32(v_bits, min_bits));
        auto m2 = _mm_andnot_si128(_mm_cmplt_epi32(v_bits, min_bits), _mm_cmplt_epi32(v_bits, inf_bits));
        auto m3 = _mm_cmpeq_epi32(v_bits, inf_bits);
        auto m4 = _mm_cmpgt_epi32(v_bits, inf_bits);

        // Select result via blending
        auto ret = _mm_setzero_si128();
        ret = _mm_and_si128(m0, _mm_set1_epi32(FP_ZERO));
        ret = _mm_ternarylogic_epi32(ret, m1, _mm_set1_epi32(FP_SUBNORMAL), 0xf8);
        ret = _mm_ternarylogic_epi32(ret, m2, _mm_set1_epi32(FP_NORMAL),    0xf8);
        ret = _mm_ternarylogic_epi32(ret, m3, _mm_set1_epi32(FP_INFINITE),  0xf8);
        ret = _mm_ternarylogic_epi32(ret, m4, _mm_set1_epi32(FP_NAN),       0xf8);

        return vec4x32i{ret};

        #elif defined(AVEL_SSE2)
        // Approach based on testing of ranges of bit patterns to which the various categories belong

        auto v_bits = _mm_castps_si128(decay(v));

        // Take absolute value
        v_bits = _mm_and_si128(v_bits, _mm_set1_epi32(0x7fffffff));

        // Bit pattern for FLT_MIN
        const auto min_bits = _mm_set1_epi32(0x00800000);

        // Bit pattern for +INFINITY
        const auto inf_bits = _mm_set1_epi32(0x7f800000);

        // Masks for individual categories
        auto m0 = _mm_cmpeq_epi32(v_bits, _mm_setzero_si128());
        auto m1 = _mm_andnot_si128(m0, _mm_cmplt_epi32(v_bits, min_bits));
        auto m2 = _mm_andnot_si128(_mm_cmplt_epi32(v_bits, min_bits), _mm_cmplt_epi32(v_bits, inf_bits));
        auto m3 = _mm_cmpeq_epi32(v_bits, inf_bits);
        auto m4 = _mm_cmpgt_epi32(v_bits, inf_bits);

        // Select result via blending
        auto ret = _mm_setzero_si128();
        ret = _mm_and_si128(m0, _mm_set1_epi32(FP_ZERO));
        ret = _mm_or_si128(ret, _mm_and_si128(m1, _mm_set1_epi32(FP_SUBNORMAL)));
        ret = _mm_or_si128(ret, _mm_and_si128(m2, _mm_set1_epi32(FP_NORMAL)));
        ret = _mm_or_si128(ret, _mm_and_si128(m3, _mm_set1_epi32(FP_INFINITE)));
        ret = _mm_or_si128(ret, _mm_and_si128(m4, _mm_set1_epi32(FP_NAN)));

        return vec4x32i{ret};
        #endif

        #if defined(AVEL_NEON)
        //TODO: Clean up and optimize
        const auto fp_infinite  = vdupq_n_s32(int(FP_INFINITE));
        const auto fp_nan       = vdupq_n_s32(int(FP_NAN));
        const auto fp_normal    = vdupq_n_s32(int(FP_NORMAL));
        const auto fp_subnormal = vdupq_n_s32(int(FP_SUBNORMAL));
        const auto fp_zero      = vdupq_n_s32(int(FP_ZERO));

        auto infinity = vdupq_n_f32(INFINITY);
        auto flt_min = vdupq_n_f32(FLT_MIN);

        auto zero_mask      = vreinterpretq_s32_u32(vcaleq_f32(decay(v), vdupq_n_f32(0.0f)));
        auto subnormal_mask = vreinterpretq_s32_u32(vbicq_u32(vcaltq_f32(decay(v), flt_min), vcaleq_f32(decay(v), vdupq_n_f32(0.0f))));
        auto infinite_mask  = vreinterpretq_s32_u32(vcaleq_f32(infinity, decay(v)));
        auto nan_mask       = vreinterpretq_s32_u32(vmvnq_u32(vceqq_f32(decay(v), decay(v))));
        auto normal_mask    = vreinterpretq_s32_u32(vandq_u32(vcaleq_f32(flt_min, decay(v)), vcaltq_f32(decay(v), infinity)));

        return vec4x32i{
            vorrq_s32(
                vandq_s32(infinite_mask, fp_infinite),
                vorrq_s32(
                    vorrq_s32(
                        vandq_s32(nan_mask, fp_nan),
                        vandq_s32(normal_mask, fp_normal)
                    ),
                    vorrq_s32(
                        vandq_s32(subnormal_mask, fp_subnormal),
                        vandq_s32(zero_mask, fp_zero)
                    )
                )
            )
        };

        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask4x32f isfinite(vec4x32f v) {
        #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)) || defined(AVEL_AVX10_1)
        return !mask4x32f{_mm_fpclass_ps_mask(decay(v), 0x01 | 0x08 | 0x10 | 0x80)};

        #elif defined(AVEL_SSE2)
        return (vec4x32f{-INFINITY} < v) && (v < vec4x32f{+INFINITY});

        #endif

        #if defined(AVEL_NEON)
        return mask4x32f{vcaltq_f32(decay(v), vdupq_n_f32(INFINITY))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask4x32f isinf(vec4x32f v) {
        #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)) || defined(AVEL_AVX10_1)
        return mask4x32f{_mm_fpclass_ps_mask(decay(v), 0x08 | 0x10)};

        #elif defined(AVEL_SSE2)
        return avel::abs(v) == vec4x32f{INFINITY};

        #endif

        #if defined(AVEL_NEON)
        return mask4x32f{vcageq_f32(decay(v), vdupq_n_f32(INFINITY))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask4x32f isnan(vec4x32f v) {
        #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)) || defined(AVEL_AVX10_1)
        return mask4x32f{_mm_fpclass_ps_mask(decay(v), 0x80 | 0x01)};

        #elif defined(AVEL_AVX512VL)
        return mask4x32f{_mm_cmp_ps_mask(decay(v), decay(v), _CMP_UNORD_Q)};

        #elif defined(AVEL_SSE2)
        return mask4x32f{_mm_cmpunord_ps(decay(v), decay(v))};

        #endif

        #if defined(AVEL_NEON)
        return v != v;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask4x32f isnormal(vec4x32f v) {
        #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)) || defined(AVEL_AVX10_1)
        return !mask4x32f{_mm_fpclass_ps_mask(decay(v), 0xBF)};

        #elif defined(AVEL_SSE2)
        auto abs_v = avel::abs(v);
        return (vec4x32f{FLT_MIN} <= abs_v) && (abs_v <= vec4x32f{FLT_MAX});

        #endif

        #if defined(AVEL_NEON)
        auto is_not_denormal = vcaleq_f32(vdupq_n_f32(FLT_MIN), decay(v));
        auto is_less_than_infinity = vcaltq_f32(decay(v), vdupq_n_f32(INFINITY));
        return mask4x32f{vandq_u32(is_not_denormal, is_less_than_infinity)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask4x32f signbit(vec4x32f arg) {
        #if (defined(AVEL_AVX512VL) && defined(AVEL_AVX512DQ)) || defined(AVEL_AVX10_1)
        return mask4x32f{_mm_fpclass_ps_mask(decay(arg), 0x40 | 0x04 | 0x10)};

        #elif defined(AVEL_AVX512VL)
        return mask4x32f{_mm_cmplt_epi32_mask(_mm_castps_si128(decay(arg)), _mm_setzero_si128())};

        #elif defined(AVEL_SSE2)
        auto bits = _mm_castps_si128(decay(arg));
        auto mask_bits = _mm_srai_epi32(bits, 31);
        return mask4x32f{_mm_castsi128_ps(mask_bits)};
        #endif

        #if defined(AVEL_NEON)
        auto t0 = vreinterpretq_s32_f32(decay(arg));
        auto t1 = vshrq_n_s32(t0, 31);
        return mask4x32f{vreinterpretq_u32_s32(t1)};
        #endif
    }

    //=====================================================
    // Comparison subroutines
    //=====================================================

    [[nodiscard]]
    AVEL_FINL mask4x32f isgreater(vec4x32f x, vec4x32f y) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return mask4x32f{_mm_cmp_ps_mask(decay(x), decay(y), _CMP_GT_OQ)};

        #elif defined(AVEL_AVX2)
        return mask4x32f{_mm_cmp_ps(decay(x), decay(y), _CMP_GT_OQ)};

        #elif defined(AVEL_SSE2)
        auto state = _MM_GET_EXCEPTION_STATE();
        auto ret = x > y;
        _MM_SET_EXCEPTION_STATE(state);
        return ret;

        #endif

        #if defined(AVEL_NEON)
        //TODO: Make quiet
        return x > y;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask4x32f isgreaterequal(vec4x32f x, vec4x32f y) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return mask4x32f{_mm_cmp_ps_mask(decay(x), decay(y), _CMP_GE_OQ)};

        #elif defined(AVEL_AVX2)
        return mask4x32f{_mm_cmp_ps(decay(x), decay(y), _CMP_GE_OQ)};

        #elif defined(AVEL_SSE2)
        auto state = _MM_GET_EXCEPTION_STATE();
        auto ret = x >= y;
        _MM_SET_EXCEPTION_STATE(state);
        return ret;

        #endif

        #if defined(AVEL_NEON)
        //TODO: Make quiet
        return x >= y;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask4x32f isless(vec4x32f x, vec4x32f y) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return mask4x32f{_mm_cmp_ps_mask(decay(x), decay(y), _CMP_LT_OQ)};

        #elif defined(AVEL_AVX2)
        return mask4x32f{_mm_cmp_ps(decay(x), decay(y), _CMP_LT_OQ)};

        #elif defined(AVEL_SSE2)
        auto state = _MM_GET_EXCEPTION_STATE();
        auto ret = x < y;
        _MM_SET_EXCEPTION_STATE(state);
        return ret;

        #endif

        #if defined(AVEL_NEON)
        //TODO: Make quiet
        return x < y;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask4x32f islessequal(vec4x32f x, vec4x32f y) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return mask4x32f{_mm_cmp_ps_mask(decay(x), decay(y), _CMP_LE_OQ)};

        #elif defined(AVEL_AVX2)
        return mask4x32f{_mm_cmp_ps(decay(x), decay(y), _CMP_LE_OQ)};

        #elif defined(AVEL_SSE2)
        auto state = _MM_GET_EXCEPTION_STATE();
        auto ret = x <= y;
        _MM_SET_EXCEPTION_STATE(state);
        return ret;

        #endif

        #if defined(AVEL_NEON)
        //TODO: Make quiet
        return x <= y;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask4x32f islessgreater(vec4x32f x, vec4x32f y) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return mask4x32f{_mm_cmp_ps_mask(decay(x), decay(y), _CMP_NEQ_OQ)};

        #elif defined(AVEL_AVX)
        return mask4x32f{_mm_cmp_ps(decay(x), decay(y), _CMP_NEQ_OQ)};

        #elif defined(AVEL_SSE2)
        auto state = _MM_GET_EXCEPTION_STATE();
        auto ret = (x < y) || (x > y);
        _MM_SET_EXCEPTION_STATE(state);

        return ret;

        #endif

        #if defined(AVEL_NEON)
        //TODO: Make quiet
        return x < y || x > y;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL mask4x32f isunordered(vec4x32f x, vec4x32f y) {
        #if defined(AVEL_AVX512VL) || defined(AVEL_AVX10_1)
        return mask4x32f{_mm_cmp_ps_mask(decay(x), decay(y), _CMP_UNORD_Q)};

        #elif defined(AVEL_AVX)
        return mask4x32f{_mm_cmp_ps(decay(x), decay(y), _CMP_UNORD_Q)};

        #elif defined(AVEL_SSE2)
        auto state = _MM_GET_EXCEPTION_STATE();
        return mask4x32f{_mm_cmpunord_ps(decay(x), decay(y))};
        _MM_SET_EXCEPTION_STATE(state);

        #endif

        #if defined(AVEL_NEON)
        //TODO: Make quiet
        auto a = vceqq_f32(decay(x), decay(x));
        auto b = vceqq_f32(decay(y), decay(y));
        return !mask4x32f{vandq_u32(a, b)};
        #endif
    }

}

#endif //AVEL_VEC4X32F_HPP
