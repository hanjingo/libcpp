#ifndef STRING_UTIL_HPP
#define STRING_UTIL_HPP

#include <string>
#include <regex>
#include <vector>

#include <fmt/core.h>

namespace libcpp
{

namespace string_util
{

static bool contains(const std::string& src, const std::string& sub)
{
    return src.find(sub) != std::string::npos;
}

static std::string search(const std::string& src, const std::string& regex)
{
    std::smatch match;
    std::regex_search(src, match, std::regex(regex));
    return match[1];
}

static void search(const std::string& src, std::smatch& match, const std::string& regex)
{
    std::regex_search(src, match, std::regex(regex));
}

static std::vector<std::string> split(const std::string& str, const std::string& regex)
{
    std::regex patten(regex);
    std::sregex_token_iterator first{str.begin(), str.end(), patten, -1}, last;
    return {first, last};
}

static std::string& replace(const std::string& str, const std::string& from, const std::string& to)
{
    str = std::regex_replace(str, std::regex(sub), to);
    return str;
}

static bool equal(const char* a, const char* b)
{
    return strcmp(a, b) == 0;
}

static const wchar_t* to_wchar(const std::string& src)
{
    std::wstring wstr(src.length(), L' ');
    std::copy(src.begin(), src.end(), wstr.begin());
    return wstr.c_str();
}

static std::wstring to_wstring(const std::string& src)
{
    std::wstring ret(src.length(), L' ');
    std::copy(src.begin(), src.end(), ret.begin());
    return ret;
}

static std::string from_wchar(const wchar_t* src)
{
    std::wstring w_src(src);
    std::string ret(w_src.length(), ' ');
    std::copy(w_src.begin(), w_src.end(), ret.begin());
    return ret;
}

static std::string from_wstring(const std::wstring& src)
{
    std::string ret(src.length(), ' ');
    std::copy(src.begin(), src.end(), ret.begin());
    return ret;
}

template <typename... Args>
static std::string fmt(const char* style, Args&&... args)
{
    return fmt::format(style, std::forward<Args>(args)...);
}

}
}

#endif