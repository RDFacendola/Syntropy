
/// \file debugger.inl
///
/// \author Raffaele D. Facendola - 2019.

// ===========================================================================

namespace Syntropy::Diagnostics
{
    /************************************************************************/
    /* MACROS                                                               */
    /************************************************************************/

    #undef SYNTROPY_TRAP
    #define SYNTROPY_TRAP \
        SYNTROPY_HAL_TRAP

    #undef SYNTROPY_BREAK
    #define SYNTROPY_BREAK \
        if(Syntropy::Diagnostics::IsDebuggerAttached()){ SYNTROPY_TRAP }
}

// ===========================================================================
