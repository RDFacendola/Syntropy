/// \file contexts.h
/// \brief This header is part of the syntropy module. It contains contexts used by the rest of the module.
/// 
/// \author Raffaele D. Facendola - 2018

#include "containers/context.h"

namespace syntropy
{
    /// \brief Log context for the syntropy diagnostic system.
    extern const Context DiagnosticsCtx;

    /// \brief Log context for the syntropy memory system.
    extern const Context MemoryCtx;

    /// \brief Log context for the syntropy reflection system.
    extern const Context ReflectionCtx;

    /// \brief Log context for the syntropy serialization system.
    extern const Context SerializationCtx;

    /// \brief Log context for the syntropy application system.
    extern const Context ApplicationCtx;
}

