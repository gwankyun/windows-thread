#pragma once
#include "common.hpp"

namespace lite
{
    class thread
    {
    public:
        typedef HANDLE native_handle_type;

        thread() NOEXCEPT
        {
        }

        template<typename F, typename Arg>
        explicit thread(F f, Arg arg)
        {
            m_data.base = new Derived(f, arg);
            if (m_data.base != NULLPTR)
            {
                m_data.handle = CreateThread(NULL, 0, f, arg, 0, NULL);
            }
        }

        ~thread()
        {
            if (native_handle() != NULL)
            {
                CloseHandle(m_data.handle);
                m_data.handle = NULL;
            }
            if (m_data.base != NULLPTR)
            {
                delete m_data.base;
                m_data.base = NULLPTR;
            }
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
                , handle(NULL)
            {
            }
            ~Data() {}
            Base* base;
            native_handle_type handle;
        };

        Data m_data;
    };

}
