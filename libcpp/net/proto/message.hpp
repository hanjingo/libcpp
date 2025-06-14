#ifndef MESSAGE_HPP
#define MESSAGE_HPP

namespace libcpp
{
    class message
    {
    public:
        virtual std::size_t size() = 0;
        virtual std::size_t encode(char* buf, const std::size_t len) = 0;
        virtual std::size_t decode(const char* buf, const std::size_t len)  = 0;
    };
}

#endif