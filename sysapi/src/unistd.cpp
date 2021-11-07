#include "unistd.hpp"
#include "detail.hpp"

namespace sysapi
{

void close(int fd)
{
    const int result = ::close(fd);
    if (result == -1)
        detail::throwErrnoSystemError("sysapi::close");
}

}