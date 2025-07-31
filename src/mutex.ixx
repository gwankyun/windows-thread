module;
#include <windows.h>

export module mutex;
import common;

export namespace detail::mutex
{
    struct type
    {
        HANDLE handle;
    };

    bool create(type& _t)
    {
        _t.handle = CreateMutex(NULL, FALSE, NULL);
        return true;
    }

    void close(type& _t)
    {
        CloseHandle(_t.handle);
    }

    void lock(type& _t)
    {
        //WaitForSingleObject(_t.handle, INFINITE);
        lite::wait(_t.handle);
    }

    void unlock(type& _t)
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
    };
} // namespace detail::mutex

export namespace lite
{
    class mutex
    {
      public:
        mutex()
        {
            detail::mutex::create(m_mutex);
        }
        ~mutex()
        {
            detail::mutex::close(m_mutex);
        }

        void lock()
        {
            detail::mutex::lock(m_mutex);
        }

        void unlock()
        {
            detail::mutex::unlock(m_mutex);
        }

      private:
        detail::mutex::type m_mutex;
    };

    template <typename Mutex>
    class lock_guard
    {
      public:
        typedef Mutex mutex_type;
        explicit lock_guard(mutex_type& m)
        {
            m_mutex.lock();
        }

        ~lock_guard()
        {
            m_mutex.unlock();
        }

      private:
        mutex_type m_mutex;
    };
} // namespace lite
