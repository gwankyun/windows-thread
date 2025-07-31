module;
#include <windows.h>

export module event;
import common;
import std;

namespace detail::event
{
    struct type
    {
        HANDLE handle;
    };

    void create(type& _t)
    {
        _t.handle = CreateEvent(NULL, TRUE, FALSE, NULL);
    }

    void close(type& _t)
    {
        CloseHandle(_t.handle);
    }

    lite::wait_status_type wait(type& _t, std::uint32_t _milliseconds = INFINITE)
    {
        return lite::wait(_t.handle, _milliseconds);
    }

    void set(type& _t)
    {
        SetEvent(_t.handle);
    }

    void reset(type& _t)
    {
        ResetEvent(_t.handle);
    }
} // namespace detail::event

export namespace lite
{
    class event
    {
      public:
        event()
        {
            detail::event::create(m_event);
        }
        ~event()
        {
            detail::event::close(m_event);
        }

        void set()
        {
            detail::event::set(m_event);
        }

        void reset()
        {
            detail::event::reset(m_event);
        }

        wait_status_type wait(std::uint32_t _milliseconds = INFINITE)
        {
            return detail::event::wait(m_event, _milliseconds);
        }

      private:
        detail::event::type m_event;
    };
} // namespace lite
