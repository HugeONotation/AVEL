#ifndef AVEL_VEC1X32U_HPP
#define AVEL_VEC1X32U_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec1x32u = Vector<std::uint32_t, 1>;

    using mask1x32u = Vector_mask<std::uint32_t, 1>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec1x32u> div(vec1x32u numerator, vec1x32u denominator);





    template<>
    class Vector_mask<std::uint32_t, 1> : public avel_impl::Vector_mask1xT {
        using base = avel_impl::Vector_mask1xT;
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 1;

        //=================================================
        // Type aliases
        //=================================================

        using primitive = base::primitive;

        //=================================================
        // -ctors
        //=================================================

        using base::base;

        template<class U>
        AVEL_FINL explicit Vector_mask(Vector_mask<U, width> v):
            base(decay(convert<Vector_mask>(v)[0])) {}

        Vector_mask() = default;
        Vector_mask(const Vector_mask&) = default;
        Vector_mask(Vector_mask&&) = default;
        ~Vector_mask() = default;

        //=================================================
        // Assignment operators
        //=================================================

        Vector_mask& operator=(const Vector_mask&) = default;
        Vector_mask& operator=(Vector_mask&&) = default;

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator&=(Vector_mask rhs) {
            base::operator&=(rhs);
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            base::operator|=(rhs);
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            base::operator^=(rhs);
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector_mask operator!() const {
            return Vector_mask{base::operator!()};
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator&(Vector_mask lhs, Vector_mask rhs) {
            lhs &= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator&&(Vector_mask lhs, Vector_mask rhs) {
            lhs &= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator|(Vector_mask lhs, Vector_mask rhs) {
            lhs |= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator||(Vector_mask lhs, Vector_mask rhs) {
            lhs |= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator^(Vector_mask lhs, Vector_mask rhs) {
            lhs ^= rhs;
            return lhs;
        }


    };

    //=====================================================
    // Mask functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(mask1x32u m) {
        return std::uint32_t(decay(m));
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask1x32u m) {
        return bool(decay(m));
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask1x32u m) {
        return bool(decay(m) & 0x1);
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask1x32u m) {
        return !all(m);
    }

    //=====================================================
    // Mask conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32u, 1> convert<mask1x32u, mask1x32u>(mask1x32u m) {
        return std::array<mask1x32u, 1>{m};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x8u, 1> convert<mask1x8u, mask1x32u>(mask1x32u m) {
        return std::array<mask1x8u, 1>{mask1x8u{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x8i, 1> convert<mask1x8i, mask1x32u>(mask1x32u m) {
        return std::array<mask1x8i, 1>{mask1x8i{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x16u, 1> convert<mask1x16u, mask1x32u>(mask1x32u m) {
        return std::array<mask1x16u, 1>{mask1x16u{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x16i, 1> convert<mask1x16i, mask1x32u>(mask1x32u m) {
        return std::array<mask1x16i, 1>{mask1x16i{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32u, 1> convert<mask1x32u, mask1x8u>(mask1x8u m) {
        return std::array<mask1x32u, 1>{mask1x32u{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32u, 1> convert<mask1x32u, mask1x8i>(mask1x8i m) {
        return std::array<mask1x32u, 1>{mask1x32u{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32u, 1> convert<mask1x32u, mask1x16u>(mask1x16u m) {
        return std::array<mask1x32u, 1>{mask1x32u{decay(m)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32u, 1> convert<mask1x32u, mask1x16i>(mask1x16i m) {
        return std::array<mask1x32u, 1>{mask1x32u{decay(m)}};
    }






    template<>
    class alignas(4) Vector<std::uint32_t, 1> {
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 1;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::uint32_t;

        using primitive = std::uint32_t;

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
        // -ctors
        //=================================================

        template<class U>
        AVEL_FINL explicit Vector(Vector<U, width> v):
            content(convert<Vector>(v)[0]) {}

        AVEL_FINL explicit Vector(mask m):
            content(decay(m)) {}

        //AVEL_FINL explicit Vector(primitive content):
        //    content(content) {}

        AVEL_FINL explicit Vector(scalar x):
            content(x) {}

        AVEL_FINL explicit Vector(const std::array<scalar, width>& array) {
            content = array[0];
        }

        Vector() = default;
        Vector(const Vector&) = default;
        Vector(Vector&&) = default;
        ~Vector() = default;

        //=================================================
        // Assignment operators
        //=================================================

        Vector& operator=(const Vector&) = default;
        Vector& operator=(Vector&&) noexcept = default;

        AVEL_FINL Vector& operator=(scalar x) {
            content = x;
            return *this;
        }

        //=================================================
        // Comparison operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend mask operator==(Vector lhs, Vector rhs) {
            return mask{lhs.content == rhs.content};
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            return mask{lhs.content != rhs.content};
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            return mask{lhs.content < rhs.content};
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            return mask{lhs.content <= rhs.content};
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            return mask{lhs.content > rhs.content};
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            return mask{lhs.content >= rhs.content};
        }

        //=================================================
        // Unary arithmetic operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator+() const {
            return *this;
        }

        //Definition of operator-() deferred until after definition of vec1x32i

        //=================================================
        // Arithmetic assignment operators
        //=================================================

        AVEL_FINL Vector& operator+=(Vector rhs) {
            content += rhs.content;
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            content -= rhs.content;
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            content *= rhs.content;
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
            *this += Vector{primitive(1)};
            return *this;
        }

        AVEL_FINL Vector operator++(int) {
            auto temp = *this;
            *this += Vector{primitive(1)};
            return temp;
        }

        AVEL_FINL Vector& operator--() {
            *this -= Vector{primitive(1)};
            return *this;
        }

        AVEL_FINL Vector operator--(int) {
            auto temp = *this;
            *this -= Vector{primitive(1)};
            return temp;
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector& operator&=(Vector rhs) {
            content &= rhs.content;
            return *this;
        }

        AVEL_FINL Vector& operator|=(Vector rhs) {
            content |= rhs.content;
            return *this;
        }

        AVEL_FINL Vector& operator^=(Vector rhs) {
            content ^= rhs.content;
            return *this;
        }

        AVEL_FINL Vector& operator<<=(long long rhs) {
            if (rhs > 31) {
                content = 0;
            } else {
                content <<= rhs;
            }
            return *this;
        }

        AVEL_FINL Vector& operator>>=(long long rhs) {
            if (rhs > 31) {
                content = 0;
            } else {
                content >>= rhs;
            }
            return *this;
        }

        AVEL_FINL Vector& operator<<=(Vector rhs) {
            if (rhs.content > 31) {
                content = 0;
            } else {
                content <<= rhs.content;
            }
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector rhs) {
            if (rhs.content > 31) {
                content = 0;
            } else {
                content >>= rhs.content;
            }
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator~() const {
            return Vector{primitive(~content)};
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
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec1x32u broadcast_bits(mask1x32u m) {
        return vec1x32u{broadcast_bits<std::uint32_t>(decay(m))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32u blend(vec1x32u a, vec1x32u b, mask1x32u m) {
        return vec1x32u{blend(decay(a), decay(b), decay(m))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32u max(vec1x32u a, vec1x32u b) {
        return vec1x32u{max(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32u min(vec1x32u a, vec1x32u b) {
        return vec1x32u{min(decay(a), decay(b))};
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec1x32u, 2> minmax(vec1x32u a, vec1x32u b) {
        if (decay(a) < decay(b)) {
            return {a, b};
        } else {
            return {b, a};
        }
    }

    [[nodiscard]]
    AVEL_FINL vec1x32u clamp(vec1x32u x, vec1x32u lo, vec1x32u hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec1x32u midpoint(vec1x32u a, vec1x32u b) {
        vec1x32u t0 = a & b & vec1x32u{0x1};
        vec1x32u t1 = (a | b) & vec1x32u{0x1} & broadcast_bits(a > b);
        vec1x32u t2 = t0 | t1;
        return (a >> 1) + (b >> 1) + t2;
    }

    [[nodiscard]]
    AVEL_FINL vec1x32u average(vec1x32u a, vec1x32u b) {
        return (a >> 1) + (b >> 1) + (a & b & vec1x32u{0x1});
    }

    //Definition of neg_abs delayed until vec1x32i is defined

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x32u load<vec1x32u>(const std::uint32_t* ptr) {
        return vec1x32u{*ptr};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec1x32u aligned_load<vec1x32u>(const std::uint32_t* ptr) {
        return vec1x32u{*ptr};
    }

    AVEL_FINL void store(std::uint32_t* ptr, vec1x32u v) {
        *ptr = decay(v);
    }

    AVEL_FINL void aligned_store(std::uint32_t* ptr, vec1x32u v) {
        *ptr = decay(v);
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec1x32u> div(vec1x32u numerator, vec1x32u denominator) {
        div_type<vec1x32u> ret;
        ret.quot = decay(numerator) / decay(denominator);
        ret.rem  = decay(numerator) % decay(denominator);
        return ret;
    }

    [[nodiscard]]
    AVEL_FINL vec1x32u popcount(vec1x32u v) {
        return vec1x32u{popcount(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32u countl_zero(vec1x32u v) {
        return vec1x32u{countl_zero(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32u countl_one(vec1x32u v) {
        return vec1x32u{countl_one(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32u countr_zero(vec1x32u v) {
        return vec1x32u{countr_zero(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32u countr_one(vec1x32u v) {
        return vec1x32u{countr_one(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32u bit_width(vec1x32u v) {
        return vec1x32u{bit_width(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32u bit_floor(vec1x32u v) {
        return vec1x32u{bit_floor(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32u bit_ceil(vec1x32u v) {
        return vec1x32u{bit_ceil(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL mask1x32u has_single_bit(vec1x32u v) {
        return mask1x32u{has_single_bit(decay(v))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32u rotl(vec1x32u v, long long s) {
        return vec1x32u{rotl(decay(v), s)};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32u rotl(vec1x32u v, vec1x32u s) {
        return vec1x32u{rotl(decay(v), decay(s))};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32u rotr(vec1x32u v, long long s) {
        return vec1x32u{rotr(decay(v), s)};
    }

    [[nodiscard]]
    AVEL_FINL vec1x32u rotr(vec1x32u v, vec1x32u s) {
        return vec1x32u{rotr(decay(v), decay(s))};
    }

    //=====================================================
    // Vector conversions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::array<std::uint32_t, 1> to_array(vec1x32u x) {
        alignas(4) std::array<std::uint32_t, 1> ret;
        aligned_store(ret.data(), x);
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32u, 1> convert<vec1x32u, vec1x32u>(vec1x32u x) {
        return std::array<vec1x32u, 1>{x};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x8u, 1> convert<vec1x8u, vec1x32u>(vec1x32u x) {
        return std::array<vec1x8u, 1>{vec1x8u{std::uint8_t(decay(x) & 0xFF)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x8i, 1> convert<vec1x8i, vec1x32u>(vec1x32u x) {
        return std::array<vec1x8i, 1>{vec1x8i{std::int8_t(decay(x) & 0xFF)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x16u, 1> convert<vec1x16u, vec1x32u>(vec1x32u x) {
        return std::array<vec1x16u, 1>{vec1x16u{std::uint16_t(decay(x) & 0xFFFF)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x16i, 1> convert<vec1x16i, vec1x32u>(vec1x32u x) {
        return std::array<vec1x16i, 1>{vec1x16i{std::int16_t(decay(x) & 0xFFFF)}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32u, 1> convert<vec1x32u, vec1x8u>(vec1x8u x) {
        return std::array<vec1x32u, 1>{vec1x32u{std::uint32_t(decay(x))}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32u, 1> convert<vec1x32u, vec1x8i>(vec1x8i x) {
        return std::array<vec1x32u, 1>{vec1x32u{std::uint32_t(decay(x))}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32u, 1> convert<vec1x32u, vec1x16u>(vec1x16u x) {
        return std::array<vec1x32u, 1>{vec1x32u{std::uint32_t(decay(x))}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32u, 1> convert<vec1x32u, vec1x16i>(vec1x16i x) {
        return std::array<vec1x32u, 1>{vec1x32u{std::uint32_t(decay(x))}};
    }

}

#endif //AVEL_VEC1x32U_HPP