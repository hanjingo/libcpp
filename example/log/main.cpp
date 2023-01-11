#include <iostream>

#include <libcpp/log/log.hpp>

using namespace libcpp::math;

int main(int argc, char* argv[])
{
    Logger::Instance()->Info<std::string>("info:%s;", "libcpp");
}