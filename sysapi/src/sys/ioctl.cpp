#include "sys/ioctl.hpp"
#include "detail.hpp"

namespace sysapi::detail
{
    void throwIoctlError()
    {
        throwErrnoSystemError("sysapi::ioctl");
    }
};
