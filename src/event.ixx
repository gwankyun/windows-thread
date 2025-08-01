module;
#include <windows.h>
#include "common.h"

export module event;
import common;
import std;
import event.detail;

EXPORT namespace lite
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

        wait_status_type wait(std::uint32_t _milliseconds = infinite::value)
        {
            return detail::event::wait(m_event, _milliseconds);
        }

      private:
        detail::event::type m_event;
        NO_COPY_ASSIGN(event);
    };
} // namespace lite
