#pragma once
#include "common.hpp"

namespace lite
{
    class mutex
    {
    public:
        typedef HANDLE native_handle_type;

        CONSTEXPR_DYNAMIC_ALLOC mutex() NOEXCEPT
            : m_handle(CreateMutex(NULL, FALSE, ""))
        {
            if (m_handle == NULL)
            {
                SPDLOG_INFO("{} {}", __func__, GetLastError());
            }
        }

        ~mutex()
        {
        }

        void lock()
        {
            WaitForSingleObject(m_handle, INFINITE);
        }

        void unlock()
        {
            ReleaseMutex(m_handle);
        }

        native_handle_type native_handle()
        {
            return m_handle;
        }

    private:
        mutex(const mutex&): m_handle(NULL) {};
        mutex& operator=(const mutex&) { return *this; }

        native_handle_type m_handle;
    };

    template<typename Mutex>
    class lock_guard
    {
    public:
        typedef Mutex mutex_type;

        explicit lock_guard(mutex_type& mutex)
            : m_mutex(mutex)
        {
            m_mutex.lock();
        }

        ~lock_guard()
        {
            m_mutex.unlock();
        }

    private:
        lock_guard(const lock_guard&) {}
        lock_guard& operator=(const lock_guard&) { return *this; }

        mutex_type& m_mutex;
    };
}
