module;
#include <windows.h>
#include "common.h"

export module common;
import std;

EXPORT namespace lite
{
    enum class wait_status
    {
        ready,
        timeout,
        abandoned,
        failed,
        io_completion
    };

    // 為了兼容C++11之前的代碼。
    typedef wait_status wait_status_type;

    struct infinite
    {
        static const std::uint32_t value = INFINITE;
    };
} // namespace lite

export namespace lite
{
    struct IClose
    {
        // 纯虚函数，用于关闭资源
        virtual void close() = 0;
        // 虚析构函数，确保正确释放派生类对象
        virtual ~IClose() = default;
    };

    wait_status_type wait(HANDLE _handle, std::uint32_t _milliseconds = infinite::value)
    {
        DWORD result = ::WaitForSingleObject(_handle, _milliseconds);
        switch (result)
        {
        case WAIT_OBJECT_0:
            return wait_status::ready;
        case WAIT_TIMEOUT:
            return wait_status::timeout;
        case WAIT_ABANDONED:
            return wait_status::abandoned;
        case WAIT_FAILED:
            return wait_status::failed;
        case WAIT_IO_COMPLETION:
            return wait_status::io_completion;
        default:
            break;
        }
        return wait_status::failed;
    }

    class Handle
    {
      public:
        Handle() = default;
        ~Handle() = default;
        wait_status_type wait(std::uint32_t _milliseconds = infinite::value)
        {
            return lite::wait(m_handle, _milliseconds);
        }
        void close()
        {
            ::CloseHandle(m_handle);
        }

      private:
        HANDLE m_handle = NULL;
    };
}; // namespace lite
