#include "diagnostics/diagnostics_meta.h"

namespace syntropy
{
    namespace reflection
    {
        const Class& ClassOf_Context(ClassOf<diagnostics::Context>());
        const Class& ClassOf_LogChannel(ClassOf<diagnostics::LogChannel>());
    }

    namespace reflection
    {
        const Class& ClassOf_StreamLogChannel(ClassOf<diagnostics::StreamLogChannel>());
        const Class& ClassOf_FileLogChannel(ClassOf<diagnostics::FileLogChannel>());
    }
}
