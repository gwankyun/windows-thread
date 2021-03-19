#include <Windows.h>
#if defined(HAS_SPDLOG)
#  define SPDLOG_ACTIVE_LEVEL SPDLOG_INFO
#  include <spdlog/spdlog.h>
#else
#  define SPDLOG_INFO(...)
#endif // defined(HAS_SPDLOG)
#include <iostream>
#include <string>
#include <cstring>

#if defined(USE_LITE)
#  include <thread.hpp>
#  include <mutex.hpp>
#else
#  include <thread>
#  include <mutex>
#endif // defined(USE_LITE)

#if !defined(THREAD)
#  if defined(USE_LITE)
#    define THREAD lite::thread
#else
#    define THREAD std::thread
#  endif // defined(USE_LITE)
#endif // !THREAD

#if !defined(MUTEX)
#  if defined(USE_LITE)
#    define MUTEX lite::mutex
#else
#    define MUTEX std::mutex
#  endif // defined(USE_LITE)
#endif // !MUTEX

#if !defined(LOCK_GUARD)
#  if defined(USE_LITE)
#    define LOCK_GUARD lite::lock_guard
#else
#    define LOCK_GUARD std::lock_guard
#  endif // defined(USE_LITE)
#endif // !MUTEX

MUTEX g_mutex;

struct RunData
{
    bool finished = false;
};

DWORD WINAPI run(LPVOID param)
{
    for (size_t i = 0; i < 10; i++)
    {
        LOCK_GUARD<MUTEX> lock(g_mutex);
        SPDLOG_INFO("{}", __func__);
    }

    LOCK_GUARD<MUTEX> lock(g_mutex);
    RunData* data = static_cast<RunData*>(param);
    if (data != NULL)
    {
        data->finished = true;
    }

    return 0;
}

int main()
{
    RunData data;
    THREAD runThread(run, &data);

    bool finished = false;
    while (!finished)
    {
        LOCK_GUARD<MUTEX> lock(g_mutex);
        finished = data.finished;
        SPDLOG_INFO("{}", __func__);
        Sleep(100);
    }

    if (runThread.joinable())
    {
        runThread.join();
    }

    return 0;
}
