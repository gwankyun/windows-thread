module;
#include "common.h"

export module mutex;
import common;
import mutex.detail;
import std;
import critical_section;

EXPORT namespace lite
{
    class mutex
    {
      public:
        typedef detail::mutex::native_handle_type native_handle_type;
        mutex() NOEXCEPT
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

        bool try_lock()
        {
            return detail::mutex::try_lock(m_mutex);
        }

        native_handle_type native_handle()
        {
            return m_mutex.handle;
        }

        critical_section& get_critical_section()
        {
            return m_critical_section;
        }

      private:
        detail::mutex::type m_mutex;
        critical_section m_critical_section;
        NO_COPY_ASSIGN(mutex);
    };

    template <typename Mutex>
    class lock_guard
    {
      public:
        typedef Mutex mutex_type;
        explicit lock_guard(mutex_type& m) : m_mutex(m)
        {
            m_mutex.lock();
        }

        ~lock_guard()
        {
            m_mutex.unlock();
        }

      private:
        mutex_type& m_mutex;
        NO_COPY_ASSIGN(lock_guard);
    };

    template <typename Mutex>
    class unique_lock
    {
      public:
        typedef Mutex mutex_type;
        unique_lock() NOEXCEPT : m_mutex(NULLPTR)
        {
        }

        explicit unique_lock(mutex_type& m) : m_mutex(&m)
        {
            lock();
        }

        ~unique_lock()
        {
            if (m_mutex != NULLPTR)
            {
                unlock();
            }
        }

        void lock()
        {
            m_mutex->lock();
        }

        void unlock()
        {
            m_mutex->unlock();
        }

        bool try_lock()
        {
            m_mutex->try_lock();
        }

        mutex_type* mutex() const 
        {
            return m_mutex;
        }

        void swap(unique_lock& other) NOEXCEPT
        {
            std::swap(this->m_mutex, other->m_mutex);
        }

        mutex_type* release() NOEXCEPT
        {
            mutex_type* temp = m_mutex;
            m_mutex = NULLPTR;
            return temp;
        }

        bool owns_lock() const NOEXCEPT
        {
            if (m_mutex == NULLPTR)
            {
                return false;
            }
            return m_mutex.try_lock();
        }

        explicit operator bool() const NOEXCEPT
        {
            return owns_lock();
        }

      private:
        mutex_type* m_mutex;
        NO_COPY_ASSIGN(unique_lock);
    };

} // namespace lite
