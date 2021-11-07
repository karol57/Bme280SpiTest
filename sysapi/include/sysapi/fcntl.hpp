#pragma once
#include <fcntl.h>

namespace sysapi
{

[[gnu::nonnull(1)]] int open(const char* pathname, int flags);

}