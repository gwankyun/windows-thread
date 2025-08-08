module;
#include "common.h"
#include <windows.h>

export module thread.detail;
import common;

EXPORT namespace detail_thread
{
    struct Base
    {
        Base() {}
        virtual ~Base() {}
        virtual void run() = 0;
    };

    template <typename F>
    struct Derived : public Base
    {
        explicit Derived(F _f) : f(_f) {}
        ~Derived() {}

        void run() OVERRIDE
        {
            f();
        }

      private:
        F f;
    };

    struct function
    {
        function() {}
        template <typename Fn>
        function(Fn _fn) : base(new Derived<Fn>(_fn))
        {}

        ~function() {}

        void run()
        {
            if (base.get())
            {
                base->run();
            }
        }

        // 线程函数
        static DWORD WINAPI start(LPVOID lpParam)
        {
            // 将传入的参数转换为function指针
            function* self = static_cast<function*>(lpParam);
            if (self != NULLPTR)
            {
                // 调用存储的可调用对象
                self->run();
                return 0;
            }
            // 添加返回值
            return 0;
        }

      private:
        lite::unique_ptr<Base> base;
        NO_COPY_ASSIGN(function);
    };

    struct type
    {
        type() : id(-1) {}

        ~type() {}

        lite::Handle handle;
        DWORD id; // -1
        lite::unique_ptr<function> fn;

      private:
        NO_COPY_ASSIGN(type);
    };

    template <typename Fn>
    bool create(type & _t, Fn _fn)
    {
        _t.fn = new function(_fn);

        // 创建线程
        HANDLE handle = ::CreateThread( // 返回線程柄
            NULLPTR,                    // 默认安全属性
            0,                          // 默认栈大小
            function::start,            // 线程函数
            _t.fn.get(),                // 传递给线程函数的参数
            0,                          // 默认创建标志
            &_t.id                      // 线程 ID
        );
        if (handle == NULLPTR)
        {
            return false;
        }

        _t.handle.reset(handle);
        return true;
    }

    void join(type & _t)
    {
        // 等待线程结束
        lite::wait(_t.handle.get());

        // 关闭线程句柄
        _t.handle.reset();
    }

    typedef HANDLE native_handle_type;
    typedef DWORD id;
} // namespace detail_thread

EXPORT namespace detail
{
    namespace thread = detail_thread;
}
