#include  "syntropy/diagnostics/diagnostics_meta.h"

#include "syntropy/reflection/reflection.h"

namespace Syntropy::diagnostics
{
    reflection::AutoRegisterClass<Severity> severity_class;

    reflection::AutoRegisterClass<LogChannel> log_channel_class;

    reflection::AutoRegisterClass<StreamLogChannel> stream_log_channel_class;
    reflection::AutoRegisterClass<FileLogChannel> file_log_channel_class;
}