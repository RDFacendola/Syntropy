
/// \file assert.inl
///
/// \author Raffaele D. Facendola - 2016.

// ===========================================================================

namespace Syntropy
{
    /************************************************************************/
    /* RUN-TIME DIAGNOSTICS                                                 */
    /************************************************************************/

    #undef SYNTROPY_ASSERT
    #define SYNTROPY_ASSERT(condition) \
        if(!(condition)) { SYNTROPY_TRAP; }

    #undef SYNTROPY_UNDEFINED_BEHAVIOR
    #define SYNTROPY_UNDEFINED_BEHAVIOR(condition, message) \
        if(!(condition)) { SYNTROPY_TRAP; }

}

// ===========================================================================
