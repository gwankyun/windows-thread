module;
#include <windows.h>
#include "common.h"

export module thread.detail;
import common;

EXPORT namespace detail::thread
{
    struct Base
    {
        Base() {};
        virtual ~Base() {};
        virtual void run() = 0;
    };

    template <typename F>
    struct Derived : public Base
    {
        explicit Derived(F _f) : f(_f)
        {
        }
        ~Derived()
        {
        }

        void run() OVERRIDE
        {
            f();
        }

      private:
        F f;
    };

    struct function
    {
        template <typename Fn>
        function(Fn _fn) : base(new Derived<Fn>(_fn))
        {
        }

        ~function() {};

        // 线程函数
        static DWORD WINAPI start(LPVOID lpParam)
        {
            // 将传入的参数转换为function指针
            function* self = static_cast<function*>(lpParam);
            if (self != NULLPTR)
            {
                // 调用存储的可调用对象
                if (self->base != NULLPTR)
                {
                    self->base->run();
                }
                return 0;
            }
            // 添加返回值
            return 0;
        }

      private:
        Base* base;
    };

    struct type
    {
        type() : handle(NULLPTR), id(-1), fn(NULLPTR)
        {
        }
        ~type()
        {
            if (fn != NULLPTR)
            {
                delete fn;
            }
        }

        HANDLE handle; // NULLPTR
        DWORD id;      // -1
        function* fn;  // NULLPTR

    private:
        NO_COPY_ASSIGN(type);
    };

    template <typename Fn>
    bool create(type& _t, Fn _fn)
    {
        _t.fn = new function(_fn);

        // 创建线程
        _t.handle = CreateThread(NULLPTR,         // 默认安全属性
                                 0,               // 默认栈大小
                                 function::start, // 线程函数
                                 _t.fn,           // 传递给线程函数的参数
                                 0,               // 默认创建标志
                                 &_t.id           // 线程 ID
        );
        if (_t.handle == NULLPTR)
        {
            return false;
        }
        return true;
    }

    void join(type& _t)
    {
        // 等待线程结束
        lite::wait(_t.handle);

        // 关闭线程句柄
        CloseHandle(_t.handle);
    }

    typedef HANDLE native_handle_type;
    typedef DWORD id;
} // namespace detail::thread
