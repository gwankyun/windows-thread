module;
#include "common.h"
#include <windows.h>

export module critical_section;

export namespace lite
{
    class critical_section
    {
    public:
        typedef CRITICAL_SECTION* native_handle_type;
        critical_section()
        {
            InitializeCriticalSection(handle);
        }

        ~critical_section()
        {
            DeleteCriticalSection(handle);
        }

        void enter()
        {
            EnterCriticalSection(handle);
        }

        void leave()
        {
            LeaveCriticalSection(handle);
        }

        native_handle_type native_handle()
        {
            return handle;
        }

    private:
        native_handle_type handle;
        NO_COPY_ASSIGN(critical_section);
    };
}
