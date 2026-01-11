#ifndef AVEL_SCALAR_CONVERSIONS_HPP
#define AVEL_SCALAR_CONVERSIONS_HPP

namespace avel {

    template<
        class T,
        class U,
        typename = typename std::enable_if<std::is_integral<T>::value>::type,
        typename = typename std::enable_if<std::is_integral<U>::value>::type,
        typename = typename std::enable_if<sizeof(T) <= sizeof(U)>::type
    >
    [[nodiscard]]
    AVEL_FINL T zero_extend(U u) {
        auto unsigned_u = static_cast<typename std::make_unsigned<U>::type>(u);
        auto zero_extended_result = static_cast<typename std::make_unsigned<T>::type>(unsigned_u);
        return static_cast<T>(zero_extended_result);
    }



    /*
    template<
        class T,
        class U,
        typename = typename std::enable_if<std::is_integral<T>::value>::type,
        typename = typename std::enable_if<std::is_integral<U>::value>::type,
        typename = typename std::enable_if<sizeof(T) <= sizeof(U)>::type
    >
    [[nodiscard]]
    AVEL_FINL T sign_extend(U u) {
        auto signed_u = static_cast<typename std::make_signed<U>::type>(u);
        auto sign_extended_result = static_cast<typename std::make_signed<T>::type>(signed_u);
        return static_cast<T>(sign_extended_result);
    }



    template<
        class T,
        class U,
        typename = typename std::enable_if<std::is_integral<T>::value>::type,
        typename = typename std::enable_if<std::is_integral<U>::value>::type,
        typename = typename std::enable_if<sizeof(T) <= sizeof(U)>::type
    >
    [[nodiscard]]
    AVEL_FINL T truncate(U u) {
        return static_cast<T>(u);
    }

    template<
        class T,
        class U,
        typename = typename std::enable_if<std::is_integral<T>::value>::type,
        typename = typename std::enable_if<std::is_integral<U>::value>::type,
        typename = typename std::enable_if<sizeof(T) <= sizeof(U)>::type
    >
    [[nodiscard]]
    AVEL_FINL T narrow(U u) {
        constexpr U max = std::numeric_limits<typename std::make_unsigned<T>::type>::max();

        if (u > max) {
            u = max;
        }

        return static_cast<T>(u);
    }



    template<
        class T,
        class U,
        typename = typename std::enable_if<std::is_floating_point<T>::value>::type,
        typename = typename std::enable_if<std::is_floating_point<U>::value>::type,
        typename = typename std::enable_if<sizeof(T) <= sizeof(U)>::type
    >
    [[nodiscard]]
    AVEL_FINL T floor_narrow(U u) {
        //TODO: Implement
    }

    template<
        class T,
        class U,
        typename = typename std::enable_if<std::is_floating_point<T>::value>::type,
        typename = typename std::enable_if<std::is_floating_point<U>::value>::type,
        typename = typename std::enable_if<sizeof(T) <= sizeof(U)>::type
    >
    [[nodiscard]]
    AVEL_FINL T ceil_narrow(U u) {
        //TODO: Implement
    }

    template<
        class T,
        class U,
        typename = typename std::enable_if<std::is_floating_point<T>::value>::type,
        typename = typename std::enable_if<std::is_floating_point<U>::value>::type,
        typename = typename std::enable_if<sizeof(T) <= sizeof(U)>::type
    >
    [[nodiscard]]
    AVEL_FINL T trunc_narrow(U u) {
        //static_assert(false, "Not implemented!");
        const std::uint64_t mask = 0x01fffffffull;

        //std::uint64_t ;

        //TODO: Implement
    }

    template<>
    [[nodiscard]]
    AVEL_FINL float trunc_narrow<float, float>(float u) {
        return u;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL double trunc_narrow<double, double>(double u) {
        return u;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL float trunc_narrow<float, double>(double u) {
        const std::uint64_t mask = (1 << 29) - 1;
        std::uint64_t u_bits = avel::bit_cast<std::uint64_t>(u);
        //std::uint64_t truncated_significand = ;
        //TODO: Implement
    }

    template<
        class T,
        class U,
        typename = typename std::enable_if<std::is_floating_point<T>::value>::type,
        typename = typename std::enable_if<std::is_floating_point<U>::value>::type,
        typename = typename std::enable_if<sizeof(T) <= sizeof(U)>::type
    >
    [[nodiscard]]
    AVEL_FINL T round_narrow(U u) {
        return static_cast<T>(u); // Rely on native behavior
    }
    */

}

#endif //AVEL_SCALAR_CONVERSIONS_HPP
