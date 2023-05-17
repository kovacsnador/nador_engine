#ifndef __NADOR_THREAD_UTILS_H__
#define __NADOR_THREAD_UTILS_H__

#ifdef PLATFORM_WINDOWS
#include <windows.h>
#else
#include <pthread.h>
#endif

#include <cstdint>

namespace nador
{
#ifdef PLATFORM_WINDOWS
    const DWORD MS_VC_EXCEPTION = 0x406D1388;
#pragma pack(push, 8)
    typedef struct tagTHREADNAME_INFO
    {
        DWORD  dwType; // Must be 0x1000.
        LPCSTR szName; // Pointer to name (in user addr space).
        DWORD  dwThreadID; // Thread ID (-1=caller thread).
        DWORD  dwFlags; // Reserved for future use, must be zero.
    } THREADNAME_INFO;
#pragma pack(pop)
    void SetThreadName(const char* threadName)
    {
        // Set the thread name using the Windows API

        THREADNAME_INFO info;
        info.dwType     = 0x1000;
        info.szName     = threadName;
        info.dwThreadID = -1;
        info.dwFlags    = 0;

        RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), reinterpret_cast<ULONG_PTR*>(&info));
    }
#else

    void SetThreadName(const char* threadName)
    {
        // Set the thread name using the pthread library
        pthread_setname_np(pthread_self(), threadName);
    }

#endif
} // namespace nador

#endif //!__NADOR_THREAD_UTILS_H__