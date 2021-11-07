add_library(sysapi
    src/detail.cpp
    src/fcntl.cpp
    src/unistd.cpp
    src/sys/ioctl.cpp)
target_include_directories(sysapi INTERFACE include/)
target_include_directories(sysapi PRIVATE include/sysapi src/include)