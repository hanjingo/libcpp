#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <stdexcept>

namespace libcpp
{

static void throw_if_false(bool target, const char* memo = "false")
{
    if (!target)
        throw std::logic_error(memo);
}

static void throw_if_not_false(bool target, const char* memo = "not false")
{
    if (target)
        throw std::logic_error(memo);
}

template<typename T>
static void throw_if_equal(T target1, T target2, const char* memo = "equal")
{
    if (target1 == target2)
        throw std::logic_error(memo);
}

template<typename T>
static void throw_if_not_equal(T target1, T target2, const char* memo = "not equal")
{
    if (target1 != target2)
        throw std::logic_error(memo);
}

template<typename T>
static void throw_if_empty(T target, const char* memo = "empty")
{
    if (target.empty())
        throw std::logic_error(memo);
}

template<typename T>
static void throw_if_not_empty(T target, const char* memo = "not empty")
{
    if (!target.empty())
        throw std::logic_error(memo);
}

template<typename T>
static void throw_if_null(T target, const char* memo = "null")
{
    if (target == nullptr || target == NULL)
        throw std::logic_error(memo);
}

template<typename T>
static void throw_if_not_null(T target, const char* memo = "not null")
{
    if (target != nullptr && target != NULL)
        throw std::logic_error(memo);
}

template<typename Container, typename T>
static void throw_if_exists(Container container, T target, const char* memo = "already exist")
{
    for (auto &elem : container) {
        if (elem != target)
            continue;

        throw std::logic_error(memo);
    }
}

template<typename Container, typename T>
static void throw_if_not_exists(Container container, T target, const char* memo = "not exist")
{
    for (auto &elem : container) {
        if (elem == target)
            return;
    }

    throw std::logic_error(memo);
}

}

#endif