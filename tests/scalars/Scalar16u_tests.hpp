#ifndef AVEL_SCALAR16U_TESTS_HPP
#define AVEL_SCALAR16U_TESTS_HPP

namespace avel_tests {

    using namespace avel;

    //=====================================================
    // bit_ceil
    //=====================================================

    TEST(Scalar16u, Bit_ceil_edge_cases) {
        EXPECT_EQ(1, bit_ceil(std::uint16_t{0}));
        EXPECT_EQ(1, bit_ceil(std::uint16_t{1}));
        EXPECT_EQ(0, bit_ceil(static_cast<std::uint16_t>(-1)));
    }

    TEST(Scalar16u, Bit_ceil_powers_of_two) {
        for (std::int32_t i = 0; i < 15; ++i) {
            std::uint16_t x = std::uint16_t{1} << i;

            EXPECT_EQ(x, bit_ceil(x));
        }
    }

    TEST(Scalar16u, Bit_ceil_powers_of_two_minus_one) {
        for (std::int32_t i = 2; i < 16; ++i) {
            std::uint16_t x = (std::uint16_t{1} << i);
            std::uint16_t y = x - 1;

            EXPECT_EQ(x, bit_ceil(y));
        }
    }

    TEST(Scalar16u, Bit_ceil_powers_of_two_plus_one) {
        for (std::int32_t i = 0; i < 15; ++i) {
            std::uint16_t x = (std::uint16_t{1} << i);
            std::uint16_t y = x + 1;

            EXPECT_EQ(x << 1, bit_ceil(y));
        }
    }

    //=====================================================
    // Bit testing
    //=====================================================

    TEST(Scalar16u, No_bits_preselected) {
        EXPECT_TRUE(avel::no_bits(std::uint16_t(0x0000)));

        for (std::uint32_t i = 0; i < sizeof(std::uint16_t) * CHAR_BIT; ++i) {
            std::uint16_t x = std::uint16_t(1) << i;
            EXPECT_FALSE(avel::no_bits(x));
        }

        for (std::uint32_t i = 0; i < sizeof(std::uint16_t) * CHAR_BIT - 1; ++i) {
            std::uint16_t x = std::uint16_t(0xffff) >> (sizeof(std::uint16_t) * CHAR_BIT - 1 - i);
            EXPECT_FALSE(avel::no_bits(x));
        }
    }

    TEST(Scalar16u, No_bits_random) {
        for (std::int32_t i = 0; i < iterations; ++i) {
            std::uint16_t x = random16u();

            bool expected = (x == 0x0000);
            bool observed = avel::no_bits(x);

            EXPECT_EQ(expected, observed);
        }
    }

    TEST(Scalar16u, No_bits_of_preselected) {
        EXPECT_TRUE(avel::no_bits_of(std::uint16_t(0x0000), std::uint16_t(0x0000)));

        for (std::uint32_t i = 0; i < sizeof(std::uint16_t) * CHAR_BIT; ++i) {
            std::uint16_t x = std::uint16_t(1) << i;
            EXPECT_TRUE(avel::no_bits_of(x, std::uint16_t(0x0000)));
        }

        EXPECT_TRUE(avel::no_bits_of(std::uint16_t(0x0000), std::uint16_t(0xffff)));

        for (std::uint32_t i = 0; i < sizeof(std::uint16_t) * CHAR_BIT - 1; ++i) {
            std::uint16_t x = std::uint16_t(0xffff) >> (sizeof(std::uint16_t) * CHAR_BIT - 1 - i);
            EXPECT_FALSE(avel::no_bits_of(x, std::uint16_t(0xffff)));
        }
    }

