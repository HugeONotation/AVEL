#ifndef AVEL_SCALAR32I_TESTS_HPP
#define AVEL_SCALAR32I_TESTS_HPP

namespace avel_tests {

    using namespace avel;

    //=====================================================
    // Comparisons
    //=====================================================

    TEST(Scalar32i, cmp_equal_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint32_t x = random_val<std::uint32_t>();
            std::int32_t  y = random_val<std::int32_t>();

            auto result = cmp_equal(x, y);

            if (x >= 0x80000000 || y < 0) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x == y);
            }
        }
    }

    TEST(Scalar32i, cmp_equal_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int32_t  x = random_val<std::int32_t>();
            std::uint32_t y = random_val<std::uint32_t>();

            auto result = cmp_equal(x, y);

            if (x < 0 || y >= 0x80000000) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x == y);
            }
        }
    }

    TEST(Scalar32i, cmp_not_equal_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint32_t x = random_val<std::uint32_t>();
            std::int32_t  y = random_val<std::int32_t>();

            auto result = cmp_not_equal(x, y);

            if (x < 0 || y >= 0x80000000) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x != y);
            }
        }
    }

    TEST(Scalar32i, cmp_not_equal_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int32_t  x = random_val<std::int32_t>();
            std::uint32_t y = random_val<std::uint32_t>();

            auto result = cmp_not_equal(x, y);

            if (x < 0 || y >= 0x80000000) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x != y);
            }
        }
    }

    TEST(Scalar32i, cmp_less_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint32_t x = random_val<std::uint32_t>();
            std::int32_t  y = random_val<std::int32_t>();

            auto result = cmp_less(x, y);

            if (x >= 0x80000000 || y < 0) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x < y);
            }
        }
    }

    TEST(Scalar32i, cmp_less_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int32_t  x = random_val<std::int32_t>();
            std::uint32_t y = random_val<std::uint32_t>();

            auto result = cmp_less(x, y);

            if (x < 0 || y >= 0x80000000) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x < y);
            }
        }
    }

    TEST(Scalar32i, Cmp_le_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint32_t x = random_val<std::uint32_t>();
            std::int32_t  y = random_val<std::int32_t>();

            auto result = cmp_less_equal(x, y);

            if (x >= 0x80000000 || y < 0) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x <= y);
            }
        }
    }

    TEST(Scalar32i, Cmp_le_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int32_t  x = random_val<std::int32_t>();
            std::uint32_t y = random_val<std::uint32_t>();

            auto result = cmp_less_equal(x, y);

            if (x < 0 || y >= 0x80000000) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x <= y);
            }
        }
    }

    TEST(Scalar32i, cmp_greater_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint32_t x = random_val<std::uint32_t>();
            std::int32_t  y = random_val<std::int32_t>();

            auto result = cmp_greater(x, y);

            if (x >= 0x80000000 || y < 0) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x > y);
            }
        }
    }

    TEST(Scalar32i, cmp_greater_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int32_t  x = random_val<std::int32_t>();
            std::uint32_t y = random_val<std::uint32_t>();

            auto result = cmp_greater(x, y);

            if (x < 0 || y >= 0x80000000) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x > y);
            }
        }
    }

    TEST(Scalar32i, cmp_greater_equal_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint32_t x = random_val<std::uint32_t>();
            std::int32_t  y = random_val<std::int32_t>();

            auto result = cmp_greater_equal(x, y);

            if (x >= 0x80000000 || y < 0) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x >= y);
            }
        }
    }

    TEST(Scalar32i, cmp_greater_equal_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int32_t  x = random_val<std::int32_t>();
            std::uint32_t y = random_val<std::uint32_t>();

            auto result = cmp_greater_equal(x, y);

            if (x < 0 || y >= 0x80000000) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x >= y);
            }
        }
    }

    //=====================================================
    // Integer operations
    //=====================================================

    TEST(Scalar32i, countl_sign_powers_of_two) {
        for (std::size_t i = 0; i < 31; ++i) {
            std::int32_t input = std::int32_t(0x00000001) << (30 - i);
            std::int32_t observed = avel::countl_sign(input);
            std::int32_t expected = i;

            EXPECT_EQ(observed, expected);
        }

        {
            std::int32_t input = 0x00;
            std::int32_t observed = avel::countl_sign(input);
            std::int32_t expected = 31;

            EXPECT_EQ(observed, expected);
        }

        for (std::size_t i = 0; i < 31; ++i) {
            std::int32_t input = std::int32_t(0xffffffff) << i;
            std::int32_t observed = avel::countl_sign(input);
            std::int32_t expected = (31 - i);

            EXPECT_EQ(observed, expected);
        }
    }

    //=====================================================
    // Bit testing
    //=====================================================

    TEST(Scalar32i, No_bits_preselected) {
        EXPECT_TRUE(avel::no_bits(std::int32_t(0x00000000)));

        for (std::int32_t i = 0; i < sizeof(std::int32_t) * CHAR_BIT; ++i) {
            std::int32_t x = std::int32_t(1) << i;
            EXPECT_FALSE(avel::no_bits(x));
        }

        for (std::uint32_t i = 0; i < sizeof(std::uint32_t) * CHAR_BIT - 1; ++i) {
            std::int32_t x = std::uint32_t(0xffffffff) >> (sizeof(std::uint32_t) * CHAR_BIT - 1 - i);
            EXPECT_FALSE(avel::no_bits(x));
        }
    }

    TEST(Scalar32i, No_bits_random) {
        for (std::int32_t i = 0; i < iterations; ++i) {
            std::int32_t x = random32u();

            bool expected = (x == 0x00000000);
            bool observed = avel::no_bits(x);

            EXPECT_EQ(expected, observed);
        }
    }

    TEST(Scalar32i, No_bits_of_preselected) {
        EXPECT_TRUE(avel::no_bits_of(std::int32_t(0x00000000), std::int32_t(0x00000000)));

        for (std::int32_t i = 0; i < sizeof(std::int32_t) * CHAR_BIT; ++i) {
            std::int32_t x = std::int32_t(1) << i;
            EXPECT_TRUE(avel::no_bits_of(x, std::int32_t(0x00000000)));
        }

        EXPECT_TRUE(avel::no_bits_of(std::int32_t(0x00000000), std::int32_t(0xffffffff)));

        for (std::uint32_t i = 0; i < sizeof(std::uint32_t) * CHAR_BIT - 1; ++i) {
            std::int32_t x = std::uint32_t(0xffffffff) >> (sizeof(std::uint32_t) * CHAR_BIT - 1 - i);
            EXPECT_FALSE(avel::no_bits_of(x, std::int32_t(0xffffffff)));
        }
    }

    TEST(Scalar32i, No_bits_of_random) {
        for (std::int32_t i = 0; i < iterations; ++i) {
            std::int32_t x = random32u();
            std::int32_t m = random32u();

            bool expected = true;
            for (std::int32_t j = 0; j < sizeof(std::int32_t) * CHAR_BIT; ++j) {
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

    TEST(Scalar32i, Any_bits_preselected) {
        EXPECT_FALSE(avel::any_bits(std::int32_t(0x00000000)));

        for (std::int32_t i = 0; i < sizeof(std::int32_t) * CHAR_BIT; ++i) {
            std::int32_t x = std::int32_t(1) << i;
            EXPECT_TRUE(avel::any_bits(x));
        }

        for (std::uint32_t i = 0; i < sizeof(std::uint32_t) * CHAR_BIT - 1; ++i) {
            std::int32_t x = std::uint32_t(0xffffffff) >> (sizeof(std::uint32_t) * CHAR_BIT - 1 - i);
            EXPECT_TRUE(avel::any_bits(x));
        }
    }

    TEST(Scalar32i, Any_bits_random) {
        for (std::int32_t i = 0; i < iterations; ++i) {
            std::int32_t x = random32u();

            bool expected = (x != 0x00000000);
            bool observed = avel::any_bits(x);

            EXPECT_EQ(expected, observed);
        }
    }

    TEST(Scalar32i, Any_bits_of_preselected) {
        EXPECT_FALSE(avel::any_bits_of(std::int32_t(0x00000000), std::int32_t(0x00000000)));

        for (std::int32_t i = 0; i < sizeof(std::int32_t) * CHAR_BIT; ++i) {
            std::int32_t x = std::int32_t(1) << i;
            EXPECT_FALSE(avel::any_bits_of(x, std::int32_t(0x00000000)));
        }

        EXPECT_FALSE(avel::any_bits_of(std::int32_t(0x00000000), std::int32_t(0xffffffff)));

        for (std::uint32_t i = 0; i < sizeof(std::uint32_t) * CHAR_BIT - 1; ++i) {
            std::int32_t x = std::uint32_t(0xffffffff) >> (sizeof(std::uint32_t) * CHAR_BIT - 1 - i);
            EXPECT_TRUE(avel::any_bits_of(x, std::int32_t(0xffffffff)));
        }
    }

    TEST(Scalar32i, Any_bits_of_random) {
        for (std::int32_t i = 0; i < iterations; ++i) {
            std::int32_t x = random32u();
            std::int32_t m = random32u();

            bool expected = false;
            for (std::int32_t j = 0; j < sizeof(std::int32_t) * CHAR_BIT; ++j) {
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

    TEST(Scalar32i, All_bits_preselected) {
        EXPECT_FALSE(avel::all_bits(std::int32_t(0x00000000)));

        for (std::int32_t i = 0; i < sizeof(std::int32_t) * CHAR_BIT; ++i) {
            std::int32_t x = std::int32_t(1) << i;
            EXPECT_FALSE(avel::all_bits(x));
        }

        for (std::uint32_t i = 0; i < sizeof(std::uint32_t) * CHAR_BIT - 1; ++i) {
            std::int32_t x = std::uint32_t(0xffffffff) >> (sizeof(std::uint32_t) * CHAR_BIT - 1 - i);
            EXPECT_FALSE(avel::all_bits(x));
        }

        EXPECT_TRUE(avel::all_bits(std::int32_t(0xffffffff)));
    }

    TEST(Scalar32i, All_bits_random) {
        for (std::int32_t i = 0; i < iterations; ++i) {
            std::int32_t x = random32u();

            bool expected = (x == 0xffffffff);
            bool observed = avel::all_bits(x);

            EXPECT_EQ(expected, observed);
        }
    }

    TEST(Scalar32i, All_bits_of_preselected) {
        EXPECT_TRUE(avel::all_bits_of(std::int32_t(0x00000000), std::int32_t(0x00000000)));

        for (std::int32_t i = 0; i < sizeof(std::int32_t) * CHAR_BIT; ++i) {
            std::int32_t x = std::int32_t(1) << i;
            EXPECT_TRUE(avel::all_bits_of(x, std::int32_t(0x00000000)));
        }

        for (std::uint32_t i = 0; i < sizeof(std::uint32_t) * CHAR_BIT - 1; ++i) {
            std::int32_t x = std::uint32_t(0xffffffff) >> (sizeof(std::uint32_t) * CHAR_BIT - 1 - i);
            EXPECT_FALSE(avel::all_bits_of(x, std::int32_t(0xffffffff)));
        }

        EXPECT_TRUE(avel::all_bits_of(std::int32_t(0xffffffff), std::int32_t(0xffffffff)));
    }

    TEST(Scalar32i, All_bits_of_random) {
        for (std::int32_t i = 0; i < iterations; ++i) {
            std::int32_t x = random32u();
            std::int32_t m = random32u();

            bool expected = true;
            for (std::int32_t j = 0; j < sizeof(std::int32_t) * CHAR_BIT; ++j) {
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

#endif //AVEL_SCALAR32I_TESTS_HPP
