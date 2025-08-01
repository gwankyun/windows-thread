module;
#include "common.h"

export module thread;
import std;
import common;
import thread.detail;

EXPORT namespace lite
{
    class thread
    {
      public:
        typedef detail::thread::native_handle_type native_handle_type;

        class id
        {
          public:
            detail::thread::id value;
        };

        thread() NOEXCEPT : hasInit(false)
        {
        }

        template <typename Fn>
        thread(Fn _fn)
        {
            detail::thread::create(handle, _fn);
            hasInit = true;
        }

        ~thread()
        {
        }

        bool joinable() const NOEXCEPT
        {
            return hasInit;
        }

        void join()
        {
            detail::thread::join(handle);
        }

        void detach()
        {
        }

        native_handle_type native_handle()
        {
            return handle.handle;
        }

        thread::id get_id() const NOEXCEPT
        {
            thread::id i;
            i.value = handle.id;
            return i;
        }

      private:
        detail::thread::type handle;
        bool hasInit;
        NO_COPY_ASSIGN(thread);
    };

    class jthread
    {
      public:
        typedef thread::native_handle_type native_handle_type;
        typedef thread::id id;

        jthread() {};

        template <typename Fn>
        jthread(Fn _fn) : m_thread(_fn)
        {
        }

        ~jthread()
        {
            if (m_thread.joinable())
            {
                m_thread.join();
            }
        }

        bool joinable() const NOEXCEPT
        {
            return m_thread.joinable();
        }

        void join()
        {
            m_thread.join();
        }

        native_handle_type native_handle()
        {
            return m_thread.native_handle();
        }

        thread::id get_id() const NOEXCEPT
        {
            return m_thread.get_id();
        }

      private:
        thread m_thread;
        NO_COPY_ASSIGN(jthread);
    };
} // namespace lite
