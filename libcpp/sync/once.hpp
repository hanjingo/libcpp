#ifndef ONCE_HPP
#define ONCE_HPP

#include <mutex>

namespace libcpp
{
namespace sync
{

}
}

#define ONCE(cmd) \
    static std::once_flag __func__; \
    std::call_once(__func__, [&](){cmd}); \

#endif // ONCE_HPP