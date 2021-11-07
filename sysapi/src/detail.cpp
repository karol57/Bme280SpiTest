#include "detail.hpp"
#include <cerrno>
#include <system_error>

namespace sysapi::detail
{

void throwErrnoSystemError(const char* what_arg)
{
    throw std::system_error(errno, std::generic_category(), what_arg);
}

}