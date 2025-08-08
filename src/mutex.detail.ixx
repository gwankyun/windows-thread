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
        lite::Handle handle;
    };

    bool create(type & _t)
    {
        HANDLE handle = ::CreateMutex( // 返回互斥柄
            NULL,                      // 不能被子進程承繼
            FALSE,                     // 是否立即被當前線程擁有
            NULL                       // 名稱
        );
        _t.handle.reset(handle);
        return true;
    }

    void close(type & _t)
    {
        _t.handle.reset();
    }

    void lock(type & _t)
    {
        lite::wait(_t.handle.get());
    }

    bool try_lock(type & _t)
    {
        return lite::wait(_t.handle.get(), 0) == lite::wait_status::ready;
    }

    void unlock(type & _t)
    {
        ReleaseMutex(_t.handle.get());
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