    TEST(Scalar16u, No_bits_of_random) {
        for (std::int32_t i = 0; i < iterations; ++i) {
            std::uint16_t x = random16u();
            std::uint16_t m = random16u();

            bool expected = true;
            for (std::int32_t j = 0; j < sizeof(std::uint16_t) * CHAR_BIT; ++j) {
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

    TEST(Scalar16u, Any_bits_preselected) {
        EXPECT_FALSE(avel::any_bits(std::uint16_t(0x0000)));

        for (std::uint32_t i = 0; i < sizeof(std::uint16_t) * CHAR_BIT; ++i) {
            std::uint16_t x = std::uint16_t(1) << i;
            EXPECT_TRUE(avel::any_bits(x));
        }

        for (std::uint32_t i = 0; i < sizeof(std::uint16_t) * CHAR_BIT - 1; ++i) {
            std::uint16_t x = std::uint16_t(0xffff) >> (sizeof(std::uint16_t) * CHAR_BIT - 1 - i);
            EXPECT_TRUE(avel::any_bits(x));
        }
    }

    TEST(Scalar16u, Any_bits_random) {
        for (std::int32_t i = 0; i < iterations; ++i) {
            std::uint16_t x = random16u();

            bool expected = (x != 0x0000);
            bool observed = avel::any_bits(x);

            EXPECT_EQ(expected, observed);
        }
    }

    TEST(Scalar16u, Any_bits_of_preselected) {
        EXPECT_FALSE(avel::any_bits_of(std::uint16_t(0x0000), std::uint16_t(0x0000)));

        for (std::uint32_t i = 0; i < sizeof(std::uint16_t) * CHAR_BIT; ++i) {
            std::uint16_t x = std::uint16_t(1) << i;
            EXPECT_FALSE(avel::any_bits_of(x, std::uint16_t(0x0000)));
        }

        EXPECT_FALSE(avel::any_bits_of(std::uint16_t(0x0000), std::uint16_t(0xffff)));

        for (std::uint32_t i = 0; i < sizeof(std::uint16_t) * CHAR_BIT - 1; ++i) {
            std::uint16_t x = std::uint16_t(0xffff) >> (sizeof(std::uint16_t) * CHAR_BIT - 1 - i);
            EXPECT_TRUE(avel::any_bits_of(x, std::uint16_t(0xffff)));
        }
    }

    TEST(Scalar16u, Any_bits_of_random) {
        for (std::int32_t i = 0; i < iterations; ++i) {
            std::uint16_t x = random16u();
            std::uint16_t m = random16u();

            bool expected = false;
            for (std::int32_t j = 0; j < sizeof(std::uint16_t) * CHAR_BIT; ++j) {
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

    TEST(Scalar16u, All_bits_preselected) {
        EXPECT_FALSE(avel::all_bits(std::uint16_t(0x0000)));

        for (std::uint32_t i = 0; i < sizeof(std::uint16_t) * CHAR_BIT; ++i) {
            std::uint16_t x = std::uint16_t(1) << i;
            EXPECT_FALSE(avel::all_bits(x));
        }

        for (std::uint32_t i = 0; i < sizeof(std::uint16_t) * CHAR_BIT - 1; ++i) {
            std::uint16_t x = std::uint16_t(0xffff) >> (sizeof(std::uint16_t) * CHAR_BIT - 1 - i);
            EXPECT_FALSE(avel::all_bits(x));
        }

        EXPECT_TRUE(avel::all_bits(std::uint16_t(0xffff)));
    }

    TEST(Scalar16u, All_bits_random) {
        for (std::int32_t i = 0; i < iterations; ++i) {
            std::uint16_t x = random16u();

            bool expected = (x == 0xffff);
            bool observed = avel::all_bits(x);

            EXPECT_EQ(expected, observed);
        }
    }

    TEST(Scalar16u, All_bits_of_preselected) {
        EXPECT_TRUE(avel::all_bits_of(std::uint16_t(0x0000), std::uint16_t(0x0000)));

        for (std::uint32_t i = 0; i < sizeof(std::uint16_t) * CHAR_BIT; ++i) {
            std::uint16_t x = std::uint16_t(1) << i;
            EXPECT_TRUE(avel::all_bits_of(x, std::uint16_t(0x0000)));
        }

        for (std::uint32_t i = 0; i < sizeof(std::uint16_t) * CHAR_BIT - 1; ++i) {
            std::uint16_t x = std::uint16_t(0xffff) >> (sizeof(std::uint16_t) * CHAR_BIT - 1 - i);
            EXPECT_FALSE(avel::all_bits_of(x, std::uint16_t(0xffff)));
        }

        EXPECT_TRUE(avel::all_bits_of(std::uint16_t(0xffff), std::uint16_t(0xffff)));
    }

    TEST(Scalar16u, All_bits_of_random) {
        for (std::int32_t i = 0; i < iterations; ++i) {
            std::uint16_t x = random16u();
            std::uint16_t m = random16u();

            bool expected = true;
            for (std::int32_t j = 0; j < sizeof(std::uint16_t) * CHAR_BIT; ++j) {
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

#endif //AVEL_SCALAR16U_TESTS_HPP
