#ifndef AVEL_SCALAR64F_TESTS_HPP
#define AVEL_SCALAR64F_TESTS_HPP

namespace avel_tests {

    using namespace avel;

    //=====================================================
    // General Floating-point Operations
    //=====================================================

    TEST(Scalar64f_tests, fmod_edge_cases) {
        EXPECT_TRUE(compare_common_bytes(avel::fmod(+0.0, 1.0), +0.0));
        EXPECT_TRUE(compare_common_bytes(avel::fmod(-0.0, 1.0), -0.0));

        EXPECT_TRUE(avel::isnan(avel::fmod(1.0, +0.0)));
        EXPECT_TRUE(avel::isnan(avel::fmod(1.0, -0.0)));

        EXPECT_TRUE(avel::isnan(avel::fmod(double(+INFINITY), 1.0)));
        EXPECT_TRUE(avel::isnan(avel::fmod(double(-INFINITY), 1.0)));

        EXPECT_EQ(1.0, avel::fmod(1.0, double(+INFINITY)));
        EXPECT_EQ(1.0, avel::fmod(1.0, double(-INFINITY)));

        EXPECT_TRUE(avel::isnan(avel::fmod(1.0, double(NAN))));
        EXPECT_TRUE(avel::isnan(avel::fmod(double(NAN), 1.0)));
        EXPECT_TRUE(avel::isnan(avel::fmod(double(NAN), double(NAN))));

        EXPECT_TRUE(avel::isnan(avel::fmod(double(+INFINITY), double(+INFINITY))));
        EXPECT_TRUE(avel::isnan(avel::fmod(double(+INFINITY), double(-INFINITY))));
        EXPECT_TRUE(avel::isnan(avel::fmod(double(-INFINITY), double(+INFINITY))));
        EXPECT_TRUE(avel::isnan(avel::fmod(double(-INFINITY), double(-INFINITY))));

        EXPECT_TRUE(avel::isnan(avel::fmod(double(+INFINITY), double(NAN))));
        EXPECT_TRUE(avel::isnan(avel::fmod(double(-INFINITY), double(NAN))));

        EXPECT_TRUE(avel::isnan(avel::fmod(double(NAN), double(+INFINITY))));
        EXPECT_TRUE(avel::isnan(avel::fmod(double(NAN), double(-INFINITY))));
    }

    TEST(Scalar64f_tests, fmod_preselected_simple_cases) {
        EXPECT_EQ(0.5, avel::fmod(1.5, +1.0));
        EXPECT_EQ(0.5, avel::fmod(1.5, -1.0));

        EXPECT_EQ(-0.5, avel::fmod(-1.5, +1.0));
        EXPECT_EQ(-0.5, avel::fmod(-1.5, -1.0));

        EXPECT_EQ(0.0, avel::fmod(4.5, 1.5));
        EXPECT_EQ(0.0, avel::fmod(4.5, 1.5));
    }

    TEST(Scalar64f_tests, fmod_subnormal_inputs) {
        //EXPECT_EQ(0.0, avel::fmod(1.12103877145985365674e-44, 1.40129846432481707092e-45));
        //EXPECT_EQ(0.0, avel::fmod(1.12103877145985365674e-44, 1.40129846432481707092e-45));
        //EXPECT_EQ(1.40129846432481707092e-45, avel::fmod(1.26116861789233536383e-44, 2.80259692864963414185e-45));
        //EXPECT_EQ(0.0f, avel::fmod(1.17549435082228750797e-38f, 5.87747175411143753984e-39f));
        //EXPECT_EQ(0.0f, avel::fmod(1.17549435082228750797e-38f, 2.93873587705571876992e-39f));
        //EXPECT_EQ(7.3468396926392969248e-40, avel::fmod(1.54283633545425235421e-38f, 2.93873587705571876992e-39f));
    }

    TEST(Scalar64f_tests, fmod_high_dynamic_range) {
        //EXPECT_EQ(0.0f, avel::fmod(3.40282346638528859812e+38f, 1.40129846432481707092e-45f));
        //EXPECT_EQ(0.0f, avel::fmod(3.40282346638528859812e+38f, 2.80259692864963414185e-45f));
        //EXPECT_EQ(0.0f, avel::fmod(3.40282346638528859812e+38f, 4.20389539297445121277e-45f));
        //EXPECT_EQ(0.0f, avel::fmod(3.40282346638528859812e+38f, 7.00649232162408535462e-45f));
        //EXPECT_EQ(0.0f, avel::fmod(3.40282346638528859812e+38f, 1.26116861789233536383e-44f));
        //EXPECT_EQ(0.0f, avel::fmod(3.40282346638528859812e+38f, 2.94272677508211584894e-44f));
    }

    TEST(Scalar64f_tests, fmod_randomized_inputs) {
        for (std::size_t i = 0; i < iterations; ++i) {
            double x = random_val<double>();
            double y = random_val<double>();

            double expected = std::fmod(x, y);
            double observed = avel::fmod(x, y);

            EXPECT_EQ(expected, observed);
            if (expected != observed) {
                int breakpoint_dummy = 345;
            }
        }
    }

    TEST(Scalar64f_tests, frac_edge_cases) {
        EXPECT_EQ(+0.0, avel::frac(+0.0));
        EXPECT_EQ(-0.0, avel::frac(-0.0));
        EXPECT_TRUE(std::isnan(avel::frac(+INFINITY)));
        EXPECT_TRUE(std::isnan(avel::frac(-INFINITY)));
        EXPECT_TRUE(std::isnan(avel::frac(NAN)));
    }

    TEST(Scalar64f_tests, frac_random) {
        for (std::uint32_t i = 0; i < iterations; ++i) {
            double x = random_val<double>();

            if (x >= 4503599627370496.0) {
                EXPECT_EQ(0.0f, avel::frac(x));
            } else {
                EXPECT_EQ(x - std::trunc(x), avel::frac(x));
            }
        }
    }



    TEST(Scalar64f_tests, modf_edge_cases) {
        double exp0 = 0.0;
        double sig0 = avel::modf(0.0, &exp0);
        EXPECT_EQ(exp0, 0.0);
        EXPECT_EQ(sig0, 0.0);

        double exp1 = 0.0;
        double sig1 = avel::modf(-0.0, &exp1);
        EXPECT_TRUE(avel::compare_common_bytes(exp1, -0.0));
        EXPECT_TRUE(avel::compare_common_bytes(sig1, -0.0));

        double exp2 = 0.0;
        double sig2 = avel::modf(NAN, &exp2);
        EXPECT_TRUE(std::isnan(exp2));
        EXPECT_TRUE(std::isnan(sig2));

        double exp3 = 0.0;
        double sig3 = avel::modf(+INFINITY, &exp3);
        EXPECT_EQ(exp3, +INFINITY);
        EXPECT_EQ(sig3, 0.0);

        double exp4 = 0.0;
        double sig4 = avel::modf(-INFINITY, &exp4);
        EXPECT_EQ(exp4, -INFINITY);
        EXPECT_EQ(sig4, 0.0);
    }

    TEST(Scalar64f_tests, modf_random) {
        for (std::size_t i = 0; i < iterations; i++) {
            double input = random64f();
            double expected_exp0 = 0.0;
            double expected_sig0 = std::modf(input, &expected_exp0);

            double observed_exp0 = 0.0;
            double observed_sig0 = avel::modf(input, &observed_exp0);

            EXPECT_EQ(expected_exp0, observed_exp0);
            EXPECT_EQ(expected_sig0, observed_sig0);
        }
    }

}

#endif //AVEL_SCALAR64F_TESTS_HPP
