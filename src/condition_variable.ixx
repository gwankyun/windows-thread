module;
#include "common.h"
#include <windows.h>

export module condition_variable;
import mutex;
import critical_section;

export namespace lite
{
    class condition_variable
    {
      public:
        typedef CONDITION_VARIABLE* native_handle_type;
        condition_variable()
        {
            //InitializeCriticalSection(&cs);
            InitializeConditionVariable(handle);
        }

        ~condition_variable()
        {
            //DeleteCriticalSection(&cs);
        }

        void notify_one() noexcept
        {
            cs.enter();
            WakeConditionVariable(handle);
            cs.leave();
        }

        void notify_all() noexcept
        {
            cs.enter();
            WakeAllConditionVariable(handle);
            cs.leave();
        }

        void wait(lite::unique_lock<lite::mutex>& lock)
        {
            critical_section& cs = lock.mutex()->get_critical_section();
            SleepConditionVariableCS(handle, cs.native_handle(), INFINITE);
        }

        native_handle_type native_handle()
        {
            return handle;
        }

      private:
        critical_section cs; // 臨界區
        native_handle_type handle;
        NO_COPY_ASSIGN(condition_variable);
    };

} // namespace lite
