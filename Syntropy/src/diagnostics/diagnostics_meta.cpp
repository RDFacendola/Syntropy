#include  "diagnostics/diagnostics_meta.h"

#include "reflection/reflection.h"

namespace syntropy::diagnostics
{
    reflection::AutoRegisterClass<Severity> severity_class;

    reflection::AutoRegisterClass<LogChannel> log_channel_class;

    reflection::AutoRegisterClass<StreamLogChannel> stream_log_channel_class;
    reflection::AutoRegisterClass<FileLogChannel> file_log_channel_class;
}