#ifndef CPU_H
#define CPU_H

#include <stdlib.h>

#if defined(_WIN32)
#include <windows.h>
#elif __APPLE__
#include <sys/param.h>
#include <sys/sysctl.h>
#elif __linux__
#include <unistd.h>
#include <pthread.h>
#else
#pragma warning unknown OS, some function will be disabled
#endif

// See Also: https://caiorss.github.io/C-Cpp-Notes/Preprocessor_and_Macros.html
#if defined(_M_X86) || defined(_M_I86) ||defined( _M_IX86) || defined(i386) || defined(__i386) || defined(__i386__) || defined(__IA32__)
#define ARCH "X86"
#elif defined(_M_X64) || defined(__x86_64__) || defined(__x86_64)
#define ARCH "X64"
#elif defined(_M_AMD64) || defined(__amd64__) || defined(__amd64)
#define ARCH "AMD64"
#elif defined(_M_ARM) || defined(_M_ARMT) || defined(__arm__)
#define ARCH "ARM"
#elif defined(__aarch64__)
#define ARCH "ARM64"
#else
#define ARCH "UNKNOW ARCH"
#endif

static unsigned int cpu_cores()
{
#if defined(_WIN32)
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return sysinfo.dwNumberOfProcessors;
#elif __APPLE__
    int nm[2];
    size_t len = 4;
    unsigned int count;

    nm[0] = CTL_HW;
    nm[1] = HW_AVAILCPU;
    sysctl(nm, 2, &count, &len, NULL, 0);

    if (count < 1) {
        nm[1] = HW_NCPU;
        sysctl(nm, 2, &count, &len, NULL, 0);
        if (count < 1)
            count = 1;
    }
    return count;
#elif __linux__
    return sysconf(_SC_NPROCESSORS_ONLN);
#else
    return std::thread::hardware_concurrency();
#endif
}

static bool cpu_bind(const unsigned int core)
{
#if defined(__windows__)
    HANDLE hThread = GetCurrentThread();
    DWORD_PTR mask = SetThreadAffinityMask(hThread, (DWORD_PTR)(1LLU << i));
    return (mask != 0);
#elif __linux__
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(core, &mask);
    return (pthread_setaffinity_np(pthread_self(), sizeof(mask), &mask) >= 0);
#else
    return false;
#endif
}


#endif