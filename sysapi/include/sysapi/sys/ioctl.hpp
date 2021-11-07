#pragma once
#include <sys/ioctl.h>
#include <utility>

namespace sysapi
{
namespace detail
{
    [[gnu::cold, noreturn]] void throwIoctlError();
}

template<typename... Args>
int ioctl(int fd, unsigned long request, Args&&... args)
{
    const int result = ::ioctl(fd, request, std::forward<Args>(args)...);
    if (result == -1)
        detail::throwIoctlError();
    return result;
}

}