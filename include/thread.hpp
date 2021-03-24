#pragma once
#include "common.hpp"

namespace lite
{
    class thread
    {
    public:
        typedef HANDLE native_handle_type;

        class id
        {
        public:
            id()
            {
            }

            ~id()
            {
            }

            friend class thread;
            friend bool operator==(thread::id lhs, thread::id rhs) NOEXCEPT;
            friend bool operator!=(thread::id lhs, thread::id rhs) NOEXCEPT;
            friend bool operator<(thread::id lhs, thread::id rhs) NOEXCEPT;
            friend bool operator<=(thread::id lhs, thread::id rhs) NOEXCEPT;
            friend bool operator>(thread::id lhs, thread::id rhs) NOEXCEPT;
            friend bool operator>=(thread::id lhs, thread::id rhs) NOEXCEPT;

        private:
            DWORD m_id;
        };

        thread() NOEXCEPT
        {
        }

        template<typename F, typename Arg>
        explicit thread(F f, Arg arg)
        {
            m_data.base = new Derived(f, arg);
            if (m_data.base != NULLPTR)
            {
                m_data.handle = CreateThread(NULLPTR, 0, f, arg, 0, &(m_id.m_id));
            }
        }

        ~thread()
        {
            m_data.close();
        }

        void swap(thread& other) NOEXCEPT
        {
            Data temp = m_data;
            m_data = other.m_data;
            other.m_data = temp;
        }

        bool joinable() const NOEXCEPT
        {
            return true;
        }

        void join()
        {
            WaitForSingleObject(native_handle(), INFINITE);
        }

        id get_id() const NOEXCEPT
        {
            return m_id;
        }

        native_handle_type native_handle()
        {
            return m_data.handle;
        }

    private:
        thread(const thread&) {};
        thread& operator=(const thread&) { return *this; };

        struct Base
        {
            Base() {};
            virtual ~Base() {};
            //virtual void run() = 0;
        };

        template<typename F, typename Arg>
        struct Derived : public Base
        {
            explicit Derived(F f, Arg arg)
                : m_f(f)
                , m_arg(arg)
            {
            }

            //void run() OVERRIDE
            //{
            //    m_f(m_arg);
            //}

            F m_f;
            Arg m_arg;
        };

        struct Data
        {
            Data()
                : base(NULLPTR)
                , handle(NULLPTR)
            {
            }
            ~Data() {}
            void close()
            {
                if (handle != NULLPTR)
                {
                    CloseHandle(handle);
                    handle = NULLPTR;
                }
                if (base != NULLPTR)
                {
                    delete base;
                    base = NULLPTR;
                }
            }
            Base* base;
            native_handle_type handle;
        };

        Data m_data;
        id m_id;
    };

    inline bool operator==(thread::id lhs, thread::id rhs) NOEXCEPT
    {
        return lhs.m_id == lhs.m_id;
    }

    inline bool operator!=(thread::id lhs, thread::id rhs) NOEXCEPT
    {
        return lhs.m_id != lhs.m_id;
    }

    inline bool operator<(thread::id lhs, thread::id rhs) NOEXCEPT
    {
        return lhs.m_id < lhs.m_id;
    }

    inline bool operator<=(thread::id lhs, thread::id rhs) NOEXCEPT
    {
        return lhs.m_id <= lhs.m_id;
    }

    inline bool operator>(thread::id lhs, thread::id rhs) NOEXCEPT
    {
        return lhs.m_id > lhs.m_id;
    }

    inline bool operator>=(thread::id lhs, thread::id rhs) NOEXCEPT
    {
        return lhs.m_id >= lhs.m_id;
    }
}
