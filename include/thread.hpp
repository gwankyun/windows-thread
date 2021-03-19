#pragma once
#include "common.hpp"

namespace lite
{
    class thread
    {
    public:
        typedef HANDLE native_handle_type;

        thread()
            : m_base(NULLPTR)
            , m_handle(NULL)
        {
        }

        template<typename F, typename Arg>
        explicit thread(F f, Arg arg)
            : m_base(new Derived(f, arg))
            , m_handle(NULL)
        {
            if (m_base != NULL)
            {
                m_handle = CreateThread(NULL, 0, f, arg, 0, NULL);
            }
        }

        ~thread()
        {
            if (m_handle != NULL)
            {
                CloseHandle(m_handle);
                m_handle = NULL;
            }
            if (m_base != NULLPTR)
            {
                delete m_base;
                m_base = NULLPTR;
            }
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
            return m_handle;
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

        Base* m_base;
        native_handle_type m_handle;
    };

}
