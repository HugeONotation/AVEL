#ifndef AVEL_SCALAR16I_TESTS_HPP
#define AVEL_SCALAR16I_TESTS_HPP

namespace avel_tests {

    using namespace avel;

    //=====================================================
    // Comparisons
    //=====================================================

    TEST(Scalar16i, cmp_equal_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint16_t x = random_val<std::uint16_t>();
            std::int16_t  y = random_val<std::int16_t>();

            auto result = cmp_equal(x, y);

            if (x >= 0x8000 || y < 0) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x == y);
            }
        }
    }

    TEST(Scalar16i, cmp_equal_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int16_t  x = random_val<std::int16_t>();
            std::uint16_t y = random_val<std::uint16_t>();

            auto result = cmp_equal(x, y);

            if (x < 0 || y >= 0x8000) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x == y);
            }
        }
    }

    TEST(Scalar16i, cmp_not_equal_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint16_t x = random_val<std::uint16_t>();
            std::int16_t  y = random_val<std::int16_t>();

            auto result = cmp_not_equal(x, y);

            if (x < 0 || y >= 0x8000) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x != y);
            }
        }
    }

    TEST(Scalar16i, cmp_not_equal_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int16_t  x = random_val<std::int16_t>();
            std::uint16_t y = random_val<std::uint16_t>();

            auto result = cmp_not_equal(x, y);

            if (x < 0 || y >= 0x8000) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x != y);
            }
        }
    }

    TEST(Scalar16i, cmp_less_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint16_t x = random_val<std::uint16_t>();
            std::int16_t  y = random_val<std::int16_t>();

            auto result = cmp_less(x, y);

            if (x >= 0x8000 || y < 0) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x < y);
            }
        }
    }

    TEST(Scalar16i, cmp_less_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int16_t  x = random_val<std::int16_t>();
            std::uint16_t y = random_val<std::uint16_t>();

            auto result = cmp_less(x, y);

            if (x < 0 || y >= 0x8000) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x < y);
            }
        }
    }

    TEST(Scalar16i, Cmp_le_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint16_t x = random_val<std::uint16_t>();
            std::int16_t  y = random_val<std::int16_t>();

            auto result = cmp_less_equal(x, y);

            if (x >= 0x8000 || y < 0) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x <= y);
            }
        }
    }

    TEST(Scalar16i, Cmp_le_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int16_t  x = random_val<std::int16_t>();
            std::uint16_t y = random_val<std::uint16_t>();

            auto result = cmp_less_equal(x, y);

            if (x < 0 || y >= 0x8000) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x <= y);
            }
        }
    }

    TEST(Scalar16i, cmp_greater_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint16_t x = random_val<std::uint16_t>();
            std::int16_t  y = random_val<std::int16_t>();

            auto result = cmp_greater(x, y);

            if (x >= 0x8000 || y < 0) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x > y);
            }
        }
    }

    TEST(Scalar16i, cmp_greater_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int16_t  x = random_val<std::int16_t>();
            std::uint16_t y = random_val<std::uint16_t>();

            auto result = cmp_greater(x, y);

            if (x < 0 || y >= 0x8000) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x > y);
            }
        }
    }

    TEST(Scalar16i, cmp_greater_equal_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint16_t x = random_val<std::uint16_t>();
            std::int16_t  y = random_val<std::int16_t>();

            auto result = cmp_greater_equal(x, y);

            if (x >= 0x8000 || y < 0) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x >= y);
            }
        }
    }

    TEST(Scalar16i, cmp_greater_equal_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int16_t  x = random_val<std::int16_t>();
            std::uint16_t y = random_val<std::uint16_t>();

            auto result = cmp_greater_equal(x, y);

            if (x < 0 || y >= 0x8000) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x >= y);
            }
        }
    }

    //=====================================================
    // Integer operations
    //=====================================================

    TEST(Scalar16i, countl_sign_powers_of_two) {
        for (std::size_t i = 0; i < 15; ++i) {
            std::int16_t input = std::int16_t(0x0001) << (14 - i);
            std::int16_t observed = avel::countl_sign(input);
            std::int16_t expected = i;

            EXPECT_EQ(observed, expected);
        }

        {
            std::int16_t input = 0x0000;
            std::int16_t observed = avel::countl_sign(input);
            std::int16_t expected = 15;

            EXPECT_EQ(observed, expected);
        }

        for (std::size_t i = 0; i < 15; ++i) {
            std::int16_t input = std::int8_t(0xffff) << i;
            std::int16_t observed = avel::countl_sign(input);
            std::int16_t expected = (15 - i);

            EXPECT_EQ(observed, expected);
        }
    }

    //=====================================================
    // Bit testing
    //=====================================================

    TEST(Scalar16i, No_bits_preselected) {
        EXPECT_TRUE(avel::no_bits(std::int16_t(0x0000)));

        for (std::uint32_t i = 0; i < sizeof(std::int16_t) * CHAR_BIT; ++i) {
            std::int16_t x = std::int16_t(1) << i;
            EXPECT_FALSE(avel::no_bits(x));
        }

        for (std::uint32_t i = 0; i < sizeof(std::uint16_t) * CHAR_BIT - 1; ++i) {
            std::int16_t x = std::uint16_t(0xffff) >> (sizeof(std::uint16_t) * CHAR_BIT - 1 - i);
            EXPECT_FALSE(avel::no_bits(x));
        }
    }

    TEST(Scalar16i, No_bits_random) {
        for (std::int32_t i = 0; i < iterations; ++i) {
            std::int16_t x = random16u();

            bool expected = (x == 0x0000);
            bool observed = avel::no_bits(x);

            EXPECT_EQ(expected, observed);
        }
    }

    TEST(Scalar16i, No_bits_of_preselected) {
        EXPECT_TRUE(avel::no_bits_of(std::int16_t(0x0000), std::int16_t(0x0000)));

        for (std::uint32_t i = 0; i < sizeof(std::int16_t) * CHAR_BIT; ++i) {
            std::int16_t x = std::int16_t(1) << i;
            EXPECT_TRUE(avel::no_bits_of(x, std::int16_t(0x0000)));
        }

        EXPECT_TRUE(avel::no_bits_of(std::int16_t(0x0000), std::int16_t(0xffff)));

        for (std::uint32_t i = 0; i < sizeof(std::uint16_t) * CHAR_BIT - 1; ++i) {
            std::int16_t x = std::uint16_t(0xffff) >> (sizeof(std::uint16_t) * CHAR_BIT - 1 - i);
            EXPECT_FALSE(avel::no_bits_of(x, std::int16_t(0xffff)));
        }
    }

    TEST(Scalar16i, No_bits_of_random) {
        for (std::int32_t i = 0; i < iterations; ++i) {
            std::int16_t x = random16u();
            std::int16_t m = random16u();

            bool expected = true;
            for (std::int32_t j = 0; j < sizeof(std::int16_t) * CHAR_BIT; ++j) {
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

    TEST(Scalar16i, Any_bits_preselected) {
        EXPECT_FALSE(avel::any_bits(std::int16_t(0x0000)));

        for (std::uint32_t i = 0; i < sizeof(std::int16_t) * CHAR_BIT; ++i) {
            std::int16_t x = std::int16_t(1) << i;
            EXPECT_TRUE(avel::any_bits(x));
        }

        for (std::uint32_t i = 0; i < sizeof(std::uint16_t) * CHAR_BIT - 1; ++i) {
            std::int16_t x = std::uint16_t(0xffff) >> (sizeof(std::uint16_t) * CHAR_BIT - 1 - i);
            EXPECT_TRUE(avel::any_bits(x));
        }
    }

    TEST(Scalar16i, Any_bits_random) {
        for (std::int32_t i = 0; i < iterations; ++i) {
            std::int16_t x = random16u();

            bool expected = (x != 0x0000);
            bool observed = avel::any_bits(x);

            EXPECT_EQ(expected, observed);
        }
    }

    TEST(Scalar16i, Any_bits_of_preselected) {
        EXPECT_FALSE(avel::any_bits_of(std::int16_t(0x0000), std::int16_t(0x0000)));

        for (std::uint32_t i = 0; i < sizeof(std::int16_t) * CHAR_BIT; ++i) {
            std::int16_t x = std::int16_t(1) << i;
            EXPECT_FALSE(avel::any_bits_of(x, std::int16_t(0x0000)));
        }

        EXPECT_FALSE(avel::any_bits_of(std::int16_t(0x0000), std::int16_t(0xffff)));

        for (std::uint32_t i = 0; i < sizeof(std::uint16_t) * CHAR_BIT - 1; ++i) {
            std::int16_t x = std::uint16_t(0xffff) >> (sizeof(std::uint16_t) * CHAR_BIT - 1 - i);
            EXPECT_TRUE(avel::any_bits_of(x, std::int16_t(0xffff)));
        }
    }

    TEST(Scalar16i, Any_bits_of_random) {
        for (std::int32_t i = 0; i < iterations; ++i) {
            std::int16_t x = random16u();
            std::int16_t m = random16u();

            bool expected = false;
            for (std::int32_t j = 0; j < sizeof(std::int16_t) * CHAR_BIT; ++j) {
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

    TEST(Scalar16i, All_bits_preselected) {
        EXPECT_FALSE(avel::all_bits(std::int16_t(0x0000)));

        for (std::uint32_t i = 0; i < sizeof(std::int16_t) * CHAR_BIT; ++i) {
            std::int16_t x = std::int16_t(1) << i;
            EXPECT_FALSE(avel::all_bits(x));
        }

        for (std::uint32_t i = 0; i < sizeof(std::uint16_t) * CHAR_BIT - 1; ++i) {
            std::int16_t x = std::uint16_t(0xffff) >> (sizeof(std::uint16_t) * CHAR_BIT - 1 - i);
            EXPECT_FALSE(avel::all_bits(x));
        }

        EXPECT_TRUE(avel::all_bits(std::int16_t(0xffff)));
    }

    TEST(Scalar16i, All_bits_random) {
        for (std::int32_t i = 0; i < iterations; ++i) {
            std::int16_t x = random16u();

            bool expected = (x == 0xffff);
            bool observed = avel::all_bits(x);

            EXPECT_EQ(expected, observed);
        }
    }

    TEST(Scalar16i, All_bits_of_preselected) {
        EXPECT_TRUE(avel::all_bits_of(std::int16_t(0x0000), std::int16_t(0x0000)));

        for (std::uint32_t i = 0; i < sizeof(std::int16_t) * CHAR_BIT; ++i) {
            std::int16_t x = std::int16_t(1) << i;
            EXPECT_TRUE(avel::all_bits_of(x, std::int16_t(0x0000)));
        }

        for (std::uint32_t i = 0; i < sizeof(std::uint16_t) * CHAR_BIT - 1; ++i) {
            std::int16_t x = std::uint16_t(0xffff) >> (sizeof(std::uint16_t) * CHAR_BIT - 1 - i);
            EXPECT_FALSE(avel::all_bits_of(x, std::int16_t(0xffff)));
        }

        EXPECT_TRUE(avel::all_bits_of(std::int16_t(0xffff), std::int16_t(0xffff)));
    }

    TEST(Scalar16i, All_bits_of_random) {
        for (std::int32_t i = 0; i < iterations; ++i) {
            std::int16_t x = random16u();
            std::int16_t m = random16u();

            bool expected = true;
            for (std::int32_t j = 0; j < sizeof(std::int16_t) * CHAR_BIT; ++j) {
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

#endif //AVEL_SCALAR16I_TESTS_HPP
