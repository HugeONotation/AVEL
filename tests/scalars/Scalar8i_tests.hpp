#ifndef AVEL_SCALAR8I_TESTS_HPP
#define AVEL_SCALAR8I_TESTS_HPP

namespace avel_tests {

    using namespace avel;

    //=====================================================
    // Comparisons
    //=====================================================

    TEST(Scalar8i, cmp_equal_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint8_t x = random_val<std::uint8_t>();
            std::int8_t  y = random_val<std::int8_t>();

            auto result = cmp_equal(x, y);

            if (x >= 0x80 || y < 0) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x == y);
            }
        }
    }

    TEST(Scalar8i, cmp_equal_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int8_t  x = random_val<std::int8_t>();
            std::uint8_t y = random_val<std::uint8_t>();

            auto result = cmp_equal(x, y);

            if (x < 0 || y >= 0x80) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x == y);
            }
        }
    }

    TEST(Scalar8i, cmp_not_equal_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint8_t x = random_val<std::uint8_t>();
            std::int8_t  y = random_val<std::int8_t>();

            auto result = cmp_not_equal(x, y);

            if (x < 0 || y >= 0x80) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x != y);
            }
        }
    }

    TEST(Scalar8i, cmp_not_equal_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int8_t  x = random_val<std::int8_t>();
            std::uint8_t y = random_val<std::uint8_t>();

            auto result = cmp_not_equal(x, y);

            if (x < 0 || y >= 0x80) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x != y);
            }
        }
    }

    TEST(Scalar8i, cmp_less_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint8_t x = random_val<std::uint8_t>();
            std::int8_t  y = random_val<std::int8_t>();

            auto result = cmp_less(x, y);

            if (x >= 0x80 || y < 0) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x < y);
            }
        }
    }

    TEST(Scalar8i, cmp_less_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int8_t  x = random_val<std::int8_t>();
            std::uint8_t y = random_val<std::uint8_t>();

            auto result = cmp_less(x, y);

            if (x < 0 || y >= 0x80) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x < y);
            }
        }
    }

    TEST(Scalar8i, Cmp_le_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint8_t x = random_val<std::uint8_t>();
            std::int8_t  y = random_val<std::int8_t>();

            auto result = cmp_less_equal(x, y);

            if (x >= 0x80 || y < 0) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x <= y);
            }
        }
    }

    TEST(Scalar8i, Cmp_le_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int8_t  x = random_val<std::int8_t>();
            std::uint8_t y = random_val<std::uint8_t>();

            auto result = cmp_less_equal(x, y);

            if (x < 0 || y >= 0x80) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x <= y);
            }
        }
    }

    TEST(Scalar8i, cmp_greater_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint8_t x = random_val<std::uint8_t>();
            std::int8_t  y = random_val<std::int8_t>();

            auto result = cmp_greater(x, y);

            if (x >= 0x80 || y < 0) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x > y);
            }
        }
    }

    TEST(Scalar8i, cmp_greater_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int8_t  x = random_val<std::int8_t>();
            std::uint8_t y = random_val<std::uint8_t>();

            auto result = cmp_greater(x, y);

            if (x < 0 || y >= 0x80) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x > y);
            }
        }
    }

    TEST(Scalar8i, cmp_greater_equal_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint8_t x = random_val<std::uint8_t>();
            std::int8_t  y = random_val<std::int8_t>();

            auto result = cmp_greater_equal(x, y);

            if (x >= 0x80 || y < 0) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x >= y);
            }
        }
    }

    TEST(Scalar8i, cmp_greater_equal_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int8_t  x = random_val<std::int8_t>();
            std::uint8_t y = random_val<std::uint8_t>();

            auto result = cmp_greater_equal(x, y);

            if (x < 0 || y >= 0x80) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x >= y);
            }
        }
    }

    //=====================================================
    // Integer operations
    //=====================================================

    TEST(Scalar8i, countl_sign_powers_of_two) {
        for (std::size_t i = 0; i < 7; ++i) {
            std::int8_t input = std::int8_t(0x01) << (6 - i);
            std::int8_t observed = avel::countl_sign(input);
            std::int8_t expected = i;

            EXPECT_EQ(observed, expected);
        }

        {
            std::int8_t input = 0x00;
            std::int8_t observed = avel::countl_sign(input);
            std::int8_t expected = 7;

            EXPECT_EQ(observed, expected);
        }

        for (std::size_t i = 0; i < 7; ++i) {
            std::int8_t input = std::int8_t(0xff) << i;
            std::int8_t observed = avel::countl_sign(input);
            std::int8_t expected = (7 - i);

            EXPECT_EQ(observed, expected);
        }
    }

    TEST(Scalar8i, countl_sign_random) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int8_t x = random_val<std::int8_t>();

            std::uint8_t observed = avel::countl_sign(x);

            if (x < 0) {
                EXPECT_EQ(observed, avel::countl_one(x) - 1);
            } else {
                EXPECT_EQ(observed, avel::countl_zero(x) - 1);
            }
        }
    }

    //=====================================================
    // Bit testing
    //=====================================================

    TEST(Scalar8i, No_bits_preselected) {
        EXPECT_TRUE(avel::no_bits(std::int8_t(0x00)));

        for (std::uint32_t i = 0; i < sizeof(std::int8_t) * CHAR_BIT; ++i) {
            std::int8_t x = std::int8_t(1) << i;
            EXPECT_FALSE(avel::no_bits(x));
        }

        for (std::uint32_t i = 0; i < sizeof(std::int8_t) * CHAR_BIT - 1; ++i) {
            std::int8_t x = std::uint8_t(0xff) >> (sizeof(std::int8_t) * CHAR_BIT - 1 - i);
            EXPECT_FALSE(avel::no_bits(x));
        }
    }

    TEST(Scalar8i, No_bits_random) {
        for (std::int32_t i = 0; i < iterations; ++i) {
            std::int8_t x = random8u();

            bool expected = (x == 0x00);
            bool observed = avel::no_bits(x);

            EXPECT_EQ(expected, observed);
        }
    }

    TEST(Scalar8i, No_bits_of_preselected) {
        EXPECT_TRUE(avel::no_bits_of(std::int8_t(0x00), std::int8_t(0x00)));

        for (std::uint32_t i = 0; i < sizeof(std::int8_t) * CHAR_BIT; ++i) {
            std::int8_t x = std::int8_t(1) << i;
            EXPECT_TRUE(avel::no_bits_of(x, std::int8_t(0x00)));
        }

        EXPECT_TRUE(avel::no_bits_of(std::int8_t(0x00), std::int8_t(0xff)));

        for (std::uint32_t i = 0; i < sizeof(std::int8_t) * CHAR_BIT - 1; ++i) {
            std::int8_t x = std::uint8_t(0xff) >> (sizeof(std::int8_t) * CHAR_BIT - 1 - i);
            EXPECT_FALSE(avel::no_bits_of(x, std::int8_t(0xff)));
        }
    }

    TEST(Scalar8i, No_bits_of_random) {
        for (std::int32_t i = 0; i < iterations; ++i) {
            std::int8_t x = random8u();
            std::int8_t m = random8u();

            bool expected = true;
            for (std::int32_t j = 0; j < sizeof(std::int8_t) * CHAR_BIT; ++j) {
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

    TEST(Scalar8i, Any_bits_preselected) {
        EXPECT_FALSE(avel::any_bits(std::int8_t(0x00)));

        for (std::uint32_t i = 0; i < sizeof(std::int8_t) * CHAR_BIT; ++i) {
            std::int8_t x = std::int8_t(1) << i;
            EXPECT_TRUE(avel::any_bits(x));
        }

        for (std::uint32_t i = 0; i < sizeof(std::int8_t) * CHAR_BIT - 1; ++i) {
            std::int8_t x = std::uint8_t(0xff) >> (sizeof(std::int8_t) * CHAR_BIT - 1 - i);
            EXPECT_TRUE(avel::any_bits(x));
        }
    }

    TEST(Scalar8i, Any_bits_random) {
        for (std::int32_t i = 0; i < iterations; ++i) {
            std::int8_t x = random8u();

            bool expected = (x != 0x00);
            bool observed = avel::any_bits(x);

            EXPECT_EQ(expected, observed);
        }
    }

    TEST(Scalar8i, Any_bits_of_preselected) {
        EXPECT_FALSE(avel::any_bits_of(std::int8_t(0x00), std::int8_t(0x00)));

        for (std::uint32_t i = 0; i < sizeof(std::int8_t) * CHAR_BIT; ++i) {
            std::int8_t x = std::int8_t(1) << i;
            EXPECT_FALSE(avel::any_bits_of(x, std::int8_t(0x00)));
        }

        EXPECT_FALSE(avel::any_bits_of(std::int8_t(0x00), std::int8_t(0xff)));

        for (std::uint32_t i = 0; i < sizeof(std::int8_t) * CHAR_BIT - 1; ++i) {
            std::int8_t x = std::uint8_t(0xff) >> (sizeof(std::int8_t) * CHAR_BIT - 1 - i);
            EXPECT_TRUE(avel::any_bits_of(x, std::int8_t(0xff)));
        }
    }

    TEST(Scalar8i, Any_bits_of_random) {
        for (std::int32_t i = 0; i < iterations; ++i) {
            std::int8_t x = random8u();
            std::int8_t m = random8u();

            bool expected = false;
            for (std::int32_t j = 0; j < sizeof(std::int8_t) * CHAR_BIT; ++j) {
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

    TEST(Scalar8i, All_bits_preselected) {
        EXPECT_FALSE(avel::all_bits(std::int8_t(0x00)));

        for (std::uint32_t i = 0; i < sizeof(std::int8_t) * CHAR_BIT; ++i) {
            std::int8_t x = std::int8_t(1) << i;
            EXPECT_FALSE(avel::all_bits(x));
        }

        for (std::uint32_t i = 0; i < sizeof(std::int8_t) * CHAR_BIT - 1; ++i) {
            std::int8_t x = std::uint8_t(0xff) >> (sizeof(std::int8_t) * CHAR_BIT - 1 - i);
            EXPECT_FALSE(avel::all_bits(x));
        }

        EXPECT_TRUE(avel::all_bits(std::int8_t(0xff)));
    }

    TEST(Scalar8i, All_bits_random) {
        for (std::int32_t i = 0; i < iterations; ++i) {
            std::int8_t x = random8u();

            bool expected = (x == 0xff);
            bool observed = avel::all_bits(x);

            EXPECT_EQ(expected, observed);
        }
    }

    TEST(Scalar8i, All_bits_of_preselected) {
        EXPECT_TRUE(avel::all_bits_of(std::int8_t(0x00), std::int8_t(0x00)));

        for (std::uint32_t i = 0; i < sizeof(std::int8_t) * CHAR_BIT; ++i) {
            std::int8_t x = std::int8_t(1) << i;
            EXPECT_TRUE(avel::all_bits_of(x, std::int8_t(0x00)));
        }

        for (std::uint32_t i = 0; i < sizeof(std::int8_t) * CHAR_BIT - 1; ++i) {
            std::int8_t x = std::uint8_t(0xff) >> (sizeof(std::int8_t) * CHAR_BIT - 1 - i);
            EXPECT_FALSE(avel::all_bits_of(x, std::int8_t(0xff)));
        }

        EXPECT_TRUE(avel::all_bits_of(std::int8_t(0xff), std::int8_t(0xff)));
    }

    TEST(Scalar8i, All_bits_of_random) {
        for (std::int32_t i = 0; i < iterations; ++i) {
            std::int8_t x = random8u();
            std::int8_t m = random8u();

            bool expected = true;
            for (std::int32_t j = 0; j < sizeof(std::int8_t) * CHAR_BIT; ++j) {
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

#endif //AVEL_SCALAR8I_TESTS_HPP
