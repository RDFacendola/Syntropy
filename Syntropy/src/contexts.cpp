#include "contexts.h"

namespace syntropy
{
    /// \brief Log context for the syntropy diagnostic system.
    const Context DiagnosticsCtx{ "Syntropy|Diagnostics" };

    /// \brief Log context for the syntropy memory system.
    const Context MemoryCtx{ "Syntropy|Memory" };

    /// \brief Log context for the syntropy reflection system.
    const Context ReflectionCtx{ "Syntropy|Reflection" };

    /// \brief Log context for the syntropy serialization system.
    const Context SerializationCtx{ "Syntropy|Serialization" };

    /// \brief Log context for the syntropy application system.
    const Context ApplicationCtx{ "Syntropy|Application" };
}

