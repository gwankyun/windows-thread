module;
#include "common.h"
#include <windows.h>

export module event.detail;
import common;
import std;

EXPORT namespace detail_event
{
    struct type
    {
        //HANDLE handle;
        lite::Handle handle;
    };

    void create(type & _t)
    {
        HANDLE handle = ::CreateEvent( // 返回事件句柄
            NULL,                      // 子進程不能繼承
            TRUE,                      // 事件手動重置
            FALSE,                     // 初始為未通知狀態
            NULL                       // 對象名稱
        );
        _t.handle.reset(handle);
    }

    void close(type & _t)
    {
        //::CloseHandle(_t.handle);
        _t.handle.reset();
    }

    lite::wait_status_type wait(type & _t, std::uint32_t _milliseconds = lite::infinite::value)
    {
        return lite::wait(_t.handle.get(), _milliseconds);
        //return _t.handle->wait(_milliseconds);
    }

    bool set(type & _t)
    {
        return ::SetEvent(_t.handle.get()) == TRUE;
    }

    bool reset(type & _t)
    {
        return ::ResetEvent(_t.handle.get()) == TRUE;
    }
} // namespace detail::event

EXPORT namespace detail
{
    namespace event = detail_event;
}
