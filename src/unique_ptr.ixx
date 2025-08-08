module;
#include "common.h"

// #include <algorithm>

#include <cassert>

export module unique_ptr;
import std;

EXPORT namespace lite
{
    // 默认删除器
    template <typename T>
    struct default_delete
    {
        void operator()(T* _ptr) const
        {
            delete _ptr;
        }
    };

    template <>
    struct default_delete<void>
    {
        // 由于 void 指针不能直接删除，这里不实现具体操作，
        // 使用时需要提供自定义删除器
        void operator()(void* _ptr) const
        {
            // 这里不做操作，使用时必须提供自定义删除器
        }
    };

    // 数组默认删除器
    template <typename T>
    struct default_delete<T[]>
    {
        void operator()(T* _ptr) const
        {
            delete[] _ptr;
        }
    };

    template <                               //
        typename T,                          //
        typename Deleter = default_delete<T> //
        >
    class unique_ptr
    {
      public:
        typedef T element_type;
        typedef T* pointer;
        unique_ptr(pointer _ptr = NULLPTR, const Deleter& _d = Deleter()) //
            : m_ptr(_ptr), m_deleter(_d)
        {}

        unique_ptr& operator=(pointer _ptr)
        {
            reset(_ptr);
            return *this;
        }

        ~unique_ptr()
        {
            reset();
        }

        // 重载解引用运算符
        T& operator*() const
        {
            return *m_ptr;
        }

        // 重载成员访问运算符
        pointer operator->() const
        {
            assert(m_ptr != NULLPTR);
            return m_ptr;
        }

        explicit operator bool() const NOEXCEPT
        {
            return get() != NULLPTR;
        }

        /// @brief 返回指向被管理对象的指针
        pointer get() const NOEXCEPT
        {
            return m_ptr;
        }

        /// @brief 替换被管理对象
        void reset(pointer _ptr = NULLPTR) NOEXCEPT
        {
            if (m_ptr != _ptr)
            {
                m_deleter(m_ptr);
            }
            m_ptr = _ptr;
        }

        /// @brief 返回一个指向被管理对象的指针，并释放所有权
        pointer release() NOEXCEPT
        {
            pointer temp = NULLPTR;
            std::swap(temp, m_ptr);
            return temp;
        }

        void swap(unique_ptr& _other) NOEXCEPT
        {
            std::swap(m_ptr, _other.m_ptr);
            std::swap(m_deleter, _other.m_deleter);
        }

        // 获取删除器
        Deleter& get_deleter() NOEXCEPT
        {
            return m_deleter;
        }

        const Deleter& get_deleter() const NOEXCEPT
        {
            return m_deleter;
        }

        // 移动构造函数（C++98 用非 const 引用模拟）
        unique_ptr(unique_ptr& _other) //
            : m_ptr(_other.release()), m_deleter(_other.get_deleter())
        {
            swap(_other);
        }

        // 移动赋值运算符（C++98 用非 const 引用模拟）
        unique_ptr& operator=(unique_ptr& _other)
        {
            if (this != &_other)
            {
                reset(_other.release());
                m_deleter = _other.get_deleter();
            }
            return *this;
        }

      private:
        T* m_ptr;
        Deleter m_deleter;
        NO_COPY_ASSIGN(unique_ptr);
    };

    // void特化
    template <typename Deleter>
    class unique_ptr<void, Deleter>
    {
      public:
        typedef void element_type;
        typedef void* pointer;
        unique_ptr(pointer _ptr = NULLPTR, const Deleter& _d = Deleter()) //
            : m_ptr(_ptr), m_deleter(_d)
        {}

        ~unique_ptr()
        {
            reset();
        }

        /// @brief 返回指向被管理对象的指针
        pointer get() const NOEXCEPT
        {
            return m_ptr;
        }

        explicit operator bool() const NOEXCEPT
        {
            return get() != NULLPTR;
        }

        /// @brief 替换被管理对象
        void reset(pointer _ptr = NULLPTR) NOEXCEPT
        {
            if (m_ptr != _ptr)
            {
                m_deleter(m_ptr);
            }
            m_ptr = _ptr;
        }

        /// @brief 返回一个指向被管理对象的指针，并释放所有权
        pointer release() NOEXCEPT
        {
            pointer temp = NULLPTR;
            std::swap(temp, m_ptr);
            return temp;
        }

        void swap(unique_ptr& _other) NOEXCEPT
        {
            std::swap(m_ptr, _other.m_ptr);
            std::swap(m_deleter, _other.m_deleter);
        }

        // 获取删除器
        Deleter& get_deleter() NOEXCEPT
        {
            return m_deleter;
        }

        const Deleter& get_deleter() const NOEXCEPT
        {
            return m_deleter;
        }

        // 移动构造函数（C++98 用非 const 引用模拟）
        unique_ptr(unique_ptr& _other) //
            : m_ptr(_other.release()), m_deleter(_other.get_deleter())
        {}

        // 移动赋值运算符（C++98 用非 const 引用模拟）
        unique_ptr& operator=(unique_ptr& _other)
        {
            if (this != &_other)
            {
                reset(_other.release());
                m_deleter = _other.get_deleter();
            }
            return *this;
        }

      private:
        void* m_ptr;
        Deleter m_deleter;
        NO_COPY_ASSIGN(unique_ptr);
    };

    // 数组特化
    template <typename T, typename Deleter>
    class unique_ptr<T[], Deleter>
    {
      public:
        typedef T element_type;
        typedef T* pointer;
        unique_ptr(pointer _ptr = NULLPTR, const Deleter& _d = Deleter()) //
            : m_ptr(_ptr), m_deleter(_d)
        {}

        unique_ptr& operator=(pointer _ptr)
        {
            reset(_ptr);
            return *this;
        }

        ~unique_ptr()
        {
            reset();
        }

        /// @brief 返回指向被管理对象的指针
        pointer get() const NOEXCEPT
        {
            return m_ptr;
        }

        explicit operator bool() const NOEXCEPT
        {
            return get() != NULLPTR;
        }

        /// @brief 替换被管理对象
        void reset(pointer _ptr = NULLPTR) NOEXCEPT
        {
            if (m_ptr != _ptr)
            {
                m_deleter(m_ptr);
            }
            m_ptr = _ptr;
        }

        /// @brief 返回一个指向被管理对象的指针，并释放所有权
        pointer release() NOEXCEPT
        {
            pointer temp = NULLPTR;
            std::swap(temp, m_ptr);
            return temp;
        }

        void swap(unique_ptr& _other) NOEXCEPT
        {
            std::swap(m_ptr, _other.m_ptr);
            std::swap(m_deleter, _other.m_deleter);
        }

        // 重载下标运算符
        T& operator[](size_t _index) const
        {
            assert(m_ptr);
            return m_ptr[_index];
        }

        // 获取删除器
        Deleter& get_deleter() NOEXCEPT
        {
            return m_deleter;
        }

        const Deleter& get_deleter() const NOEXCEPT
        {
            return m_deleter;
        }

        // 移动构造函数（C++98 用非 const 引用模拟）
        unique_ptr(unique_ptr& _other) //
            : m_ptr(_other.release()), m_deleter(_other.get_deleter())
        {}

        // 移动赋值运算符（C++98 用非 const 引用模拟）
        unique_ptr& operator=(unique_ptr& _other)
        {
            if (this != &_other)
            {
                reset(_other.release());
                m_deleter = _other.get_deleter();
            }
            return *this;
        }

      private:
        T* m_ptr;
        Deleter m_deleter;
        NO_COPY_ASSIGN(unique_ptr);
    };
}
