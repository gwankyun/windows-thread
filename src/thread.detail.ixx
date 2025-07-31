module;
#include <windows.h>

#define NULLPTR nullptr
#define NOEXCEPT noexcept
#define OVERRIDE override

export module thread.detail;
import common;

export namespace detail::thread
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
        explicit Derived(F f) : m_f(f)
        {
        }
        ~Derived()
        {
        }

        void run() OVERRIDE
        {
            m_f();
        }

      private:
        F m_f;
    };

    struct function
    {
        template <typename Fn>
        function(Fn _fn) : base(new Derived<Fn>(_fn))
        {
        }

        ~function() {};

        // 线程函数
        static DWORD WINAPI start_routine(LPVOID lpParam)
        {
            // 将传入的参数转换为 thread_function 指针
            auto self = static_cast<function*>(lpParam);
            if (self)
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
        Base* base;
    };

    struct type
    {
        type() = default;
        ~type()
        {
            if (fn != NULLPTR)
            {
                delete fn;
            }
        }
        // 使用 delete 关键字显式删除拷贝构造函数
        type(const type&) = delete;
        // 使用 delete 关键字显式删除赋值运算符重载
        type& operator=(const type&) = delete;
        HANDLE handle = NULLPTR;
        DWORD id = -1;
        function* fn = nullptr;
    };

    template <typename Fn>
    bool create(type& _t, Fn _fn)
    {
        _t.fn = new function(_fn);

        // 创建线程
        _t.handle = CreateThread(NULLPTR,                 // 默认安全属性
                                 0,                       // 默认栈大小
                                 function::start_routine, // 线程函数
                                 _t.fn,                   // 传递给线程函数的参数
                                 0,                       // 默认创建标志
                                 &_t.id                   // 线程 ID
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
        // WaitForSingleObject(_t.handle, INFINITE);
        lite::wait(_t.handle);

        // 关闭线程句柄
        CloseHandle(_t.handle);
    }

    typedef HANDLE native_handle_type;
    typedef DWORD id;
} // namespace detail::thread
