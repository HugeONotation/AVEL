#ifndef AVEL_VEC8X16I_HPP
#define AVEL_VEC8X16I_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec8x16i = Vector<std::int16_t, 8>;
    using arr8x16i = std::array<std::int16_t, 8>;
    using mask8x16i = Vector_mask<std::int16_t, 8>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec8x16i> div(vec8x16i numerator, vec8x16i denominator);
    vec8x16i broadcast_mask(mask8x16i m);
    vec8x16i blend(mask8x16i m, vec8x16i a, vec8x16i b);





    template<>
    class Vector_mask<std::int16_t, 8> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 8;

        //=================================================
        // Type aliases
        //=================================================

        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        using primitive = __mmask8;
        #elif defined(AVEL_SSE2)
        using primitive = __m128i;
        #endif

        #if defined(AVEL_NEON)
        using primitive = uint16x8_t;
        #endif

    private:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

    public:

        //=================================================
        // -ctors
        //=================================================

        template<class U>
        AVEL_FINL explicit Vector_mask(Vector_mask<U, width> m):
            Vector_mask(convert<Vector_mask>(m)[0]) {}

        AVEL_FINL explicit Vector_mask(primitive p):
            content(p) {}

        AVEL_FINL explicit Vector_mask(bool b):
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content(b ? 0xFF : 0x00) {}
        #elif defined(AVEL_SSE2)
            content(b ? _mm_set1_epi16(-1) : _mm_setzero_si128()) {}
        #endif
        #if defined(AVEL_NEON)
            content(vmovq_n_u16(b ? -1 : 0)) {}
        #endif

        AVEL_FINL explicit Vector_mask(const arr8xb& arr) {
            static_assert(
                sizeof(bool) == 1,
                "Implementation assumes bools occupy a single byte"
            );

            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto array_data = _mm_loadu_si64(arr.data());
            content = static_cast<primitive>(_mm_cmplt_epi8_mask(_mm_setzero_si128(), array_data));

            #elif defined(AVEL_SSE2)
            primitive array_data = _mm_loadu_si64(arr.data());

            array_data = _mm_unpacklo_epi8(array_data, array_data);
            content = _mm_cmplt_epi16(_mm_setzero_si128(), array_data);

            #endif

            #if defined(AVEL_NEON)
            auto array_data = vld1_u8(bit_cast<const std::uint8_t*>(arr.data()));
            auto mask = vclt_u8(vdup_n_u8(0x00), array_data);
            auto widened_mask = vcombine_u8(mask, mask);
            content = vreinterpretq_u16_u8(vzipq_u8(widened_mask, widened_mask).val[0]);
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
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content = b ? 0xFFFF : 0x00;
            #elif defined(AVEL_SSE2)
            content = b ? _mm_set1_epi16(-1) : _mm_setzero_si128();
            #endif

            #if defined(AVEL_NEON)
            content = vdupq_n_u16(b ? -1 : 0);
            #endif

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
        AVEL_FINL friend bool operator==(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return _mm512_mask2int(lhs.content) == _mm512_mask2int(rhs.content);

            #elif defined(AVEL_SSE2)
            return _mm_movemask_epi8(decay(lhs)) == _mm_movemask_epi8(decay(rhs));

            #endif

            #if defined(AVEL_AARCH64)
            auto min = vminvq_u16(vceqq_u16(decay(lhs), decay(rhs)));
            return min == 0xFFFF;

            #elif defined(AVEL_NEON)
            return !(lhs != rhs);

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return _mm512_mask2int(lhs.content) != _mm512_mask2int(rhs.content);

            #elif defined(AVEL_SSE2)
            return _mm_movemask_epi8(decay(lhs)) != _mm_movemask_epi8(decay(rhs));

            #endif

            #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
            auto min = vminvq_u8(vreinterpretq_u8_u16(vceqq_u16(decay(lhs), decay(rhs))));
            return min == 0x00;

            #elif defined(AVEL_NEON)
            auto diff = vsubq_u64(vreinterpretq_u64_u16(decay(lhs)), vreinterpretq_u64_u16(decay(rhs)));
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
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content = _kand_mask16(content, rhs.content);

            #elif defined(AVEL_SSE2)
            content = _mm_and_si128(content, rhs.content);

            #endif

            #if defined(AVEL_NEON)
            content = vandq_u16(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content = _kor_mask16(content, rhs.content);

            #elif defined(AVEL_SSE2)
            content = _mm_or_si128(content, rhs.content);

            #endif

            #if defined(AVEL_NEON)
            content = vorrq_u16(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content = _kand_mask16(_kxor_mask16(content, rhs.content), _cvtu32_mask16(0xFF));

            #elif defined(AVEL_SSE2)
            content = _mm_xor_si128(content, rhs.content);

            #endif

            #if defined(AVEL_NEON)
            content = veorq_u16(content, decay(rhs));
            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector_mask operator!() const {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return Vector_mask{primitive(_knot_mask16(content))};

            #elif defined(AVEL_AVX512VL)
            return Vector_mask{_mm_ternarylogic_epi32(content, content, content, 0x01)};

            #elif defined(AVEL_SSE2)
            primitive t = _mm_undefined_si128();
            return Vector_mask{_mm_andnot_si128(content, _mm_cmpeq_epi16(t, t))};

            #endif

            #if defined(AVEL_NEON)
            return Vector_mask{vmvnq_u16(content)};
            #endif
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

        [[nodiscard]]
        AVEL_FINL explicit operator primitive() const {
            return content;
        }

    };

    //=====================================================
    // Mask functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(mask8x16i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return popcount(decay(m));
        #elif defined(AVEL_SSE2)
        return popcount(_mm_movemask_epi8(decay(m))) / 2;
        #endif

        #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
        auto t0 = vnegq_s16(vreinterpretq_s16_u16(decay(m)));
        return static_cast<std::uint32_t>(vaddvq_s16(t0));

        #elif defined(AVEL_NEON)
        auto t0 = vreinterpretq_u16_s16(vnegq_s16(vreinterpretq_s16_u16(decay(m))));
        auto t1 = vpadd_u16(vget_low_u16(t0), vget_high_u16(t0));
        auto t2 = vpadd_u16(t1, t1);
        auto t3 = vpadd_u16(t2, t2);

        return static_cast<std::uint32_t>(vget_lane_u16(t3, 0));

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask8x16i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return decay(m);
        #elif defined(AVEL_SSE2)
        return _mm_movemask_epi8(decay(m));
        #endif

        #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
        return vmaxvq_u16(decay(m)) != 0x00;

        #elif defined(AVEL_NEON)
        auto t0 = vreinterpretq_u32_u16(decay(m));
        auto t1 = vpmax_u32(vget_low_u32(t0), vget_high_u32(t0));
        auto t2 = vpmax_u32(t1, t1);
        return vget_lane_u32(t2, 0x0) != 0x00;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask8x16i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return _mm512_mask2int(decay(m)) == 0x00FF;
        #elif defined(AVEL_SSE2)
        auto tmp = _mm_movemask_epi8(decay(m));
        return 0xFFFF == tmp;
        #endif

        #if defined(AVEL_NEON) && defined(AVEL_AARCH64)
        return vminvq_u8(vreinterpretq_u8_u16(decay(m))) == 0xFF;

        #elif defined(AVEL_NEON)
        auto t0 = vreinterpretq_u32_u16(decay(m));
        auto t1 = vpmin_u32(vget_low_u32(t0), vget_high_u32(t0));
        auto t2 = vpmin_u32(t1, t1);
        return vget_lane_u32(t2, 0x0) == 0xFFFFFFFF;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask8x16i m) {
        return !any(m);
    }

    //=====================================================
    // Mask conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask8x16i, 1> convert<mask8x16i, mask8x16i>(mask8x16i m) {
        return std::array<mask8x16i, 1>{m};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask8x16u, 1> convert<mask8x16u, mask8x16i>(mask8x16i m) {
        return {mask8x16u{mask8x16u::primitive(decay(m))}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask8x16i, 1> convert<mask8x16i, mask8x16u>(mask8x16u m) {
        return {mask8x16i{mask8x16i::primitive(decay(m))}};
    }






    template<>
    class alignas(16) Vector<std::int16_t, 8> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 8;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::int16_t;

        #if defined(AVEL_SSE2)
        using primitive = __m128i;
        #endif

        #if defined(AVEL_NEON)
        using primitive = int16x8_t;
        #endif

        using mask = Vector_mask<scalar, width>;

        template<class U>
        using rebind_type = Vector<U, width>;

        template<std::uint32_t M>
        using rebind_width = Vector<scalar, M>;

        //=================================================
        // Instance members
        //=================================================

    private:

        primitive content;

    public:

        //=================================================
        // -ctors
        //=================================================

        template<class U>
        AVEL_FINL explicit Vector(Vector<U, width> x):
            Vector(convert<Vector>(x)[0]) {}

        AVEL_FINL explicit Vector(mask m):
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content(_mm_sub_epi16(_mm_setzero_si128(), _mm_movm_epi16(decay(m)))) {}
        #elif defined(AVEL_SSE2)
            content(_mm_sub_epi16(_mm_setzero_si128(), decay(m))) {}
        #endif
        #if defined(AVEL_NEON)
            content(vnegq_s16(vreinterpretq_s16_u16(decay(m)))) {}
        #endif

        AVEL_FINL explicit Vector(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector(scalar x):
        #if defined(AVEL_SSE2)
            content(_mm_set1_epi16(x)) {}
        #endif
        #if defined(AVEL_NEON)
            content(vdupq_n_s16(x)) {}
        #endif

        AVEL_FINL explicit Vector(const arr8x16i& array) {
            #if defined(AVEL_SSE2)
            content = _mm_loadu_si128(reinterpret_cast<const __m128i*>(array.data()));
            #endif

            #if defined(AVEL_NEON)
            content = vld1q_s16(array.data());
            #endif
        }

        Vector() = default;
        Vector(const Vector&) = default;
        Vector(Vector&&) = default;
        ~Vector() = default;

        //=================================================
        // Assignment operators
        //=================================================

        AVEL_FINL Vector& operator=(scalar x) {
            #if defined(AVEL_SSE2)
            content = _mm_set1_epi16(x);
            #endif

            #if defined(AVEL_NEON)
            content = vdupq_n_s16(x);
            #endif

            return *this;
        }

        AVEL_FINL Vector& operator=(primitive p) {
            content = p;
            return *this;
        }

        Vector& operator=(const Vector&) = default;
        Vector& operator=(Vector&&) noexcept = default;

        //=================================================
        // Comparison operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend mask operator==(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmpeq_epi16_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return mask{_mm_cmpeq_epi16(lhs.content, rhs.content)};
            #endif

            #if defined(AVEL_NEON)
            return mask{vceqq_s16(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return !mask{_mm_cmpeq_epi16_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return !mask{_mm_cmpeq_epi16(lhs.content, rhs.content)};
            #endif

            #if defined(AVEL_NEON)
            return !(lhs == rhs);
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmplt_epi16_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return mask{_mm_cmplt_epi16(lhs.content, rhs.content)};
            #endif

            #if defined(AVEL_NEON)
            return mask{vcltq_s16(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{mask::primitive(_mm_cmple_epi16_mask(lhs.content, rhs.content))};
            #elif defined(AVEL_SSE2)
            return !mask{_mm_cmpgt_epi16(lhs.content, rhs.content)};
            #endif

            #if defined(AVEL_NEON)
            return mask{vcleq_s16(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmpgt_epi16_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return mask{_mm_cmpgt_epi16(lhs.content, rhs.content)};
            #endif

            #if defined(AVEL_NEON)
            return mask{vcgtq_s16(decay(lhs), decay(rhs))};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmpge_epi16_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return !mask{_mm_cmplt_epi16(lhs.content, rhs.content)};
            #endif

            #if defined(AVEL_NEON)
            return mask{vcgeq_s16(decay(lhs), decay(rhs))};
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
            return Vector{} - *this;
        }

        //=================================================
        // Arithmetic assignment operators
        //=================================================

        AVEL_FINL Vector& operator+=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_add_epi16(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vaddq_s16(content, decay(rhs));
            #endif

            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_sub_epi16(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vsubq_s16(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_mullo_epi16(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vmulq_s16(content, decay(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator/=(Vector rhs) {
            auto results = div(*this, rhs);
            content = results.quot.content;
            return *this;
        }

        AVEL_FINL Vector& operator%=(Vector rhs) {
            auto results = div(*this, rhs);
            content = results.rem.content;
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

        AVEL_FINL Vector& operator&=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_and_si128(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vandq_s16(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_or_si128(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vorrq_s16(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_xor_si128(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = veorq_s16(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(long long rhs) {
            #if defined(AVEL_SSE2)
            content  = _mm_sll_epi16(content, _mm_cvtsi64_si128(rhs));
            #endif

            #if defined(AVEL_NEON)
            content = vshlq_s16(content, vdupq_n_s16(rhs));

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(long long rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_sra_epi16(content, _mm_cvtsi64_si128(rhs));
            #endif

            #if defined(AVEL_NEON)
            content = vshlq_s16(content, vdupq_n_s16(-rhs));

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content = _mm_sllv_epi16(content, rhs.content);

            #elif defined(AVEL_AVX2)
            auto t0 = _mm256_cvtepu16_epi32(content);
            auto t1 = _mm256_cvtepu16_epi32(decay(rhs));

            auto t2 = _mm256_and_si256(_mm256_sllv_epi32(t0, t1), _mm256_set1_epi32(0x0000FFFF));

            auto lo = t2;
            auto hi = _mm256_permute2f128_si256(lo, lo, 0x01);
            content = _mm256_castsi256_si128(_mm256_packus_epi32(lo, hi));

            #elif defined(AVEL_SSE2)
            for (unsigned i = 0; i < 4; ++i) {
                auto t0 = _mm_and_si128(rhs.content, _mm_set1_epi16(1u << i));
                auto m = _mm_cmplt_epi16(primitive{}, t0);

                auto a = _mm_andnot_si128(m, content);
                auto b = _mm_and_si128(m, _mm_sll_epi16(content, _mm_cvtsi64_si128(1u << i)));
                content = _mm_or_si128(a, b);
            }

            auto m = _mm_cmplt_epi16(rhs.content, _mm_set1_epi16(16));
            content = _mm_and_si128(content, m);

            #endif

            #if defined(AVEL_NEON)
            content = vshlq_s16(content, decay(rhs));

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content = _mm_srav_epi16(content, rhs.content);

            #elif defined(AVEL_AVX2)
            auto t0 = _mm256_cvtepi16_epi32(content);
            auto t1 = _mm256_cvtepi16_epi32(decay(rhs));

            auto t2 = _mm256_and_si256(_mm256_srav_epi32(t0, t1), _mm256_set1_epi32(0x0000FFFF));

            auto lo = t2;
            auto hi = _mm256_permute2f128_si256(lo, lo, 0x01);
            content = _mm256_castsi256_si128(_mm256_packus_epi32(lo, hi));

            #elif defined(AVEL_SSE41)
            auto threshold = _mm_set1_epi16(8);

            for (unsigned i = 4; i-- > 0; ) {
                auto m = _mm_cmplt_epi16(decay(rhs), threshold);

                auto a = _mm_sra_epi16(content, _mm_cvtsi64_si128(1u << i));
                auto b = content;

                content = _mm_blendv_epi8(a, b, m);

                rhs = _mm_sub_epi16(decay(rhs), _mm_andnot_si128(m, threshold));
                threshold = _mm_srai_epi16(threshold, 0x1);
            }

            //TODO: Consider implementation using pshufb and 32-bit
            // multiplication

            #elif defined(AVEL_SSE2)
            auto zeros = _mm_setzero_si128();

            auto negative_mask = _mm_cmplt_epi16(content, zeros);
            content = _mm_xor_si128(content, negative_mask);

            auto non_zero_mask = _mm_cmplt_epi16(decay(rhs), _mm_set1_epi16(16));
            content = _mm_and_si128(content, non_zero_mask);

            auto c0 = _mm_cmplt_epi16(_mm_slli_epi16(decay(rhs), 12), zeros);
            content = _mm_max_epi16(_mm_andnot_si128(c0, content), _mm_srli_epi16(content, 8));

            auto c1 = _mm_cmplt_epi16(_mm_slli_epi16(decay(rhs), 13), zeros);
            content = _mm_max_epi16(_mm_andnot_si128(c1, content), _mm_srli_epi16(content, 4));

            auto c2 = _mm_cmplt_epi16(_mm_slli_epi16(decay(rhs), 14), zeros);
            content = _mm_max_epi16(_mm_andnot_si128(c2, content), _mm_srli_epi16(content, 2));

            auto c3 = _mm_cmplt_epi16(_mm_slli_epi16(decay(rhs), 15), zeros);
            content = _mm_max_epi16(_mm_andnot_si128(c3, content), _mm_srli_epi16(content, 1));


            content = _mm_xor_si128(content, negative_mask);

            #endif

            #if defined(AVEL_NEON)
            content = vshlq_s16(content, vnegq_s16(decay(rhs)));

            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator~() const {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return Vector{_mm_ternarylogic_epi32(content, content, content, 0x01)};

            #elif defined(AVEL_SSE2)
            auto undef = _mm_undefined_si128();
            return Vector{_mm_andnot_si128(content, _mm_cmpeq_epi32(undef, undef))};

            #endif

            #if defined(AVEL_NEON)
            return Vector{vmvnq_s16(content)};
            #endif
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
        AVEL_FINL friend Vector operator<<(Vector lhs, long long rhs) {
            lhs <<= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator>>(Vector lhs, long long rhs) {
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
        // Conversion operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL explicit operator primitive() const {
            return content;
        }

        [[nodiscard]]
        AVEL_FINL explicit operator mask() const {
            return Vector{} != *this;
        }

    };

    //=====================================================
    // Delayed definitions
    //=====================================================

    AVEL_FINL vec8x16i operator-(vec8x16u x) {
        return vec8x16i{} - vec8x16i{x};
    }

    //=====================================================
    // Arrangment operations
    //=====================================================

    template<std::uint32_t N>
    AVEL_FINL std::int16_t extract(vec8x16i v) {
        static_assert(N <= vec8x16i::width, "Specified index does not exist");
        typename std::enable_if<N <= vec8x16i::width, int>::type dummy_variable = 0;

        #if defined(AVEL_SS41)
        return _mm_extract_epi16(decay(v), N);

        #elif defined(AVEL_SSE2)
        return _mm_cvtsi128_si32(_mm_srli_si128(decay(v), 2 * N)) & 0xFFFF;

        #endif
    }

    //=====================================================
    // General vector operations
    //=====================================================

    /*
    [[nodiscard]]
    AVEL_FINL std::uint32_t count(vec8x16i x) {
        return count(vec8x16u{x});
    }

    [[nodiscard]]
    AVEL_FINL bool any(vec8x16i x) {
        return any(vec8x16u{x});
    }

    [[nodiscard]]
    AVEL_FINL bool all(vec8x16i x) {
        return all(vec8x16u{x});
    }

    [[nodiscard]]
    AVEL_FINL bool none(vec8x16i x) {
        return none(vec8x16u{x});
    }
    */

    [[nodiscard]]
    AVEL_FINL vec8x16i broadcast_mask(mask8x16i m) {
        return vec8x16i{broadcast_mask(mask8x16u{m})};
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i blend(mask8x16i m, vec8x16i a, vec8x16i b) {
        return vec8x16i{blend(mask8x16u{m}, vec8x16u{a}, vec8x16u{b})};
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i byteswap(vec8x16i x) {
        return vec8x16i{byteswap(vec8x16u(x))};
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i max(vec8x16i a, vec8x16i b) {
        #if defined(AVEL_SSE41)
        return vec8x16i{_mm_max_epi16(decay(a), decay(b))};

        #elif defined(AVEL_SSE2)
        return blend(a < b, b, a);

        #endif

        #if defined(AVEL_NEON)
        return vec8x16i{vmaxq_s16(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i min(vec8x16i a, vec8x16i b) {
        #if defined(AVEL_SSE41)
        return vec8x16i{_mm_min_epi16(decay(a), decay(b))};

        #elif defined(AVEL_SSE2)
        return blend(b < a, b, a);

        #endif

        #if defined(AVEL_NEON)
        return vec8x16i{vminq_s16(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec8x16i, 2> minmax(vec8x16i a, vec8x16i b) {
        #if defined(AVEL_SSE41)
        return std::array<vec8x16i, 2>{
            vec8x16i{_mm_min_epi16(decay(a), decay(b))},
            vec8x16i{_mm_max_epi16(decay(a), decay(b))}
        };

        #elif defined(AVEL_SSE2)
        auto m = b < a;
        return {blend(m, b, a), blend(m, a, b)};

        #endif

        #if defined(AVEL_NEON)
        return {
            vec8x16i{vminq_s16(decay(a), decay(b))},
            vec8x16i{vmaxq_s16(decay(a), decay(b))}
        };
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i clamp(vec8x16i x, vec8x16i lo, vec8x16i hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i average(vec8x16i x, vec8x16i y) {
        #if defined(AVEL_SSE2)
        auto avg = (x & y) + ((x ^ y) >> 1);
        auto c = broadcast_mask((x < -y) | (y == vec8x16i{std::int16_t(1 << 15)})) & (x ^ y) & vec8x16i{1};

        return avg + c;

        #endif

        #if defined(AVEL_NEON)
        auto avg = vec8x16i{vhaddq_s16(decay(x), decay(y))};
        auto c = broadcast_mask((x < -y) | (y == vec8x16i{std::int16_t(1 << 15)})) & (x ^ y) & vec8x16i{1};

        return avg + c;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i midpoint(vec8x16i a, vec8x16i b) {
        //TODO: Leverage newer instruction sets
        #if defined(AVEL_SSE2)
        auto offset = _mm_set1_epi16(0x8000);

        auto a_offset = _mm_xor_si128(decay(a), offset);
        auto b_offset = _mm_xor_si128(decay(b), offset);

        auto average_offset = _mm_avg_epu16(a_offset, b_offset);
        auto average = _mm_xor_si128(average_offset, offset);

        auto m = _mm_cmplt_epi16(decay(a), decay(b));
        auto bias = _mm_and_si128(_mm_xor_si128(decay(a), decay(b)), _mm_and_si128(m, _mm_set1_epi16(0x01)));

        return vec8x16i{_mm_sub_epi16(average, bias)};
        #endif

        #if defined(AVEL_NEON)
        vec8x16i t0 = vec8x16i{vhaddq_s16(decay(a), decay(b))};
        vec8x16i t1 = (a ^ b) & vec8x16i{0x1} & broadcast_mask(b < a);
        return t0 + t1;

        #endif
    };

    [[nodiscard]]
    AVEL_FINL vec8x16i negate(mask8x16i m, vec8x16i x) {
        auto mask = broadcast_mask(m);
        return (x ^ mask) - mask;
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i abs(vec8x16i x) {
        #if defined(AVEL_SSSE3)
        return vec8x16i{_mm_abs_epi16(decay(x))};

        #elif defined(AVEL_SSE2)
        auto y = x >> 15;
        return (x ^ y) - y;

        #endif

        #if defined(AVEL_NEON)
        auto zeros = vdupq_n_s16(0x00);
        return vec8x16i{vabdq_s16(decay(x), zeros)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i neg_abs(vec8x16i x) {
        #if defined(AVEL_SSSE3)
        return -vec8x16i{_mm_abs_epi16(decay(x))};

        #elif defined(AVEL_SSE2)
        auto y = ~(x >> 15);
        return (x ^ y) - y;

        #endif

        #if defined(AVEL_NEON)
        auto y = vmvnq_s16(vshrq_n_s16(decay(x), 15));
        return vec8x16i{vsubq_s16(veorq_s16(decay(x), y), y)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i neg_abs(vec8x16u x) {
        #if defined(AVEL_SSSE3)
        return -vec8x16i{_mm_abs_epi16(decay(x))};

        #elif defined(AVEL_SSE2)
        auto z = vec8x16i(x);
        auto y = ~z >> 15;
        return (z ^ y) - y;

        #endif

        #if defined(AVEL_NEON)
        auto y = vmvnq_s16(vshrq_n_s16(vreinterpretq_s16_u16(decay(x)), 15));
        return vec8x16i{vsubq_s16(veorq_s16(vreinterpretq_s16_u16(decay(x)), y), y)};
        #endif
    }

    //=====================================================
    // Load/Store operations
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x16i load<vec8x16i>(const std::int16_t* ptr) {
        #if defined(AVEL_SSE2)
        return vec8x16i{_mm_loadu_si128(reinterpret_cast<const __m128i*>(ptr))};
        #endif

        #if defined(AVEL_NEON)
        return vec8x16i{vld1q_s16(ptr)};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x16i aligned_load<vec8x16i>(const std::int16_t* ptr) {
        #if defined(AVEL_SSE2)
        return vec8x16i{_mm_load_si128(reinterpret_cast<const __m128i*>(ptr))};
        #endif

        #if defined(AVEL_NEON)
        return vec8x16i{vld1q_s16(ptr)};
        #endif
    }

    AVEL_FINL void store(std::int16_t* ptr, vec8x16i x) {
        #if defined(AVEL_SSE2)
        _mm_storeu_si128(reinterpret_cast<__m128i*>(ptr), decay(x));
        #endif

        #if defined(AVEL_NEON)
        vst1q_s16(ptr, decay(x));
        #endif
    }

    AVEL_FINL void aligned_store(std::int16_t* ptr, vec8x16i x) {
        #if defined(AVEL_SSE2)
        _mm_store_si128(reinterpret_cast<__m128i*>(ptr), decay(x));
        #endif

        #if defined(AVEL_NEON)
        vst1q_s16(ptr, decay(x));
        #endif
    }

    //=====================================================
    // Integer vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec8x16i> div(vec8x16i x, vec8x16i y) {
        vec8x16i quotient{};

        mask8x16i sign_mask0 = (x < vec8x16i{});
        mask8x16i sign_mask1 = (y < vec8x16i{});

        mask8x16i sign_mask2 = sign_mask0 ^ sign_mask1;

        vec8x16u numerator{abs(x)};
        vec8x16u denominator{abs(y)};

        //TODO: Compute i more appropriately?
        //TODO: Otherwise optimize
        std::int32_t i = 16;

        for (; (i-- > 0) && any(mask8x16u(numerator));) {
            mask8x16u b = ((numerator >> i) >= denominator);
            numerator -= (broadcast_mask(b) & (denominator << i));
            quotient |= vec8x16i{(vec8x16u{b} << i)};
        }

        //Adjust quotient's sign. Should be xor of operands' signs
        quotient = blend(sign_mask2, -quotient, quotient);

        //Adjust numerator's sign. Should be same sign as it was originally
        x = blend(sign_mask0, -vec8x16i{numerator}, vec8x16i{numerator});

        return {quotient, x};
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i popcount(vec8x16i x) {
        return vec8x16i{popcount(vec8x16u(x))};
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i countl_zero(vec8x16i x) {
        return vec8x16i{countl_zero(vec8x16u{x})};
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i countl_one(vec8x16i x) {
        return vec8x16i{countl_one(vec8x16u{x})};
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i countr_zero(vec8x16i x) {
        return vec8x16i{countr_zero(vec8x16u{x})};
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i countr_one(vec8x16i x) {
        return vec8x16i{countr_one(vec8x16u{x})};
    }

    [[nodiscard]]
    AVEL_FINL mask8x16i has_single_bit(vec8x16i x) {
        return mask8x16i{has_single_bit(vec8x16u(x))};
    }

    //=====================================================
    // Bit Manipulation Operations
    //=====================================================

    template<std::uint32_t S>
    [[nodiscard]]
    vec8x16i bit_shift_left(vec8x16i v) {
        static_assert(S <= 16, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 16, int>::type dummy_variable = 0;

        return vec8x16i{bit_shift_left<S>(vec8x16u{v})};
    }

    template<std::uint32_t S>
    vec8x16i bit_shift_right(vec8x16i v) {
        static_assert(S <= 16, "Cannot shift by more than scalar width");
        typename std::enable_if<S <= 16, int>::type dummy_variable = 0;

        #if defined(AVEL_SSE2)
        return vec8x16i{_mm_srai_epi16(decay(v), S)};
        #endif

        #if defined(AVEL_NEON)
        return vec8x16i{vshrq_n_s16(decay(v), S)};
        #endif
    }

    template<>
    vec8x16i bit_shift_right<0>(vec8x16i v) {
        return v;
    }



    [[nodiscard]]
    AVEL_FINL vec8x16i rotl(vec8x16i x, long long s) {
        return vec8x16i{rotl(vec8x16u{x}, s)};
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i rotl(vec8x16i x, vec8x16i s) {
        return vec8x16i{rotl(vec8x16u{x}, vec8x16u{s})};
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i rotr(vec8x16i x, long long s) {
        return vec8x16i{rotr(vec8x16u{x}, s)};
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i rotr(vec8x16i x, vec8x16i s) {
        return vec8x16i{rotr(vec8x16u{x}, vec8x16u{s})};
    }

    //=====================================================
    // Vector conversions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL arr8x16i to_array(vec8x16i x) {
        alignas(16) arr8x16i ret;
        aligned_store(ret.data(), x);
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec8x16i, 1> convert<vec8x16i, vec8x16i>(vec8x16i x) {
        return {x};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec8x16u, 1> convert<vec8x16u, vec8x16i>(vec8x16i x) {
        #if defined(AVEL_SSE2)
        return {vec8x16u{decay(x)}};
        #endif

        #if defined(AVEL_NEON)
        return {vec8x16u{vreinterpretq_u16_s16(decay(x))}};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec8x16i, 1> convert<vec8x16i, vec8x16u>(vec8x16u x) {
        #if defined(AVEL_SSE2)
        return {vec8x16i{decay(x)}};
        #endif

        #if defined(AVEL_NEON)
        return {vec8x16i{vreinterpretq_s16_u16(decay(x))}};
        #endif
    }

}

#endif //AVEL_VEC8x16I_HPP
