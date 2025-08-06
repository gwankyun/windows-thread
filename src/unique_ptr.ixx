module;
#include "common.h"
#include <cassert>

export module unique_ptr;

EXPORT namespace lite
{
    // 默认删除器
    template <typename T>
    struct default_delete
    {
        void operator()(T* ptr) const
        {
            delete ptr;
        }
    };

    template <>
    struct default_delete<void>
    {
        // 由于 void 指针不能直接删除，这里不实现具体操作，
        // 使用时需要提供自定义删除器
        void operator()(void* ptr) const
        {
            // 这里不做操作，使用时必须提供自定义删除器
        }
    };

    // 数组默认删除器
    template <typename T>
    struct default_delete<T[]>
    {
        void operator()(T* ptr) const
        {
            delete[] ptr;
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
        unique_ptr(T* _ptr = NULLPTR, const Deleter& d = Deleter()) //
            : m_ptr(_ptr), m_deleter(d)
        {
        }

        unique_ptr& operator=(T* _ptr)
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
        T* operator->() const
        {
            assert(m_ptr != NULLPTR);
            return m_ptr;
        }

        /// @brief 返回指向被管理对象的指针
        T* get() const NOEXCEPT
        {
            return m_ptr;
        }

        /// @brief 替换被管理对象
        void reset(T* ptr = NULLPTR) NOEXCEPT
        {
            if (m_ptr != ptr)
            {
                // delete m_ptr;
                m_deleter(m_ptr);
            }
            m_ptr = ptr;
        }

        /// @brief 返回一个指向被管理对象的指针，并释放所有权
        T* release() NOEXCEPT
        {
            T* temp = m_ptr;
            m_ptr = NULLPTR;
            return temp;
        }

        // 获取删除器
        Deleter& get_deleter()
        {
            return m_deleter;
        }

        const Deleter& get_deleter() const
        {
            return m_deleter;
        }

        // 移动构造函数（C++98 用非 const 引用模拟）
        unique_ptr(unique_ptr& other) //
            : m_ptr(other.release()), m_deleter(other.get_deleter())
        {
        }

        // 移动赋值运算符（C++98 用非 const 引用模拟）
        unique_ptr& operator=(unique_ptr& other)
        {
            if (this != &other)
            {
                reset(other.release());
                m_deleter = other.get_deleter();
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
        unique_ptr(void* _ptr = NULLPTR, const Deleter& d = Deleter()) //
            : m_ptr(_ptr), m_deleter(d)
        {
        }

        ~unique_ptr()
        {
            reset();
        }

        // 重载成员访问运算符
        void* operator->() const
        {
            assert(m_ptr != NULLPTR);
            return m_ptr;
        }

        /// @brief 返回指向被管理对象的指针
        void* get() const NOEXCEPT
        {
            return m_ptr;
        }

        /// @brief 替换被管理对象
        void reset(void* ptr = NULLPTR) NOEXCEPT
        {
            if (m_ptr != ptr)
            {
                // delete m_ptr;
                m_deleter(m_ptr);
            }
            m_ptr = ptr;
        }

        /// @brief 返回一个指向被管理对象的指针，并释放所有权
        void* release() NOEXCEPT
        {
            void* temp = m_ptr;
            m_ptr = NULLPTR;
            return temp;
        }

        // 获取删除器
        Deleter& get_deleter()
        {
            return m_deleter;
        }

        const Deleter& get_deleter() const
        {
            return m_deleter;
        }

        // 移动构造函数（C++98 用非 const 引用模拟）
        unique_ptr(unique_ptr& other) //
            : m_ptr(other.release()), m_deleter(other.get_deleter())
        {
        }

        // 移动赋值运算符（C++98 用非 const 引用模拟）
        unique_ptr& operator=(unique_ptr& other)
        {
            if (this != &other)
            {
                reset(other.release());
                m_deleter = other.get_deleter();
            }
            return *this;
        }

      private:
        void* m_ptr;
        Deleter m_deleter;
        NO_COPY_ASSIGN(unique_ptr);
    };

    template <typename T, typename Deleter>
    class unique_ptr<T[], Deleter>
    {
      public:
        typedef T element_type;
        typedef T* pointer;
        unique_ptr(T* _ptr = NULLPTR, const Deleter& d = Deleter()) //
            : m_ptr(_ptr), m_deleter(d)
        {
        }

        unique_ptr& operator=(T* _ptr)
        {
            reset(_ptr);
            return *this;
        }

        ~unique_ptr()
        {
            reset();
        }

        /// @brief 返回指向被管理对象的指针
        T* get() const NOEXCEPT
        {
            return m_ptr;
        }

        /// @brief 替换被管理对象
        void reset(T* ptr = NULLPTR) NOEXCEPT
        {
            if (m_ptr != ptr)
            {
                // delete m_ptr;
                m_deleter(m_ptr);
            }
            m_ptr = ptr;
        }

        /// @brief 返回一个指向被管理对象的指针，并释放所有权
        T* release() NOEXCEPT
        {
            T* temp = m_ptr;
            m_ptr = NULLPTR;
            return temp;
        }

        // 重载下标运算符
        T& operator[](size_t index) const
        {
            return m_ptr[index];
        }

        // 获取删除器
        Deleter& get_deleter()
        {
            return m_deleter;
        }

        const Deleter& get_deleter() const
        {
            return m_deleter;
        }

        // 移动构造函数（C++98 用非 const 引用模拟）
        unique_ptr(unique_ptr& other) //
            : m_ptr(other.release()), m_deleter(other.get_deleter())
        {
        }

        // 移动赋值运算符（C++98 用非 const 引用模拟）
        unique_ptr& operator=(unique_ptr& other)
        {
            if (this != &other)
            {
                reset(other.release());
                m_deleter = other.get_deleter();
            }
            return *this;
        }

      private:
        T* m_ptr;
        Deleter m_deleter;
        NO_COPY_ASSIGN(unique_ptr);
    };
}
