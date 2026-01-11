#ifndef AVEL_SCALAR64U_TESTS_HPP
#define AVEL_SCALAR64U_TESTS_HPP

namespace avel_tests {

    using namespace avel;

    //=====================================================
    // bit_ceil
    //=====================================================

    TEST(Scalar64u, Bit_ceil_edge_cases) {
        EXPECT_EQ(1, bit_ceil(std::uint64_t{0}));
        EXPECT_EQ(1, bit_ceil(std::uint64_t{1}));
        EXPECT_EQ(0, bit_ceil(static_cast<std::uint64_t>(-1)));
    }

    TEST(Scalar64u, Bit_ceil_powers_of_two) {
        for (std::int32_t i = 0; i < 63; ++i) {
            std::uint64_t x = std::uint64_t{1} << i;

            EXPECT_EQ(x, bit_ceil(x));
        }
    }

    TEST(Scalar64u, Bit_ceil_powers_of_two_minus_one) {
        for (std::int32_t i = 2; i < 64; ++i) {
            std::uint64_t x = (std::uint64_t{1} << i);
            std::uint64_t y = x - 1;

            EXPECT_EQ(x, bit_ceil(y));
        }
    }

    TEST(Scalar64u, Bit_ceil_powers_of_two_plus_one) {
        for (std::int32_t i = 0; i < 63; ++i) {
            std::uint64_t x = (std::uint64_t{1} << i);
            std::uint64_t y = x + 1;

            EXPECT_EQ(x << 1, bit_ceil(y));
        }
    }

    //=====================================================
    // Bit testing
    //=====================================================

    TEST(Scalar64u, No_bits_preselected) {
        EXPECT_TRUE(avel::no_bits(std::uint64_t(0x0000000000000000)));

        for (std::uint64_t i = 0; i < sizeof(std::uint64_t) * CHAR_BIT; ++i) {
            std::uint64_t x = std::uint64_t(1) << i;
            EXPECT_FALSE(avel::no_bits(x));
        }

        for (std::uint64_t i = 0; i < sizeof(std::uint64_t) * CHAR_BIT - 1; ++i) {
            std::uint64_t x = std::uint64_t(0xffffffffffffffff) >> (sizeof(std::uint64_t) * CHAR_BIT - 1 - i);
            EXPECT_FALSE(avel::no_bits(x));
        }
    }

    TEST(Scalar64u, No_bits_random) {
        for (std::uint64_t i = 0; i < iterations; ++i) {
            std::uint64_t x = random64u();

            bool expected = (x == 0x0000000000000000);
            bool observed = avel::no_bits(x);

            EXPECT_EQ(expected, observed);
        }
    }

    TEST(Scalar64u, No_bits_of_preselected) {
        EXPECT_TRUE(avel::no_bits_of(std::uint64_t(0x0000000000000000), std::uint64_t(0x0000000000000000)));

        for (std::uint64_t i = 0; i < sizeof(std::uint64_t) * CHAR_BIT; ++i) {
            std::uint64_t x = std::uint64_t(1) << i;
            EXPECT_TRUE(avel::no_bits_of(x, std::uint64_t(0x0000000000000000)));
        }

        EXPECT_TRUE(avel::no_bits_of(std::uint64_t(0x0000000000000000), std::uint64_t(0xffffffffffffffff)));

        for (std::uint64_t i = 0; i < sizeof(std::uint64_t) * CHAR_BIT - 1; ++i) {
            std::uint64_t x = std::uint64_t(0xffffffffffffffff) >> (sizeof(std::uint64_t) * CHAR_BIT - 1 - i);
            EXPECT_FALSE(avel::no_bits_of(x, std::uint64_t(0xffffffffffffffff)));
        }
    }

    TEST(Scalar64u, No_bits_of_random) {
        for (std::uint64_t i = 0; i < iterations; ++i) {
            std::uint64_t x = random64u();
            std::uint64_t m = random64u();

            bool expected = true;
            for (std::uint64_t j = 0; j < sizeof(std::uint64_t) * CHAR_BIT; ++j) {
                bool x_bit = (x >> j) & 0x1;
                bool m_bit = (m >> j) & 0x1;

                if (x_bit && m_bit) {
                    expected &= false;
                }
            }

            bool observed = avel::no_bits_of(x, m);

            EXPECT_EQ(expected, observed);
        }
    }

    TEST(Scalar64u, Any_bits_preselected) {
        EXPECT_FALSE(avel::any_bits(std::uint64_t(0x0000000000000000)));

        for (std::uint64_t i = 0; i < sizeof(std::uint64_t) * CHAR_BIT; ++i) {
            std::uint64_t x = std::uint64_t(1) << i;
            EXPECT_TRUE(avel::any_bits(x));
        }

        for (std::uint64_t i = 0; i < sizeof(std::uint64_t) * CHAR_BIT - 1; ++i) {
            std::uint64_t x = std::uint64_t(0xffffffffffffffff) >> (sizeof(std::uint64_t) * CHAR_BIT - 1 - i);
            EXPECT_TRUE(avel::any_bits(x));
        }
    }

