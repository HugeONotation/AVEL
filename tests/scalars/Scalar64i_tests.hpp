#ifndef AVEL_SCALAR64I_TESTS_HPP
#define AVEL_SCALAR64I_TESTS_HPP

namespace avel_tests {

    using namespace avel;

    //=====================================================
    // Comparisons
    //=====================================================

    TEST(Scalar64i, cmp_equal_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint64_t x = random_val<std::uint64_t>();
            std::int64_t  y = random_val<std::int64_t>();

            auto result = cmp_equal(x, y);

            if (x >= 0x8000000000000000ull || y < 0) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x == y);
            }
        }
    }

    TEST(Scalar64i, cmp_equal_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int64_t  x = random_val<std::int64_t>();
            std::uint64_t y = random_val<std::uint64_t>();

            auto result = cmp_equal(x, y);

            if (x < 0 || y >= 0x8000000000000000ull) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x == y);
            }
        }
    }

    TEST(Scalar64i, cmp_not_equal_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint64_t x = random_val<std::uint64_t>();
            std::int64_t  y = random_val<std::int64_t>();

            auto result = cmp_not_equal(x, y);

            if (x < 0 || y >= 0x8000000000000000ull) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x != y);
            }
        }
    }

    TEST(Scalar64i, cmp_not_equal_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int64_t  x = random_val<std::int64_t>();
            std::uint64_t y = random_val<std::uint64_t>();

            auto result = cmp_not_equal(x, y);

            if (x < 0 || y >= 0x8000000000000000ull) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x != y);
            }
        }
    }

    TEST(Scalar64i, cmp_less_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint64_t x = random_val<std::uint64_t>();
            std::int64_t  y = random_val<std::int64_t>();

            auto result = cmp_less(x, y);

            if (x >= 0x8000000000000000ull || y < 0) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x < y);
            }
        }
    }

    TEST(Scalar64i, cmp_less_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int64_t  x = random_val<std::int64_t>();
            std::uint64_t y = random_val<std::uint64_t>();

            auto result = cmp_less(x, y);

            if (x < 0 || y >= 0x8000000000000000ull) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x < y);
            }
        }
    }

    TEST(Scalar64i, Cmp_le_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint64_t x = random_val<std::uint64_t>();
            std::int64_t  y = random_val<std::int64_t>();

            auto result = cmp_less_equal(x, y);

            if (x >= 0x8000000000000000ull || y < 0) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x <= y);
            }
        }
    }

    TEST(Scalar64i, Cmp_le_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int64_t  x = random_val<std::int64_t>();
            std::uint64_t y = random_val<std::uint64_t>();

            auto result = cmp_less_equal(x, y);

            if (x < 0 || y >= 0x8000000000000000ull) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x <= y);
            }
        }
    }

    TEST(Scalar64i, cmp_greater_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint64_t x = random_val<std::uint64_t>();
            std::int64_t  y = random_val<std::int64_t>();

            auto result = cmp_greater(x, y);

            if (x >= 0x8000000000000000ull || y < 0) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x > y);
            }
        }
    }

    TEST(Scalar64i, cmp_greater_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int64_t  x = random_val<std::int64_t>();
            std::uint64_t y = random_val<std::uint64_t>();

            auto result = cmp_greater(x, y);

            if (x < 0 || y >= 0x8000000000000000ull) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x > y);
            }
        }
    }

    TEST(Scalar64i, cmp_greater_equal_u_i) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::uint64_t x = random_val<std::uint64_t>();
            std::int64_t  y = random_val<std::int64_t>();

            auto result = cmp_greater_equal(x, y);

            if (x >= 0x8000000000000000ull || y < 0) {
                EXPECT_TRUE(result);
            } else {
                EXPECT_EQ(result, x >= y);
            }
        }
    }

    TEST(Scalar64i, cmp_greater_equal_i_u) {
        for (std::size_t i = 0; i < iterations; ++i) {
            std::int64_t  x = random_val<std::int64_t>();
            std::uint64_t y = random_val<std::uint64_t>();

            auto result = cmp_greater_equal(x, y);

            if (x < 0 || y >= 0x8000000000000000ull) {
                EXPECT_FALSE(result);
            } else {
                EXPECT_EQ(result, x >= y);
            }
        }
    }

    //=====================================================
    // Integer operations
    //=====================================================

    TEST(Scalar64i, countl_sign_powers_of_two) {
        for (std::size_t i = 0; i < 63; ++i) {
            std::int64_t input = std::int64_t(0x0000000000000001) << (62 - i);
            std::int64_t observed = avel::countl_sign(input);
            std::int64_t expected = i;

            EXPECT_EQ(observed, expected);
        }

        {
            std::int64_t input = 0x0000000000000000;
            std::int64_t observed = avel::countl_sign(input);
            std::int64_t expected = 63;

            EXPECT_EQ(observed, expected);
        }

        for (std::size_t i = 0; i < 63; ++i) {
            std::int64_t input = std::int64_t(0xffffffffffffffff) << i;
            std::int64_t observed = avel::countl_sign(input);
            std::int64_t expected = (63 - i);

            EXPECT_EQ(observed, expected);
        }
    }

    //=====================================================
    // Bit testing
    //=====================================================

    TEST(Scalar64i, No_bits_preselected) {
        EXPECT_TRUE(avel::no_bits(std::int64_t(0x0000000000000000)));

        for (std::int64_t i = 0; i < sizeof(std::int64_t) * CHAR_BIT; ++i) {
            std::int64_t x = std::int64_t(1) << i;
            EXPECT_FALSE(avel::no_bits(x));
        }

        for (std::uint64_t i = 0; i < sizeof(std::uint64_t) * CHAR_BIT - 1; ++i) {
            std::int64_t x = std::uint64_t(0xffffffffffffffff) >> (sizeof(std::uint64_t) * CHAR_BIT - 1 - i);
            EXPECT_FALSE(avel::no_bits(x));
        }
    }

    TEST(Scalar64i, No_bits_random) {
        for (std::int64_t i = 0; i < iterations; ++i) {
            std::int64_t x = random64u();

            bool expected = (x == 0x0000000000000000);
            bool observed = avel::no_bits(x);

            EXPECT_EQ(expected, observed);
        }
    }

    TEST(Scalar64i, No_bits_of_preselected) {
        EXPECT_TRUE(avel::no_bits_of(std::int64_t(0x0000000000000000), std::int64_t(0x0000000000000000)));

        for (std::int64_t i = 0; i < sizeof(std::int64_t) * CHAR_BIT; ++i) {
            std::int64_t x = std::int64_t(1) << i;
            EXPECT_TRUE(avel::no_bits_of(x, std::int64_t(0x0000000000000000)));
        }

        EXPECT_TRUE(avel::no_bits_of(std::int64_t(0x0000000000000000), std::int64_t(0xffffffffffffffff)));

        for (std::uint64_t i = 0; i < sizeof(std::uint64_t) * CHAR_BIT - 1; ++i) {
            std::int64_t x = std::uint64_t(0xffffffffffffffff) >> (sizeof(std::uint64_t) * CHAR_BIT - 1 - i);
            EXPECT_FALSE(avel::no_bits_of(x, std::int64_t(0xffffffffffffffff)));
        }
    }

    TEST(Scalar64i, No_bits_of_random) {
        for (std::int64_t i = 0; i < iterations; ++i) {
            std::int64_t x = random64u();
            std::int64_t m = random64u();

            bool expected = true;
            for (std::int64_t j = 0; j < sizeof(std::int64_t) * CHAR_BIT; ++j) {
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

    TEST(Scalar64i, Any_bits_preselected) {
        EXPECT_FALSE(avel::any_bits(std::int64_t(0x0000000000000000)));

        for (std::int64_t i = 0; i < sizeof(std::int64_t) * CHAR_BIT; ++i) {
            std::int64_t x = std::int64_t(1) << i;
            EXPECT_TRUE(avel::any_bits(x));
        }

        for (std::uint64_t i = 0; i < sizeof(std::uint64_t) * CHAR_BIT - 1; ++i) {
            std::int64_t x = std::uint64_t(0xffffffffffffffff) >> (sizeof(std::uint64_t) * CHAR_BIT - 1 - i);
            EXPECT_TRUE(avel::any_bits(x));
        }
    }

    TEST(Scalar64i, Any_bits_random) {
        for (std::int64_t i = 0; i < iterations; ++i) {
            std::int64_t x = random64u();

            bool expected = (x != 0x0000000000000000);
            bool observed = avel::any_bits(x);

            EXPECT_EQ(expected, observed);
        }
    }

    TEST(Scalar64i, Any_bits_of_preselected) {
        EXPECT_FALSE(avel::any_bits_of(std::int64_t(0x0000000000000000), std::int64_t(0x0000000000000000)));

        for (std::int64_t i = 0; i < sizeof(std::int64_t) * CHAR_BIT; ++i) {
            std::int64_t x = std::int64_t(1) << i;
            EXPECT_FALSE(avel::any_bits_of(x, std::int64_t(0x0000000000000000)));
        }

        EXPECT_FALSE(avel::any_bits_of(std::int64_t(0x0000000000000000), std::int64_t(0xffffffffffffffff)));

        for (std::uint64_t i = 0; i < sizeof(std::uint64_t) * CHAR_BIT - 1; ++i) {
            std::int64_t x = std::uint64_t(0xffffffffffffffff) >> (sizeof(std::uint64_t) * CHAR_BIT - 1 - i);
            EXPECT_TRUE(avel::any_bits_of(x, std::int64_t(0xffffffffffffffff)));
        }
    }

    TEST(Scalar64i, Any_bits_of_random) {
        for (std::int64_t i = 0; i < iterations; ++i) {
            std::int64_t x = random64u();
            std::int64_t m = random64u();

            bool expected = false;
            for (std::int64_t j = 0; j < sizeof(std::int64_t) * CHAR_BIT; ++j) {
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

    TEST(Scalar64i, All_bits_preselected) {
        EXPECT_FALSE(avel::all_bits(std::int64_t(0x0000000000000000)));

        for (std::int64_t i = 0; i < sizeof(std::int64_t) * CHAR_BIT; ++i) {
            std::int64_t x = std::int64_t(1) << i;
            EXPECT_FALSE(avel::all_bits(x));
        }

        for (std::uint64_t i = 0; i < sizeof(std::uint64_t) * CHAR_BIT - 1; ++i) {
            std::int64_t x = std::uint64_t(0xffffffffffffffff) >> (sizeof(std::uint64_t) * CHAR_BIT - 1 - i);
            EXPECT_FALSE(avel::all_bits(x));
        }

        EXPECT_TRUE(avel::all_bits(std::int64_t(0xffffffffffffffff)));
    }

    TEST(Scalar64i, All_bits_random) {
        for (std::int64_t i = 0; i < iterations; ++i) {
            std::int64_t x = random64u();

            bool expected = (x == 0xffffffffffffffff);
            bool observed = avel::all_bits(x);

            EXPECT_EQ(expected, observed);
        }
    }

    TEST(Scalar64i, All_bits_of_preselected) {
        EXPECT_TRUE(avel::all_bits_of(std::int64_t(0x0000000000000000), std::int64_t(0x0000000000000000)));

        for (std::int64_t i = 0; i < sizeof(std::int64_t) * CHAR_BIT; ++i) {
            std::int64_t x = std::int64_t(1) << i;
            EXPECT_TRUE(avel::all_bits_of(x, std::int64_t(0x0000000000000000)));
        }

        for (std::uint64_t i = 0; i < sizeof(std::uint64_t) * CHAR_BIT - 1; ++i) {
            std::int64_t x = std::uint64_t(0xffffffffffffffff) >> (sizeof(std::uint64_t) * CHAR_BIT - 1 - i);
            EXPECT_FALSE(avel::all_bits_of(x, std::int64_t(0xffffffffffffffff)));
        }

        EXPECT_TRUE(avel::all_bits_of(std::int64_t(0xffffffffffffffff), std::int64_t(0xffffffffffffffff)));
    }

    TEST(Scalar64i, All_bits_of_random) {
        for (std::int64_t i = 0; i < iterations; ++i) {
            std::int64_t x = random64u();
            std::int64_t m = random64u();

            bool expected = true;
            for (std::int64_t j = 0; j < sizeof(std::int64_t) * CHAR_BIT; ++j) {
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

#endif //AVEL_SCALAR64I_TESTS_HPP
