#ifndef SIZE_HPP
#define SIZE_HPP

namespace libcpp
{
namespace types
{

using size = long long;

static const size BYTE = 1;
static const size KB   = BYTE * 1024;
static const size MB   = KB * 1024;
static const size GB   = MB * 1024;
static const size TB   = GB * 1024;

}
}

#endif