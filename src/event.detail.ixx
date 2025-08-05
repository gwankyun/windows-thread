module;
#include <windows.h>
#include "common.h"

export module event.detail;
import common;
import std;

EXPORT namespace detail::event
{
    struct type
    {
        HANDLE handle;
    };

    void create(type& _t)
    {
        _t.handle = ::CreateEvent(NULL, TRUE, FALSE, NULL);
    }

    void close(type& _t)
    {
        ::CloseHandle(_t.handle);
    }

    lite::wait_status_type wait(type& _t, std::uint32_t _milliseconds = lite::infinite::value)
    {
        return lite::wait(_t.handle, _milliseconds);
    }

    void set(type& _t)
    {
        ::SetEvent(_t.handle);
    }

    void reset(type& _t)
    {
        ::ResetEvent(_t.handle);
    }
} // namespace detail::event
