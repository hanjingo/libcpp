#include <gtest/gtest.h>
#include <libcpp/util/dll.h>

typedef int(*hello)(void);
typedef int(*world)(void);

TEST(dll, dll_open)
{
    ASSERT_EQ(dll_open("./libdll_example.so", DLL_RTLD_LAZY) != NULL, true);
}

TEST(dll, dll_get)
{
    void* example = dll_open("./libdll_example.so", DLL_RTLD_LAZY);
    ASSERT_EQ(example != NULL, true);

    hello fn1 = (hello)dll_get(example, "hello");
    ASSERT_EQ(fn1 != NULL, true);
    ASSERT_EQ(fn1(), 1);

    world fn2 = (world)dll_get(example, "world");
    ASSERT_EQ(fn2 != NULL, true);
    ASSERT_EQ(fn2(), 2);
}

TEST(dll, dll_close)
{
    void* example = dll_open("./libdll_example.so", DLL_RTLD_LAZY);
    ASSERT_EQ(example != NULL, true);

    int ret = dll_close(example);
    ASSERT_EQ(ret == 0, true);
    ASSERT_EQ(example != NULL, true);
}