#include "fcntl.hpp"
#include "detail.hpp"

namespace sysapi
{

int open(const char* pathname, int flags)
{
    const int result = ::open(pathname, flags);
    if (result == -1)
        detail::throwErrnoSystemError("fcntl::open");
    return result;
}

}