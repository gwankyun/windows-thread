module;
#include "common.h"
#include <windows.h>

export module mutex.detail;
import common;

EXPORT namespace detail_mutex
{
    typedef HANDLE native_handle_type;

    struct type
    {
        native_handle_type handle;
    };

    bool create(type & _t)
    {
        _t.handle = CreateMutex(NULL, FALSE, NULL);
        return true;
    }

    void close(type & _t)
    {
        CloseHandle(_t.handle);
    }

    void lock(type & _t)
    {
        lite::wait(_t.handle);
    }

    bool try_lock(type & _t)
    {
        return lite::wait(_t.handle, 0) == lite::wait_status::ready;
    }

    void unlock(type & _t)
    {
        ReleaseMutex(_t.handle);
    }

    struct lock_guard
    {
        lock_guard(type& _t) : t(_t)
        {
            lock(t);
        }
        ~lock_guard()
        {
            unlock(t);
        }
        type& t;

      private:
        NO_COPY_ASSIGN(lock_guard);
    };
} // namespace detail::mutex

EXPORT namespace detail
{
    namespace mutex = detail_mutex;
}