    TEST(Scalar64u, Any_bits_random) {
        for (std::uint64_t i = 0; i < iterations; ++i) {
            std::uint64_t x = random64u();

            bool expected = (x != 0x0000000000000000);
            bool observed = avel::any_bits(x);

            EXPECT_EQ(expected, observed);
        }
    }

    TEST(Scalar64u, Any_bits_of_preselected) {
        EXPECT_FALSE(avel::any_bits_of(std::uint64_t(0x0000000000000000), std::uint64_t(0x0000000000000000)));

        for (std::uint64_t i = 0; i < sizeof(std::uint64_t) * CHAR_BIT; ++i) {
            std::uint64_t x = std::uint64_t(1) << i;
            EXPECT_FALSE(avel::any_bits_of(x, std::uint64_t(0x0000000000000000)));
        }

        EXPECT_FALSE(avel::any_bits_of(std::uint64_t(0x0000000000000000), std::uint64_t(0xffffffffffffffff)));

        for (std::uint64_t i = 0; i < sizeof(std::uint64_t) * CHAR_BIT - 1; ++i) {
            std::uint64_t x = std::uint64_t(0xffffffffffffffff) >> (sizeof(std::uint64_t) * CHAR_BIT - 1 - i);
            EXPECT_TRUE(avel::any_bits_of(x, std::uint64_t(0xffffffffffffffff)));
        }
    }

    TEST(Scalar64u, Any_bits_of_random) {
        for (std::uint64_t i = 0; i < iterations; ++i) {
            std::uint64_t x = random64u();
            std::uint64_t m = random64u();

            bool expected = false;
            for (std::uint64_t j = 0; j < sizeof(std::uint64_t) * CHAR_BIT; ++j) {
                bool x_bit = (x >> j) & 0x1;
                bool m_bit = (m >> j) & 0x1;

                if (x_bit && m_bit) {
                    expected |= true;
                }
            }

            bool observed = avel::any_bits_of(x, m);

            EXPECT_EQ(expected, observed);
        }
    }

    TEST(Scalar64u, All_bits_preselected) {
        EXPECT_FALSE(avel::all_bits(std::uint64_t(0x0000000000000000)));

        for (std::uint64_t i = 0; i < sizeof(std::uint64_t) * CHAR_BIT; ++i) {
            std::uint64_t x = std::uint64_t(1) << i;
            EXPECT_FALSE(avel::all_bits(x));
        }

        for (std::uint64_t i = 0; i < sizeof(std::uint64_t) * CHAR_BIT - 1; ++i) {
            std::uint64_t x = std::uint64_t(0xffffffffffffffff) >> (sizeof(std::uint64_t) * CHAR_BIT - 1 - i);
            EXPECT_FALSE(avel::all_bits(x));
        }

        EXPECT_TRUE(avel::all_bits(std::uint64_t(0xffffffffffffffff)));
    }

    TEST(Scalar64u, All_bits_random) {
        for (std::uint64_t i = 0; i < iterations; ++i) {
            std::uint64_t x = random64u();

            bool expected = (x == 0xffffffffffffffff);
            bool observed = avel::all_bits(x);

            EXPECT_EQ(expected, observed);
        }
    }

    TEST(Scalar64u, All_bits_of_preselected) {
        EXPECT_TRUE(avel::all_bits_of(std::uint64_t(0x0000000000000000), std::uint64_t(0x0000000000000000)));

        for (std::uint64_t i = 0; i < sizeof(std::uint64_t) * CHAR_BIT; ++i) {
            std::uint64_t x = std::uint64_t(1) << i;
            EXPECT_TRUE(avel::all_bits_of(x, std::uint64_t(0x0000000000000000)));
        }

        for (std::uint64_t i = 0; i < sizeof(std::uint64_t) * CHAR_BIT - 1; ++i) {
            std::uint64_t x = std::uint64_t(0xffffffffffffffff) >> (sizeof(std::uint64_t) * CHAR_BIT - 1 - i);
            EXPECT_FALSE(avel::all_bits_of(x, std::uint64_t(0xffffffffffffffff)));
        }

        EXPECT_TRUE(avel::all_bits_of(std::uint64_t(0xffffffffffffffff), std::uint64_t(0xffffffffffffffff)));
    }

    TEST(Scalar64u, All_bits_of_random) {
        for (std::uint64_t i = 0; i < iterations; ++i) {
            std::uint64_t x = random64u();
            std::uint64_t m = random64u();

            bool expected = true;
            for (std::uint64_t j = 0; j < sizeof(std::uint64_t) * CHAR_BIT; ++j) {
                bool x_bit = (x >> j) & 0x1;
                bool m_bit = (m >> j) & 0x1;

                if (!x_bit && m_bit) {
                    expected &= false;
                }
            }

            bool observed = avel::all_bits_of(x, m);

            EXPECT_EQ(expected, observed);
        }
    }

}

#endif //AVEL_SCALAR64U_TESTS_HPP
