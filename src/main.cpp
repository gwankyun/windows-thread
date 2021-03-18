#include <iostream>
#include <string>
#include <cstring>
#include <Windows.h>

inline std::string getFunc(const std::string& func)
{
    char str[10];
    memset(str, ' ', sizeof(str));
    str[9] = '\0';
    memcpy_s(str, sizeof(str) - 1, func.c_str(), sizeof(str) - 1 < func.size() ? sizeof(str) - 1 : func.size());
    return std::string(str);
}

inline std::string getLine(int line)
{
    char str[20];
    memset(str, '\0', sizeof(str));
    sprintf_s(str, sizeof(str), "% 5d", line);
    return std::string(str);
}

#ifndef LOG
#  define LOG() std::cout << "[" << getFunc(__func__) << ":" <<  getLine(__LINE__) << "] "
#endif // !LOG

HANDLE g_mutex = NULL;

struct RunData
{
    bool finished = false;
};

DWORD WINAPI run(LPVOID param)
{
    for (size_t i = 0; i < 10; i++)
    {
        WaitForSingleObject(g_mutex, INFINITE);
        LOG() << i << "\n";
        ReleaseMutex(g_mutex);
    }

    WaitForSingleObject(g_mutex, INFINITE);
    RunData* data = static_cast<RunData*>(param);
    if (data != NULL)
    {
        data->finished = true;
    }
    ReleaseMutex(g_mutex);

    return 0;
}

int main()
{
    g_mutex = CreateMutex(NULL, FALSE, "g_mutex");
    if (g_mutex == NULL)
    {
        return 1;
    }

    RunData data;
    HANDLE runThread = CreateThread(NULL, 0, run, &data, 0, NULL);
    if (runThread == NULL)
    {
        return 1;
    }
    CloseHandle(runThread);

    bool finished = false;
    while (!finished)
    {
        WaitForSingleObject(g_mutex, INFINITE);
        finished = data.finished;
        LOG() << "\n";
        Sleep(100);
        ReleaseMutex(g_mutex);
    }

    return 0;
}
