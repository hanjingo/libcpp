#include <gtest/gtest.h>
#include <libcpp/crypto/md5.hpp>

TEST(md5, calc)
{
    ASSERT_STREQ(
        libcpp::md5::calc("hehehunanchina@live.com").c_str(),
        "2da6acfccab34c8ac05295d0f4262b84"
    );

    ASSERT_STREQ(
        libcpp::md5::calc("hehehunanchina@live.com", libcpp::md5::upper_case).c_str(),
        "2DA6ACFCCAB34C8AC05295D0F4262B84"
    );
}