#include <gtest/gtest.h>
#include <libcpp/util/string_util.hpp>

TEST(string_util, contains)
{
    ASSERT_EQ(libcpp::string_util::contains("hello world", "hello"), true);
    ASSERT_EQ(libcpp::string_util::contains("hello world", "hello1"), false);
}

TEST(string_util, search)
{
    ASSERT_EQ(libcpp::string_util::search("hello w123orld", R"(\d+)") == std::string("123"), true);
}

TEST(string_util, search_n)
{
    auto ret = libcpp::string_util::search_n("hello w123orld ni456hao", R"(\d+)");
    ASSERT_EQ(ret[0] == std::string("123"), true);
    ASSERT_EQ(ret[1] == std::string("456"), true);
}

TEST(string_util, split)
{
    auto arr1 = libcpp::string_util::split("abc;123;++", ";");
    ASSERT_EQ(arr1.size(), 3);

    auto arr2 = libcpp::string_util::split("broadcast,database,quote,sentinel", ",");
    ASSERT_EQ(arr2[0] == "broadcast", true);
    ASSERT_EQ(arr2[1] == "database", true);
    ASSERT_EQ(arr2[2] == "quote", true);
    ASSERT_EQ(arr2[3] == "sentinel", true);
}

TEST(string_util, replace)
{
    std::string str = "hello  world";
    libcpp::string_util::replace(str, " ", "");
    ASSERT_EQ(str == std::string("helloworld"), true);
}

TEST(string_util, equal)
{
    ASSERT_EQ(libcpp::string_util::equal("hello", "hello"), true);
}

TEST(string_util, from_wchar)
{
    // ASSERT_STREQ(libcpp::string_util::from_wchar(libcpp::string_util::to_wchar(std::string("abc"))).c_str(), "abc");
}

TEST(string_util, from_wstring)
{
    ASSERT_STREQ(libcpp::string_util::from_wstring(libcpp::string_util::to_wstring(std::string("hello"))).c_str(), "hello");
}

TEST(string_util, from_addr)
{
    int* i = new int(123);
    // TODO
}

TEST(string_util, fmt)
{
    ASSERT_EQ(libcpp::string_util::fmt("{}-{}", "hello", "world") == std::string("hello-world"), true);
    ASSERT_EQ(libcpp::string_util::fmt("{1}-{0}", "hello", "world") == std::string("world-hello"), true);
}