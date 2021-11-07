#pragma once

namespace sysapi::detail
{
    [[gnu::cold, noreturn]] void throwErrnoSystemError(const char* what_arg);
}